#include <stdlib.h>

#include "memory.h"

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
	// TODO: test case correct to shared memory

	return malloc(size);
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
}

/* Escreve uma transação no buffer de memória partilhada entre a Main e as carteiras.
 * A transação deve ser escrita numa posição livre do buffer,
 * tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
 * Se não houver nenhuma posição livre, não escreve nada e a transação é perdida.
 */
void write_main_wallets_buffer(struct ra_buffer *buffer, int buffer_size, struct transaction *tx)
{
}

/* Função que escreve uma transação no buffer de memória partilhada entre as carteiras e os servidores.
 * A transação deve ser escrita numa posição livre do buffer, tendo em conta o tipo de buffer
 * e as regras de escrita em buffers desse tipo. Se não houver nenhuma posição livre, não escreve nada.
 */
void write_wallets_servers_buffer(struct circ_buffer *buffer, int buffer_size, struct transaction *tx)
{
}

/* Função que escreve uma transação no buffer de memória partilhada entre os servidores e a Main, a qual
 * servirá de comprovativo da execução da transação. A transação deve ser escrita numa posição livre do
 * buffer, tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
 * Se não houver nenhuma posição livre, não escreve nada.
 */
void write_servers_main_buffer(struct ra_buffer *buffer, int buffer_size, struct transaction *tx)
{
}

/* Função que lê uma transação do buffer entre a Main e as carteiras, se houver alguma disponível para ler
 * e que seja direcionada a própria carteira que está a tentar ler. A leitura deve ser feita tendo em conta
 * o tipo de buffer e as regras de leitura em buffers desse tipo. Se não houver nenhuma transação disponível,
 * afeta tx->id com o valor -1.
 */
void read_main_wallets_buffer(struct ra_buffer *buffer, int wallet_id, int buffer_size, struct transaction *tx)
{
}

/* Função que lê uma transação do buffer entre as carteiras e os servidores, se houver alguma disponível para ler.
 * A leitura deve ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo. Qualquer
 * servidor pode ler qualquer transação deste buffer. Se não houver nenhuma transação disponível,
 * afeta tx->id com o valor -1.
 */
void read_wallets_servers_buffer(struct circ_buffer *buffer, int buffer_size, struct transaction *tx)
{
}

/* Função que lê uma transação do buffer entre os servidores e a Main, se houver alguma disponível para ler
 * e que tenha o tx->id igual a tx_id. A leitura deve ser feita tendo em conta o tipo de buffer e as regras
 * de leitura em buffers desse tipo. Se não houver nenhuma transação disponível, afeta tx->id com o valor -1.
 */
void read_servers_main_buffer(struct ra_buffer *buffer, int tx_id, int buffer_size, struct transaction *tx)
{
}
