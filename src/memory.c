/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº -----
 */

#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <stdbool.h>

/* Função que reserva uma zona de memória dinâmica com o tamanho indicado
 * por size, preenche essa zona de memória com o valor 0, e retorna um
 * apontador para a mesma.
 */
void *allocate_dynamic_memory(int size)
{
	return calloc(size, sizeof(char));
}

/* Função que reserva uma zona de memória partilhada com tamanho indicado
 * por size e nome name, preenche essa zona de memória com o valor 0, e
 * retorna um apontador para a mesma. Pode concatenar o id do utilizador
 * resultante da função getuid() a name, para tornar o nome único para
 * a zona de memória.
 */
void *create_shared_memory(char *name, int size)
{
	char full_name[256];
	sprintf(full_name, "/%s%d", name, getuid());

	int fd = shm_open(full_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		printf("erro: create_shared_memory/smh_open()\n");
		return NULL;
	}

	int ret = ftruncate(fd, size);
	if (ret == -1)
	{
		printf("erro: create_shared_memory/ftruncate()\n");
		return NULL;
	}

	void *mem_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (mem_ptr == MAP_FAILED)
	{
		printf("erro: create_shared_memory/mmap()\n");
		return NULL;
	}
	memset(mem_ptr, 0, size);
	return mem_ptr;
}

/* Liberta uma zona de memória dinâmica previamente alocada.
 */
void deallocate_dynamic_memory(void *ptr)
{
	free(ptr);
}

/* Remove uma zona de memória partilhada previamente criada.
 */
void destroy_shared_memory(char *name, void *ptr, int size)
{
	munmap(ptr, size);
	unlink(name);
}

/* Escreve uma transação no buffer de memória partilhada entre a Main e as carteiras.
 * A transação deve ser escrita numa posição livre do buffer,
 * tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
 * Se não houver nenhuma posição livre, não escreve nada e a transação é perdida.
 */
void write_main_wallets_buffer(struct ra_buffer *buffer, int buffer_size, struct transaction *tx)
{
	for (int i = 0; i < buffer_size; i++)
	{
		if (buffer->ptrs[i] == 0)
		{
			buffer->ptrs[i] = 1;
			buffer->buffer[i] = *tx;
			return;
		}
	}
}

bool isFull(struct circ_buffer *buffer, int buffer_size)
{
	return (buffer->ptrs->in + 1) % buffer_size == buffer->ptrs->out;
}
bool isEmpty(struct circ_buffer *buffer)
{
	return buffer->ptrs->in == buffer->ptrs->out;
}

/* Função que escreve uma transação no buffer de memória partilhada entre as carteiras e os servidores.
 * A transação deve ser escrita numa posição livre do buffer, tendo em conta o tipo de buffer
 * e as regras de escrita em buffers desse tipo. Se não houver nenhuma posição livre, não escreve nada.
 */
void write_wallets_servers_buffer(struct circ_buffer *buffer, int buffer_size, struct transaction *tx)
{
	if (!isFull(buffer, buffer_size))
	{
		buffer->buffer[buffer->ptrs->in++] = *tx;
		buffer->ptrs->in %= buffer_size;
	}
}

/* Função que escreve uma transação no buffer de memória partilhada entre os servidores e a Main, a qual
 * servirá de comprovativo da execução da transação. A transação deve ser escrita numa posição livre do
 * buffer, tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
 * Se não houver nenhuma posição livre, não escreve nada.
 */
void write_servers_main_buffer(struct ra_buffer *buffer, int buffer_size, struct transaction *tx)
{
	for (int i = 0; i < buffer_size; i++)
	{
		if (buffer->ptrs[i] == 0)
		{
			buffer->ptrs[i] = 1;
			buffer->buffer[i] = *tx;
			return;
		}
	}
}

/* Função que lê uma transação do buffer entre a Main e as carteiras, se houver alguma disponível para ler
 * e que seja direcionada a própria carteira que está a tentar ler. A leitura deve ser feita tendo em conta
 * o tipo de buffer e as regras de leitura em buffers desse tipo. Se não houver nenhuma transação disponível,
 * afeta tx->id com o valor -1.
 */
void read_main_wallets_buffer(struct ra_buffer *buffer, int wallet_id, int buffer_size, struct transaction *tx)
{
	for (int i = 0; i < buffer_size; i++)
	{
		if (buffer->ptrs[i] == 1 && buffer->buffer[i].src_id == wallet_id)
		{
			buffer->ptrs[i] = 0;
			*tx = buffer->buffer[i];
			return;
		}
	}
	tx->id = -1;
}

/* Função que lê uma transação do buffer entre as carteiras e os servidores, se houver alguma disponível para ler.
 * A leitura deve ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo. Qualquer
 * servidor pode ler qualquer transação deste buffer. Se não houver nenhuma transação disponível,
 * afeta tx->id com o valor -1.
 */
void read_wallets_servers_buffer(struct circ_buffer *buffer, int buffer_size, struct transaction *tx)
{
	if (!isEmpty(buffer))
	{
		*tx = buffer->buffer[buffer->ptrs->out++];
		buffer->ptrs->out %= buffer_size;
	}
	else
	{
		tx->id = -1;
	}
}

/* Função que lê uma transação do buffer entre os servidores e a Main, se houver alguma disponível para ler
 * e que tenha o tx->id igual a tx_id. A leitura deve ser feita tendo em conta o tipo de buffer e as regras
 * de leitura em buffers desse tipo. Se não houver nenhuma transação disponível, afeta tx->id com o valor -1.
 */
void read_servers_main_buffer(struct ra_buffer *buffer, int tx_id, int buffer_size, struct transaction *tx)
{
	for (int i = 0; i < buffer_size; i++)
	{
		if (buffer->ptrs[i] && buffer->buffer[i].id == tx_id)
		{
			buffer->ptrs[i] = 0;
			*tx = buffer->buffer[i];
			return;
		}
	}
	tx->id = -1;
}
