#include "main.h"
#include "memory.h"
#include "process.h"
#include <string.h>
#include <stdio.h>

/* Função que lê do stdin com o scanf apropriado para cada tipo de dados
 * e valida os argumentos da aplicação, incluindo o saldo inicial,
 * o número de carteiras, o número de servidores, o tamanho dos buffers
 * e o número máximo de transações. Guarda essa informação na estrutura info_container.
 */
void main_args(int argc, char *argv[], struct info_container *info)
{
	// for (int i = 0; i < argc; i++)
	// {
	// 	printf("%d - %s\n", i, argv[i]);
	// }

	sscanf(argv[1], "%f", &info->init_balance);
	sscanf(argv[2], "%d", &info->n_wallets);
	sscanf(argv[3], "%d", &info->n_servers);
	sscanf(argv[4], "%d", &info->buffers_size);
	sscanf(argv[5], "%d", &info->max_txs);

	printf("[Main] Parâmetros corretos!\n\n\n");
	// printf("initial balance:  %0.2f\n", info->init_balance);
	// printf("wallet count: ... %d\n", info->n_wallets);
	// printf("server count: ... %d\n", info->n_servers);
	// printf("buffers size: ... %d\n", info->buffers_size);
	// printf("max transactions: %d\n", info->max_txs);
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

	info->terminate = create_shared_memory(ID_SHM_TERMINATE, sizeof(int) * 1);

	buffs->buff_main_wallets->buffer = create_shared_memory(ID_SHM_MAIN_WALLETS_BUFFER, sizeof(struct transaction) * info->buffers_size);
	buffs->buff_main_wallets->ptrs = create_shared_memory(ID_SHM_MAIN_WALLETS_PTR, sizeof(int) * info->buffers_size);

	buffs->buff_servers_main->buffer = create_shared_memory(ID_SHM_SERVERS_MAIN_BUFFER, sizeof(struct transaction) * info->buffers_size);
	buffs->buff_servers_main->ptrs = create_shared_memory(ID_SHM_SERVERS_MAIN_PTR, sizeof(int) * info->buffers_size);

	buffs->buff_wallets_servers->buffer = create_shared_memory(ID_SHM_WALLETS_SERVERS_BUFFER, sizeof(struct transaction) * info->buffers_size);
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
	int trx_counter = 0;
	while (!*info->terminate)
	{
		char buff[5];
		printf("[Main] Introduzir operação: ");
		scanf("%s", buff);
		if (!strcmp("bal", buff))
		{
			print_balance(info);
		}
		else if (!strcmp("trx", buff))
		{
			create_transaction(&trx_counter, info, buffs);
		}
		else if (!strcmp("rcp", buff))
		{
			receive_receipt(info, buffs);
		}
		else if (!strcmp("stat", buff))
		{
			print_stat(trx_counter, info);
		}
		else if (!strcmp("help", buff))
		{
			help();
		}
		else if (!strcmp("end", buff))
		{
			end_execution(info, buffs);
		}
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
	*info->terminate = 1;
	wait_processes(info);
	write_final_statistics(info);
}

/* Aguarda a terminação dos processos filhos previamente criados. Pode usar
 * a função wait_process do process.h
 */
void wait_processes(struct info_container *info)
{
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
	scanf("%d", &id);
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
	if (info->max_txs == *tx_counter)
	{
		printf("[Main] O número máximo de transações foi alcançado!\n\n");
		return;
	}

	struct transaction tx;
	scanf("%d %d %f", &tx.src_id, &tx.dest_id, &tx.amount);
	tx.id = *tx_counter;
	(*tx_counter)++;

	printf("[Main] A transação %d foi criada para transferir %0.2f SOT da carteira %d para a cartera %d!\n", tx.id, tx.amount, tx.src_id, tx.dest_id);
	write_main_wallets_buffer(buffs->buff_main_wallets, info->buffers_size, &tx);
}

/* Tenta ler o recibo da transação (identificada por id, o qual ainda está no
 * stdin a espera de ser lido dentro da função com o scanf) do buffer de memória
 * partilhada entre os servidores e a main, comprovando a conclusão da transação.
 */
void receive_receipt(struct info_container *info, struct buffers *buffs)
{
	int id;
	scanf("%d", &id);
	struct transaction trx;
	read_servers_main_buffer(buffs->buff_servers_main, id, info->buffers_size, &trx);
	if (trx.id == -1)
	{
		printf("[Main] O comprovativo da execução da transação %d não está disponível.\n\n", trx.id);
	}
	else
	{
		printf("[Main] O comprovativo da execução %d foi obtido.\n", trx.id);
		printf("[Main] O comprovativo da transação id %d contém src_id %d, dest_id %d, amount %0.2f e foi assinado pela carteira %d e servidor %d.\n\n", trx.id, trx.src_id, trx.dest_id, trx.amount, trx.wallet_signature, trx.wallet_signature);
	}
}

/* Imprime as estatísticas atuais do sistema, incluindo as configurações iniciais
 * do sistema, o valor das variáveis terminate e contador da transações criadas,
 * os pids dos processos e as restantes informações (e.g., número de transações
 * assinadas pela entidade e saldo atual no caso das carteiras).
 */
void print_stat(int tx_counter, struct info_container *info)
{
	printf("- Configuração inicial:\n");
	printf("        Propriedade     Valor\n");
	printf("        init_balance    %0.2f\n", info->init_balance);
	printf("        n_wallets       %d\n", info->n_wallets);
	printf("        n_servers       %d\n", info->n_servers);
	printf("        buffers_size:   %d\n", info->buffers_size);
	printf("        max_txs         %d\n", info->max_txs);
	printf("- Variáveis atuais:\n");
	printf("        terminate       %d\n", *info->terminate);
	printf("        tx_count:       %d\n", tx_counter);
	printf("- Informação sobre as carteiras:\n");
	printf("        Carteira        PID             Saldo           Transações Assinadas\n");
	for (int i = 0; i < info->n_wallets; i++)
	{
		printf("        %d               %d           %0.2f SOT       %d\n", i, info->wallets_pids[i], info->balances[i], info->wallets_stats[i]);
	}
	printf("- Informação sobre os servidores:\n");
	printf("        Servidor        PID             Transações Processadas\n");
	for (int i = 0; i < info->n_servers; i++)
	{
		printf("        %d               %d           %d\n", i, info->servers_pids[i], info->servers_stats[i]);
	}
	printf("\n");
}

/* Exibe informações sobre os comandos disponíveis na aplicação.
 */
void help()
{
	printf("[Main] Operações disponíveis:\n");
	printf("[Main]  bal id - consultar o saldo da carteira identificada por id.\n");
	printf("[Main]  trx src_id dest_id amount - criar uma nova transação.\n");
	printf("[Main]  rcp id - obter o comprovativo da transação de número id.\n");
	printf("[Main]  help - imprime a informação sobre as operações disponíveis.\n");
	printf("[Main]  end - termina a execução do SOchain.\n\n");
}

/* Função principal do SOchain. Inicializa o sistema, chama as funções de alocação
 * de memória, a de criação de processos filhos, a de interação do utilizador
 * e aguarda o encerramento dos processos para chamar as funções para libertar
 * a memória alocada.
 */

#include <stdbool.h>
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
	user_interaction(info, buffs);
	// release memory before terminating
	destroy_shared_memory_structs(info, buffs);
	destroy_dynamic_memory_structs(info, buffs);
	return 0;
}