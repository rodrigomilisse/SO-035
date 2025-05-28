/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#define _POSIX_C_SOURCE 200809L
#include "wallet.h"
#include "memory.h"
#include "main.h"
#include "server.h"
#include "ctime.h"
#include "synchronization.h"
#include "synchronization-private.h"
#include <time.h>
#include <stdio.h>

/* Função principal de uma carteira. Deve executar um ciclo infinito onde,
 * em cada iteração, lê uma transação da main apenas caso o src_id da transação seja
 * o seu próprio id. Se info->terminate ainda for 0, a carteira assina autorizando a transação,
 * encaminhando-a para os servidores. Se a transação tiver um id igual a -1, é ignorada e espera-se
 * alguns milisegundos antes de tentar ler uma nova transação do buffer. Se info->terminate for 1,
 * a execução termina e retorna o número de transações assinadas.
 */
int execute_wallet(int wallet_id, struct info_container *info, struct buffers *buffs)
{
	int *num_txs = &info->wallets_stats[wallet_id];
	struct transaction tx;
	while (!read_terminate(info))
	{
		// RECEIVE
		sem_wait(info->sems->main_wallet->unread);
		sem_wait(info->sems->main_wallet->mutex);

		if (read_terminate(info))
		{
			break;
		}

		wallet_receive_transaction(&tx, wallet_id, info, buffs);

		sem_post(info->sems->main_wallet->mutex);
		sem_post(tx.id == -1 ? info->sems->main_wallet->unread : info->sems->main_wallet->free_space);

		// PROCESS
		if (tx.id == -1)
		{
			nanosleep(&ts, NULL);
			continue;
		}
		wallet_process_transaction(&tx, wallet_id, info);

		// SEND
		sem_wait(info->sems->wallet_server->free_space);
		sem_wait(info->sems->wallet_server->mutex);

		if (read_terminate(info))
		{
			break;
		}
		wallet_send_transaction(&tx, info, buffs);

		sem_post(info->sems->wallet_server->mutex);
		sem_post(info->sems->wallet_server->unread);

		// PRINT
		printf("[Wallet %d] Li a transação %d do buffer e a assinei!\n", wallet_id, tx.id);
		nanosleep(&ts, NULL);
	}
	return *num_txs;
}

/* Função que lê uma transação do buffer de memória partilhada entre a main e as carteiras apenas
 * caso o src_id da transação seja o seu próprio id. Antes de tentar ler a transação, deve verificar
 * se info->terminate tem valor 1. Em caso afirmativo, retorna imediatamente da função.
 */
void wallet_receive_transaction(struct transaction *tx, int wallet_id, struct info_container *info, struct buffers *buffs)
{
	if (!read_terminate(info))
	{
		read_main_wallets_buffer(buffs->buff_main_wallets, wallet_id, info->buffers_size, tx);
	}
}

static inline void sign_transaction(struct transaction *tx, int wallet_id)
{
	tx->wallet_signature = wallet_id;
}

/* Função que assina uma transação comprovando que a carteira de origem src_id da transação corresponde
 * ao wallet_id. Atualiza o campo wallet_signature da transação e incrementa o contador de transações
 * assinadas pela carteira.
 */
void wallet_process_transaction(struct transaction *tx, int wallet_id, struct info_container *info)
{
	int *num_txs = &info->wallets_stats[wallet_id];
	sign_transaction(tx, wallet_id);
	clock_gettime(CLOCK_REALTIME, &tx->change_time.signed_by_wallet);
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