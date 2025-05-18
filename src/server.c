/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº -----
 */

#include "server.h"
#include "main.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "synchronization.h"

/* Função principal de um servidor. Deve executar um ciclo infinito onde, em
 * cada iteração, lê uma transação do buffer de memória partilhada entre as carteiras e os servidores.
 * Se a transação tiver um id válido e info->terminate ainda for 0, o servidor valida, processa e assina
 * a transação e a encaminha para o buffer partilhado com a main. Transações com id igual a -1 são ignoradas,
 * e aguarda-se alguns milisegundos antes de tentar ler novamente do buffer. Se info->terminate for 1,
 * significa que o sistema deve encerrar, retornando o número de transações processadas.
 * Pode usar os outros métodos auxiliares definidos.
 */
int execute_server(int server_id, struct info_container *info, struct buffers *buffs)
{
	int *num_txs = &info->servers_stats[server_id];
	struct transaction tx;

	while (!*info->terminate)
	{
		// RECEIVE
		wait_sem(info->sems->wallet_server->unread);
		wait_sem(info->sems->wallet_server->mutex);

		server_receive_transaction(&tx, info, buffs);

		wait_sem(info->sems->wallet_server->mutex);
		wait_sem(info->sems->wallet_server->free_space);

		// PROCESS
		if (tx.id == -1)
		{
			continue;
		}

		server_process_transaction(&tx, server_id, info);

		// SEND
		sem_wait(info->sems->server_main->free_space);
		sem_wait(info->sems->server_main->mutex);

		server_send_transaction(&tx, info, buffs);

		sem_post(info->sems->server_main->mutex);
		sem_post(info->sems->server_main->unread);

		// PRINT
		if (tx.server_signature != -1)
		{
			printf("[Server %d] Li a transação %d do buffer e esta foi processada corretamente!\n"
				   "[Server %d] ledger <- [tx.id %d, src_id %d, dest_id %d, amount %0.2f]\n\n",
				   server_id, tx.id, server_id, tx.id, tx.src_id, tx.dest_id, tx.amount);
		}
		else
		{
			printf("[Server %d] A transação 0 falhou por alguma razão!\n\n", server_id);
		}
	}
	return *num_txs;
}

/* Função que lê uma transação do buffer de memória partilhada entre as carteiras e os servidores. Caso não
 * exista transações a serem lidas do buffer, retorna uma transação com o tx.id -1. Antes de tentar ler a
 * transação do buffer, deve verificar se info->terminate tem valor 1. Em caso afirmativo, retorna imediatamente.
 */
void server_receive_transaction(struct transaction *tx, struct info_container *info, struct buffers *buffs)
{
	if (!*info->terminate)
	{
		read_wallets_servers_buffer(buffs->buff_wallets_servers, info->buffers_size, tx);
	}
}

static inline bool verify_wallet_ids(struct transaction *tx, struct info_container *info)
{
	return tx->src_id != tx->dest_id && tx->src_id < info->n_wallets && tx->dest_id < info->n_wallets;
}
static inline bool verify_funds(struct transaction *tx, float *balances)
{
	return tx->amount <= balances[tx->src_id];
}
static inline bool verify_wallet_signature(struct transaction *tx)
{
	return tx->wallet_signature == tx->src_id;
}
static inline void sign_transaction(struct transaction *tx, int server_id)
{
	tx->server_signature = server_id;
}
static inline void transfer_funds(struct transaction *tx, float *balances)
{
	balances[tx->src_id] -= tx->amount;
	balances[tx->dest_id] += tx->amount;
}

/* Função que processa uma transação tx, verificando a validade dos identificadores das carteiras de origem e destino,
 * dos fundos da carteira de origem e a assinatura da carteira de origem. Atualiza os saldos das carteiras envolvidas,
 * adiciona a assinatura do servidor e incrementa o contador de transações processadas pelo servidor.
 */
void server_process_transaction(struct transaction *tx, int server_id, struct info_container *info)
{
	int *num_txs = &info->servers_stats[server_id];
	bool transaction_is_valid = verify_wallet_ids(tx, info);
	transaction_is_valid &= verify_funds(tx, info->balances);
	transaction_is_valid &= verify_wallet_signature(tx);
	if (transaction_is_valid)
	{
		transfer_funds(tx, info->balances);
		sign_transaction(tx, server_id);
		(*num_txs)++;
	}
}

static inline bool verify_server_signature(struct transaction *tx)
{
	return tx->server_signature >= 0;
}
/* Função que escreve uma transação correta processada no buffer de memória partilhada entre os servidores e a main.
 * Caso o servidor não tenha assinado a transação, essa não será escrita pois significa que a transação era inválida.
 * Se não houver espaço no buffer, a transação não é enviada e o recibo perde-se.
 */
void server_send_transaction(struct transaction *tx, struct info_container *info, struct buffers *buffs)
{
	bool is_valid = verify_server_signature(tx);

	if (is_valid)
	{
		write_servers_main_buffer(buffs->buff_servers_main, info->buffers_size, tx);
	}
}