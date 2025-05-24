/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#define _POSIX_C_SOURCE 200809L
#include "main.h"
#include "main-private.h"
#include "memory.h"
#include "process.h"
#include "ctime.h"
#include "synchronization.h"
#include "csignal.h"
#include "csettings.h"
#include "synchronization-private.h"
#include "cstats.h"
#include "clog.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/* Função que lê do stdin com o scanf apropriado para cada tipo de dados
 * e valida os argumentos da aplicação, incluindo o saldo inicial,
 * o número de carteiras, o número de servidores, o tamanho dos buffers
 * e o número máximo de transações. Guarda essa informação na estrutura info_container.
 */
void main_args(int argc, char *argv[], struct info_container *info)
{
	if (argc != 3)
	{
		printf("[Main] Uso: ./SOchain args.txt settings.txt\n");
		exit(1);
	}
	if (init_args(info, argv[1]))
	{
		printf("[Main] Valores Incorretos! Exemplo de uso:\n"
			   "100.0\n5\n2\n5\n5\n");
		exit(1);
	}

	if (init_settings(argv[2]))
	{
		printf("[Main] Valores Incorretos! Exemplo de uso:\n"
			   "log.txt\nstats.txt\n10\n");
		exit(1);
	}

	printf("[Main] Parâmetros corretos!\n\n\n");
}

/* Função que reserva a memória dinâmica necessária, por exemplo,
 * para os arrays *_pids de info_container. Para tal, pode ser usada
 * a função allocate_dynamic_memory do memory.h.
 */
void create_dynamic_memory_structs(struct info_container *info, struct buffers *buffs)
{
	info->wallets_pids = allocate_dynamic_memory(sizeof(int) * info->n_wallets);
	info->servers_pids = allocate_dynamic_memory(sizeof(int) * info->n_servers);

	buffs->buff_main_wallets = allocate_dynamic_memory(sizeof(struct ra_buffer));

	buffs->buff_servers_main = allocate_dynamic_memory(sizeof(struct ra_buffer));

	buffs->buff_wallets_servers = allocate_dynamic_memory(sizeof(struct circ_buffer));
}

/* Função que reserva a memória partilhada necessária para a execução
 * do SOchain, incluindo buffers e arrays partilhados. É necessário
 * reservar memória partilhada para todos os buffers da estrutura
 * buffers, incluindo tanto os buffers em si como os respetivos
 * pointers, assim como para os arrays *_stats, balances e a variável
 * terminate do info_container. Pode ser usada a função
 * create_shared_memory do memory.h.
 */
void create_shared_memory_structs(struct info_container *info, struct buffers *buffs)
{
	info->balances = create_shared_memory(ID_SHM_BALANCES, sizeof(float) * info->n_wallets);

	for (int i = 0; i < info->n_wallets; i++)
	{
		info->balances[i] = info->init_balance;
	}

	info->wallets_stats = create_shared_memory(ID_SHM_WALLETS_STATS, sizeof(int) * info->n_wallets);
	info->servers_stats = create_shared_memory(ID_SHM_SERVERS_STATS, sizeof(int) * info->n_servers);

	info->terminate = create_shared_memory(ID_SHM_TERMINATE, sizeof(int));

	info->sems = create_all_semaphores(info->buffers_size);

	buffs->buff_main_wallets->buffer = create_shared_memory(ID_SHM_MAIN_WALLETS_BUFFER, sizeof(struct transaction) * info->buffers_size);
	buffs->buff_main_wallets->ptrs = create_shared_memory(ID_SHM_MAIN_WALLETS_PTR, sizeof(int) * info->buffers_size);

	buffs->buff_servers_main->buffer = create_shared_memory(ID_SHM_SERVERS_MAIN_BUFFER, sizeof(struct transaction) * info->buffers_size);
	buffs->buff_servers_main->ptrs = create_shared_memory(ID_SHM_SERVERS_MAIN_PTR, sizeof(int) * info->buffers_size);

	buffs->buff_wallets_servers->buffer = create_shared_memory(ID_SHM_WALLETS_SERVERS_BUFFER, sizeof(struct transaction) * info->buffers_size);
	for (int i = 0; i < info->buffers_size; i++)
	{
		buffs->buff_wallets_servers->buffer[i].id = -1;
	}
	buffs->buff_wallets_servers->ptrs = create_shared_memory(ID_SHM_WALLETS_SERVERS_PTR, sizeof(struct pointers));
}

/* Liberta a memória dinâmica previamente reservada. Pode utilizar a
 * função deallocate_dynamic_memory do memory.h
 */
void destroy_dynamic_memory_structs(struct info_container *info, struct buffers *buffs)
{
	deallocate_dynamic_memory(info->servers_pids);
	deallocate_dynamic_memory(info->wallets_pids);

	deallocate_dynamic_memory(buffs->buff_main_wallets);
	deallocate_dynamic_memory(buffs->buff_servers_main);
	deallocate_dynamic_memory(buffs->buff_wallets_servers);
}

/* Liberta a memória partilhada previamente reservada. Pode utilizar a
 * função destroy_shared_memory do memory.h
 */
void destroy_shared_memory_structs(struct info_container *info, struct buffers *buffs)
{
	destroy_shared_memory(ID_SHM_BALANCES, info->balances, sizeof(int) * info->n_wallets);

	destroy_shared_memory(ID_SHM_WALLETS_STATS, info->wallets_stats, sizeof(int) * info->n_wallets);
	destroy_shared_memory(ID_SHM_SERVERS_STATS, info->servers_stats, sizeof(int) * info->n_servers);

	destroy_shared_memory(ID_SHM_TERMINATE, info->terminate, sizeof(int) * 1);

	destroy_semaphores(info->sems);

	destroy_shared_memory(ID_SHM_MAIN_WALLETS_BUFFER, buffs->buff_main_wallets->buffer, sizeof(struct transaction) * info->buffers_size);
	destroy_shared_memory(ID_SHM_MAIN_WALLETS_PTR, buffs->buff_main_wallets->ptrs, sizeof(int) * info->buffers_size);

	destroy_shared_memory(ID_SHM_SERVERS_MAIN_BUFFER, buffs->buff_servers_main->buffer, sizeof(struct transaction) * info->buffers_size);
	destroy_shared_memory(ID_SHM_SERVERS_MAIN_PTR, buffs->buff_servers_main->ptrs, sizeof(int) * info->buffers_size);

	destroy_shared_memory(ID_SHM_WALLETS_SERVERS_BUFFER, buffs->buff_wallets_servers->buffer, sizeof(struct transaction) * info->buffers_size);
	destroy_shared_memory(ID_SHM_WALLETS_SERVERS_PTR, buffs->buff_wallets_servers->ptrs, sizeof(struct pointers));
}

/* Função que cria os processos das carteiras e servidores.
 * Os PIDs resultantes são armazenados nos arrays apropriados
 * da estrutura info_container.
 */
void create_processes(struct info_container *info, struct buffers *buffs)
{
	for (int i = 0; i < info->n_wallets; i++)
	{
		info->wallets_pids[i] = launch_wallet(i, info, buffs);
	}
	for (int i = 0; i < info->n_servers; i++)
	{
		info->servers_pids[i] = launch_server(i, info, buffs);
	}
}

/* Função responsável pela interação com o utilizador.
 * Permite o utilizador pedir para visualizar saldos, criar
 * transações, consultar recibos, ver estatísticas do sistema e
 * encerrar a execução.
 */
void user_interaction(struct info_container *info, struct buffers *buffs)
{
	int tx_counter = 0;

	reset_alarm();
	while (!read_terminate(info))
	{
		if (alarmed)
		{
			print_alarm_stats(info, buffs);
			alarmed = false;
		}

		printf("[Main] Introduzir operação: ");
		char buff[5];
		if (!safe_scanf(info, buffs, "%s", buff))
		{
			continue;
		}
		else if (!strcmp("bal", buff))
		{
			print_balance(info);
			// Evitar segfault quando interrupt é enviado enquanto se lê a carteira para imprimir o saldo
			// porque a função print balance não recebe buffs
			if (get_interrupt(info))
			{
				end_execution(info, buffs);
			}
		}
		else if (!strcmp("trx", buff))
		{
			create_transaction(&tx_counter, info, buffs);
		}
		else if (!strcmp("rcp", buff))
		{
			receive_receipt(info, buffs);
		}
		else if (!strcmp("stat", buff))
		{
			print_stat(tx_counter, info);
		}
		else if (!strcmp("help", buff))
		{
			help();
		}
		else if (!strcmp("end", buff))
		{
			log_format("end");
			end_execution(info, buffs);
		}
		else
		{
			printf("[Main] Operação não reconhecida, insira 'help' para assistência.\n\n");
		}

		buff[0] = '\0';
		nanosleep(&ts, NULL);
	}
}

/* Função que imprime as estatísticas finais do SOchain, incluindo
 * o número de transações assinadas por cada carteira e processadas
 * por cada servidor.
 */
void write_final_statistics(struct info_container *info)
{
	printf("[Main] A encerrar a execução do SOchain! As estatísticas da execução são:\n");
	for (int i = 0; i < info->n_wallets; i++)
	{
		printf("[Main] A carteira %d assinou %d transações e terminou com %2.0f SOT!\n", i, info->wallets_stats[i], info->balances[i]);
	}

	for (int i = 0; i < info->n_servers; i++)
	{
		printf("[Main] O servidor %d assinou %d transações!\n", i, info->servers_stats[i]);
	}
}

/* Termina a execução do programa. Deve atualizar a flag terminate e,
 * em seguida, aguardar a terminação dos processos filhos, escrever as
 * estatísticas finais e retornar.
 */
void end_execution(struct info_container *info, struct buffers *buffs)
{
	// printf("[Debug] A terminar a execução do SOchain!\n");
	sem_wait(info->sems->terminate_mutex);
	*info->terminate = 1;
	sem_post(info->sems->terminate_mutex);
	wait_processes(info);
	write_final_statistics(info);
	write_stats(info, buffs);
}

/* Aguarda a terminação dos processos filhos previamente criados. Pode usar
 * a função wait_process do process.h
 */
void wait_processes(struct info_container *info)
{
	wake_up_processes(info);
	for (int i = 0; i < info->n_wallets; i++)
	{
		wait_process(info->wallets_pids[i]);
	}

	for (int i = 0; i < info->n_servers; i++)
	{
		wait_process(info->servers_pids[i]);
	}
}

/* Imprime o saldo atual de uma carteira identificada pelo id que ainda está
 * no stdin a espera de ser lido com o scanf dentro da função
 */
void print_balance(struct info_container *info)
{
	int id;
	if (!safe_scanf(info, (struct buffers *)NULL, "%d", &id)) // TODO
	{
		return;
	}
	log_format("bal %d", id);
	if (id < 0 || id >= info->n_wallets)
	{
		printf("[Main] A carteira %d não existe!\n\n", id);
		return;
	}
	printf("[Main] O saldo da carteira %d é de %0.2f SOT atualmente.\n\n", id, info->balances[id]);
}

/* Cria uma nova transação com os dados inseridos pelo utilizador na linha de
 * comandos (e que ainda estão no stdin a espera de serem lidos com o scanf
 * dentro da função), escreve-a no buffer de memória partilhada entre a main
 * e as carteiras e atualiza o contador de transações criadas tx_counter. Caso
 * a aplicação já tenha atingido o número máximo de transações permitidas
 * a função retorna apenas uma mensagem de erro e não cria a nova transação.
 */
void create_transaction(int *tx_counter, struct info_container *info, struct buffers *buffs)
{
	if (info->max_txs == *tx_counter + 1)
	{
		if (!safe_scanf(info, buffs, "%d %d %f", &(int){0}, &(int){0}, &(float){0}))
		{
			// FLUSHED STDIN
			return;
		}
		printf("[Main] O número máximo de transações foi alcançado!\n\n");
		return;
	}

	struct transaction tx = {.wallet_signature = -1, .server_signature = -1};
	if (!safe_scanf(info, buffs, "%d %d %f", &tx.src_id, &tx.dest_id, &tx.amount))
	{
		return;
	}
	tx.id = (*tx_counter)++;

	log_format("trx %d %d %f", tx.src_id, tx.dest_id, tx.amount);

	if (tx.src_id < 0 || tx.src_id >= info->n_wallets)
	{
		printf("[Main] A carteira de origem %d não existe!\n\n", tx.src_id);
		return;
	}
	if (tx.dest_id < 0 || tx.dest_id >= info->n_wallets)
	{
		printf("[Main] A carteira de destino %d não existe!\n\n", tx.dest_id);
		return;
	}

	clock_gettime(CLOCK_REALTIME, &tx.change_time.created);

	// SEND
	sem_wait(info->sems->main_wallet->free_space);
	sem_wait(info->sems->main_wallet->mutex);

	if (read_terminate(info))
	{
		return;
	}
	write_main_wallets_buffer(buffs->buff_main_wallets, info->buffers_size, &tx);

	sem_post(info->sems->main_wallet->mutex);
	sem_post(info->sems->main_wallet->unread);

	printf("[Main] A transação %d foi criada para transferir %0.2f SOT da carteira %d para a carteira %d!\n",
		   tx.id, tx.amount, tx.src_id, tx.dest_id);
}

/* Tenta ler o recibo da transação (identificada por id, o qual ainda está no
 * stdin a espera de ser lido dentro da função com o scanf) do buffer de memória
 * partilhada entre os servidores e a main, comprovando a conclusão da transação.
 */
void receive_receipt(struct info_container *info, struct buffers *buffs)
{
	int id;
	struct transaction tx;

	if (!safe_scanf(info, buffs, "%d", &id))
	{
		return;
	}

	log_format("rcp %d", id);

	// RECEIVE
	if (sem_timedwait(info->sems->server_main->unread, &ts) == EAGAIN)
	{
		printf("[Main] O comprovativo da execução da transação %d não está disponível.\n\n", tx.id);
		return;
	}

	sem_wait(info->sems->server_main->mutex);

	if (read_terminate(info))
	{
		return;
	}

	read_servers_main_buffer(buffs->buff_servers_main, id, info->buffers_size, &tx);

	sem_post(info->sems->server_main->mutex);
	sem_post(info->sems->server_main->free_space);

	// PRINT
	if (tx.id == -1)
	{
		printf("[Main] O comprovativo da execução da transação %d não está disponível.\n\n", tx.id);
	}
	else
	{
		printf("[Main] O comprovativo da execução %d foi obtido.\n"
			   "[Main] O comprovativo da transação id %d contém src_id %d, dest_id %d, amount %0.2f e foi assinado pela carteira %d e servidor %d.\n\n",
			   tx.id, tx.id, tx.src_id, tx.dest_id, tx.amount, tx.wallet_signature, tx.wallet_signature);
	}
}

/* Imprime as estatísticas atuais do sistema, incluindo as configurações iniciais
 * do sistema, o valor das variáveis terminate e contador da transações criadas,
 * os pids dos processos e as restantes informações (e.g., número de transações
 * assinadas pela entidade e saldo atual no caso das carteiras).
 */
void print_stat(int tx_counter, struct info_container *info)
{
	printf("- Configuração inicial:\n"
		   "        Propriedade     Valor\n"
		   "        init_balance    %0.2f\n"
		   "        n_wallets       %d\n"
		   "        n_servers       %d\n"
		   "        buffers_size:   %d\n"
		   "        max_txs         %d\n"
		   "- Variáveis atuais:\n"
		   "        terminate       %d\n"
		   "        tx_count:       %d\n"
		   "- Informação sobre as carteiras:\n"
		   "        Carteira        PID             Saldo           Transações Assinadas\n",
		   info->init_balance,
		   info->n_wallets,
		   info->n_servers,
		   info->buffers_size,
		   info->max_txs,
		   read_terminate(info),
		   tx_counter);
	log_format("stat %d", tx_counter);

	char SOT_str[64];

	for (int i = 0; i < info->n_wallets; i++)
	{
		sprintf(SOT_str, "%.2f SOT", info->balances[i]);
		printf("        %-10d      %-10d      %-15s %d\n",
			   i, info->wallets_pids[i], SOT_str, info->wallets_stats[i]);
	}
	printf("- Informação sobre os servidores:\n"
		   "        Servidor        PID             Transações Processadas\n");
	for (int i = 0; i < info->n_servers; i++)
	{
		printf("        %-10d      %-10d      %d\n", i, info->servers_pids[i], info->servers_stats[i]);
	}
	printf("\n");
}

/* Exibe informações sobre os comandos disponíveis na aplicação.
 */
void help()
{
	printf("[Main] Operações disponíveis:\n"
		   "[Main]  bal id - consultar o saldo da carteira identificada por id.\n"
		   "[Main]  trx src_id dest_id amount - criar uma nova transação.\n"
		   "[Main]  rcp id - obter o comprovativo da transação de número id.\n"
		   "[Main]  help - imprime a informação sobre as operações disponíveis.\n"
		   "[Main]  end - termina a execução do SOchain.\n\n");
	log_format("help");
}

/* Função principal do SOchain. Inicializa o sistema, chama as funções de alocação
 * de memória, a de criação de processos filhos, a de interação do utilizador
 * e aguarda o encerramento dos processos para chamar as funções para libertar
 * a memória alocada.
 */
int main(int argc, char *argv[])
{
	// init data structures
	struct info_container *info = allocate_dynamic_memory(sizeof(struct info_container));
	struct buffers *buffs = allocate_dynamic_memory(sizeof(struct buffers));

	// execute main code
	main_args(argc, argv, info);
	create_dynamic_memory_structs(info, buffs);
	create_shared_memory_structs(info, buffs);
	create_processes(info, buffs);
	init_signal_handlers();
	user_interaction(info, buffs);

	// release memory before terminating
	destroy_shared_memory_structs(info, buffs);
	destroy_dynamic_memory_structs(info, buffs);

	deallocate_dynamic_memory(info);
	deallocate_dynamic_memory(buffs);
	return 0;
}

/*
Acorda todos os processos filhos (que estejam a dormirem sem_wait), de forma a estes poderem terminar.
 */
void wake_up_processes(struct info_container *info)
{
	// printf("dumping\n");
	for (int i = 0; i < info->n_wallets; i++)
	{
		sem_post(info->sems->main_wallet->unread);
		sem_post(info->sems->main_wallet->mutex);
		sem_post(info->sems->wallet_server->free_space);
		sem_post(info->sems->wallet_server->mutex);
	}
	for (int i = 0; i < info->n_servers; i++)
	{
		sem_post(info->sems->wallet_server->unread);
		sem_post(info->sems->wallet_server->mutex);
		sem_post(info->sems->server_main->free_space);
		sem_post(info->sems->server_main->mutex);
	}

	sem_post(info->sems->server_main->unread);
	sem_post(info->sems->server_main->mutex);

	sem_post(info->sems->main_wallet->free_space);
	sem_post(info->sems->main_wallet->mutex);
}

// PRIVATE

static bool safe_scanf(struct info_container *info, struct buffers *buffs, const char *format, ...)
{
	va_list args;
	va_start(args, format);

	vscanf(format, args);
	if (get_alarm())
	{
		reset_alarm();
		alarmed = true;
		return false;
	}
	if (get_interrupt())
	{
		printf("\n");
		if (buffs == NULL)
		{
			// Evitar segfault quando interrupt é enviado enquanto se lê a carteira para imprimir o saldo
			// porque a função print balance não recebe buffs
			return true;
		}
		end_execution(info, buffs);
		return false;
	}

	va_end(args);
	return true;
}