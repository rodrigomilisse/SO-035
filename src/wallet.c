#include "wallet.h"
#include "memory.h"
#include "main.h"
#include <time.h>
#include "unistd.h"
#include "server.h"

/* Função principal de uma carteira. Deve executar um ciclo infinito onde,
 * em cada iteração, lê uma transação da main apenas caso o src_id da transação seja
 * o seu próprio id. Se info->terminate ainda for 0, a carteira assina autorizando a transação,
 * encaminhando-a para os servidores. Se a transação tiver um id igual a -1, é ignorada e espera-se
 * alguns milisegundos antes de tentar ler uma nova transação do buffer. Se info->terminate for 1,
 * a execução termina e retorna o número de transações assinadas.
 */
int execute_wallet(int wallet_id, struct info_container *info, struct buffers *buffs)
{
	int *num_txs = &info->wallets_stats[wallet_id/*wallet id indexes stats?*/];
	int max_txs = info->max_txs;
	int SECONDS = 1; //change to ms!
	struct transaction *tx = allocate_dynamic_memory(sizeof(struct transaction));
	if (tx == NULL)
	{
		printf("erro: execute_wallet/allocatie_dynamic_memory()");
	}
	while(!*info->terminate && *num_txs < max_txs /*verificar?*/)
	{
		if (tx->src_id == wallet_id)
		{
			wallet_receive_transaction(tx, wallet_id, info, buffs);
			wallet_process_transaction(tx, wallet_id, info);
			wallet_send_transaction(tx, info, buffs);
		}
		sleep(SECONDS);
	}
	return *num_txs;
}

/* Função que lê uma transação do buffer de memória partilhada entre a main e as carteiras apenas
 * caso o src_id da transação seja o seu próprio id. Antes de tentar ler a transação, deve verificar
 * se info->terminate tem valor 1. Em caso afirmativo, retorna imediatamente da função.
 */
void wallet_receive_transaction(struct transaction *tx, int wallet_id, struct info_container *info, struct buffers *buffs)
{
	if(tx->src_id == wallet_id && *(info->terminate) != 1)
	{
		read_main_wallets_buffer(buffs->buff_main_wallets, wallet_id, info->buffers_size, tx);
	}
}

static void sign_transaction(struct transaction *tx, int wallet_id)
{
	tx->wallet_signature = wallet_id;
}

/* Função que assina uma transação comprovando que a carteira de origem src_id da transação corresponde
 * ao wallet_id. Atualiza o campo wallet_signature da transação e incrementa o contador de transações
 * assinadas pela carteira.
 */
void wallet_process_transaction(struct transaction *tx, int wallet_id, struct info_container *info)
{
	int *num_txs = &info->wallets_stats[wallet_id/*id de wallet para índice?*/];
	sign_transaction(tx, wallet_id);
	(*num_txs)++;
}

/* Função que escreve uma transação assinada no buffer de memória partilhada entre
 * as carteiras e os servidores. Se não houver espaço disponível no buffer, a transação
 * perde-se.
 */
void wallet_send_transaction(struct transaction *tx, struct info_container *info, struct buffers *buffs)
{
	write_wallets_servers_buffer(buffs->buff_wallets_servers, info->buffers_size, tx);
}