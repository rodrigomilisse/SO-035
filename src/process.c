/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#define _POSIX_C_SOURCE 200809L
#include "process.h"
#include "memory.h"
#include "main.h"
#include "wallet.h"
#include "server.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

/* Função que inicia um novo processo Wallet através da função fork do SO. O novo
 * processo filho irá executar a função execute_wallet respetiva, fazendo exit do retorno.
 * O processo pai devolve nesta função o pid do processo criado.
 */
int launch_wallet(int wallet_id, struct info_container *info, struct buffers *buffs)
{
	// Apenas a Main deve tratar sinais, todos os outros processos devem ignorar
	sigaction(SIGINT, &(struct sigaction){.sa_handler = SIG_IGN}, NULL);

	pid_t pid = fork();

	if (pid == 0)
	{
		exit(execute_wallet(wallet_id, info, buffs));
	}
	else
	{
		return pid;
	}
}

/* Função que inicia um novo processo Server através da função fork do SO. O novo
 * processo filho irá executar a função execute_server, fazendo exit do retorno.
 * O processo pai devolve nesta função o pid do processo criado.
 */
int launch_server(int server_id, struct info_container *info, struct buffers *buffs)
{
	pid_t pid = fork();

	if (pid == 0)
	{
		exit(execute_server(server_id, info, buffs));
	}
	else
	{
		return pid;
	}
}

/* Função que espera que um processo com PID process_id termine através da função waitpid.
 * Devolve o retorno do processo, se este tiver terminado normalmente.
 */
int wait_process(int process_id)
{
	int value;
	waitpid(process_id, &value, 0);
	return value;
}
