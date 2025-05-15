/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#define _POSIX_C_SOURCE 200809L // TODO necessario?

#include "csignal.h"
#include <signal.h>

void init_signal_handler(int *terminate_ptr)
{
	TERMINATE = terminate_ptr;
	sa_SIGINT->sa_handler = &interrupt_handler;
	sigemptyset(&sa_SIGINT->sa_mask);
	sigaddset(&sa_SIGINT->sa_mask, SIGINT);
	sigaction(SIGINT, sa_SIGINT, 0);
}

void interrupt_handler(int)
{
	*TERMINATE = 1;
}