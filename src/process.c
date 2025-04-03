#include "process.h"
#include "memory.h"
#include "main.h"
#include "wallet.h"
#include "server.h"
#include <stdlib.h>
#include <unistd.h>
//#include <sys/types.h> 
#include <sys/wait.h>

/* Função que inicia um novo processo Wallet através da função fork do SO. O novo
 * processo filho irá executar a função execute_wallet respetiva, fazendo exit do retorno.
 * O processo pai devolve nesta função o pid do processo criado.
 */
int launch_wallet(int wallet_id, struct info_container *info, struct buffers *buffs)
{
	/*pid_t?*/ int pid = fork();

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
	int pid = fork();

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
