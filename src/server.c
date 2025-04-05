#include "server.h"
#include "main.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>

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
	int alguns_milissegundos = 3;
	const struct timespec ts = {.tv_sec = 0, .tv_nsec = (long)alguns_milissegundos * 1000};

	while (!*info->terminate)
	{
		server_receive_transaction(&tx, info, buffs);
		if (tx.id != -1)
		{
			server_process_transaction(&tx, server_id, info);
			printf("[Server %d] Li a transação %d do buffer e esta foi processada corretamente!\n"
				"[Server %d] ledger <- [tx.id %d, src_id %d, dest_id %d, amount %0.2f]\n\n",
				server_id, tx.id, server_id, tx.id, tx.src_id, tx.dest_id, tx.amount);
			server_send_transaction(&tx, info, buffs);
		}
		else
		{

			nanosleep(&ts, NULL);
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

static char verify_wallet_ids(struct transaction *tx, struct info_container *info)
{
	return tx->src_id < info->n_wallets && tx->dest_id < info->n_wallets; // TODO
}
static char verify_funds(struct transaction *tx, float *balances)
{
	return tx->amount <= balances[tx->src_id];
}
static char verify_wallet_signature(struct transaction *tx)
{
	return tx->wallet_signature == tx->src_id;
}
static void sign_transaction(struct transaction *tx, int server_id)
{
	tx->server_signature = server_id;
}
static void transfer_funds(struct transaction *tx, float *balances)
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
	char transaction_is_valid = verify_wallet_ids(tx, info);
	transaction_is_valid &= verify_funds(tx, info->balances);
	transaction_is_valid &= verify_wallet_signature(tx);
	if (transaction_is_valid)
	{
		transfer_funds(tx, info->balances);
		sign_transaction(tx, server_id);
		(*num_txs)++;
	}
}

static char verify_server_signature(struct transaction *tx)
{
	return tx->server_signature >= 0;
}
/* Função que escreve uma transação correta processada no buffer de memória partilhada entre os servidores e a main.
 * Caso o servidor não tenha assinado a transação, essa não será escrita pois significa que a transação era inválida.
 * Se não houver espaço no buffer, a transação não é enviada e o recibo perde-se.
 */
void server_send_transaction(struct transaction *tx, struct info_container *info, struct buffers *buffs)
{
	char is_valid = verify_server_signature(tx);

	if (is_valid)
	{
		write_servers_main_buffer(buffs->buff_servers_main, info->buffers_size, tx);
	}
}