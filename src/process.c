#include "memory.h"
#include "main.h"
#include "process.h"

/* Função que inicia um novo processo Wallet através da função fork do SO. O novo
 * processo filho irá executar a função execute_wallet respetiva, fazendo exit do retorno.
 * O processo pai devolve nesta função o pid do processo criado.
 */
int launch_wallet(int wallet_id, struct info_container *info, struct buffers *buffs)
{
	return 0;
}

/* Função que inicia um novo processo Server através da função fork do SO. O novo
 * processo filho irá executar a função execute_server, fazendo exit do retorno.
 * O processo pai devolve nesta função o pid do processo criado.
 */
int launch_server(int server_id, struct info_container *info, struct buffers *buffs)
{
	return 0;
}

/* Função que espera que um processo com PID process_id termine através da função waitpid.
 * Devolve o retorno do processo, se este tiver terminado normalmente.
 */
int wait_process(int process_id)
{
	return 0;
}
