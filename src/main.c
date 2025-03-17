#include "memory.h"
#include "main.h"

#include <stdio.h>

/* Função que lê do stdin com o scanf apropriado para cada tipo de dados
 * e valida os argumentos da aplicação, incluindo o saldo inicial,
 * o número de carteiras, o número de servidores, o tamanho dos buffers
 * e o número máximo de transações. Guarda essa informação na estrutura info_container.
 */
void main_args(int argc, char *argv[], struct info_container *info)
{
	sscanf(argv[0], "%f", &info->init_balance);
	sscanf(argv[1], "%d", &info->n_wallets);
	sscanf(argv[2], "%d", &info->n_servers);
	sscanf(argv[3], "%d", &info->buffers_size);
	sscanf(argv[4], "%d", &info->max_txs);

	printf("initial balance:  %0.2f\n", info->init_balance);
	printf("wallet count: ... %d\n", info->n_wallets);
	printf("server count: ... %d\n", info->n_servers);
	printf("buffers size: ... %d\n", info->buffers_size);
	printf("max transactions: %d\n", info->max_txs);
}

/* Função que reserva a memória dinâmica necessária, por exemplo,
 * para os arrays *_pids de info_container. Para tal, pode ser usada
 * a função allocate_dynamic_memory do memory.h.
 */
void create_dynamic_memory_structs(struct info_container *info, struct buffers *buffs)
{
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
}

/* Liberta a memória dinâmica previamente reservada. Pode utilizar a
 * função deallocate_dynamic_memory do memory.h
 */
void destroy_dynamic_memory_structs(struct info_container *info, struct buffers *buffs)
{
}

/* Liberta a memória partilhada previamente reservada. Pode utilizar a
 * função destroy_shared_memory do memory.h
 */
void destroy_shared_memory_structs(struct info_container *info, struct buffers *buffs)
{
}

/* Função que cria os processos das carteiras e servidores.
 * Os PIDs resultantes são armazenados nos arrays apropriados
 * da estrutura info_container.
 */
void create_processes(struct info_container *info, struct buffers *buffs)
{
}

/* Função responsável pela interação com o utilizador.
 * Permite o utilizador pedir para visualizar saldos, criar
 * transações, consultar recibos, ver estatísticas do sistema e
 * encerrar a execução.
 */
void user_interaction(struct info_container *info, struct buffers *buffs)
{
}

/* Função que imprime as estatísticas finais do SOchain, incluindo
 * o número de transações assinadas por cada carteira e processadas
 * por cada servidor.
 */
void write_final_statistics(struct info_container *info)
{
}

/* Termina a execução do programa. Deve atualizar a flag terminate e,
 * em seguida, aguardar a terminação dos processos filhos, escrever as
 * estatísticas finais e retornar.
 */
void end_execution(struct info_container *info, struct buffers *buffs)
{
}

/* Aguarda a terminação dos processos filhos previamente criados. Pode usar
 * a função wait_process do process.h
 */
void wait_processes(struct info_container *info)
{
}

/* Imprime o saldo atual de uma carteira identificada pelo id que ainda está
 * no stdin a espera de ser lido com o scanf dentro da função
 */
void print_balance(struct info_container *info)
{
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
}

/* Tenta ler o recibo da transação (identificada por id, o qual ainda está no
 * stdin a espera de ser lido dentro da função com o scanf) do buffer de memória
 * partilhada entre os servidores e a main, comprovando a conclusão da transação.
 */
void receive_receipt(struct info_container *info, struct buffers *buffs)
{
}

/* Imprime as estatísticas atuais do sistema, incluindo as configurações iniciais
 * do sistema, o valor das variáveis terminate e contador da transações criadas,
 * os pids dos processos e as restantes informações (e.g., número de transações
 * assinadas pela entidade e saldo atual no caso das carteiras).
 */
void print_stat(int tx_counter, struct info_container *info)
{
}

/* Exibe informações sobre os comandos disponíveis na aplicação.
 */
void help()
{
	printf("id - Obtém o saldo atual (balance) da carteira (wallet) cujo identificador é id.\n");
	printf("trx src_id dest_id amount - O utilizador solicita a criação de uma transação em que a carteira de origem (src_id) envia uma determinada quantidade (amount) de SOT tokens para a carteira de destino (dest_id). Como resultado, obtém o identificador da transação criada (id).\n");
	printf("rcp id - Obtém o comprovativo (receipt) da execução de uma transação específica, identificada por id. O resultado retorna a instância da transação com todas as suas propriedades preenchidas.\n");
	printf("stat - Apresenta o estado atual das variáveis do info_container (descrito no final desta secção).\n");
	printf("help - Mostra as informações de ajuda sobre as operações disponíveis.\n");
	printf("end - Termina a execução do sistema SOchain.\n");
}

/* Função principal do SOchain. Inicializa o sistema, chama as funções de alocação
 * de memória, a de criação de processos filhos, a de interação do utilizador
 * e aguarda o encerramento dos processos para chamar as funções para libertar
 * a memória alocada.
 */
int main(int argc, char *argv[])
{
	printf("Hello World!\n");
	struct info_container info;
	main_args(argc, argv, &info);
	help();
	return 0;
}