/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#define _POSIX_C_SOURCE 200809L

#include "csignal.h"
#include <signal.h>
#include <stdbool.h>
#include "csettings.h"
#include <unistd.h>
#include <stdio.h>

bool TERMINATE;
bool ALARM;

struct sigaction sa_SIGINT = {.sa_handler = &interrupt_handler};

struct sigaction sa_SIGALARM = {.sa_handler = &alarm_handler};

void interrupt_handler(int signum)
{
	TERMINATE = true;

	(void)signum;
}

void alarm_handler(int signum)
{
	ALARM = true;

	(void)signum;
}

void reset_alarm()
{
	ALARM = 0;
	alarm(get_settings()->period);
}

void print_alarm_stats(struct buffers *buffs)
{
	printf("\nALARM\n");
}

void init_signal_handlers()
{
	sigaction(SIGINT, &sa_SIGINT, NULL);
	sigaction(SIGALRM, &sa_SIGALARM, NULL);
}