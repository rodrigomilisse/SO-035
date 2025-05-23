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
#include "memory.h"
#include "main.h"
#include "synchronization.h"
#include "synchronization-private.h"

bool TERMINATE;
bool ALARM;

bool get_terminate()
{
	return TERMINATE;
}

bool get_alarm()
{
	return ALARM;
}

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
	alarm(get_settings().period);
}

void print_alarm_stats(struct info_container *info, struct buffers *buffs)
{
	printf("\n");

	struct timespec now;
	struct timespec old;
	clock_gettime(CLOCK_REALTIME, &now);

	for (int i = 0; i < info->buffers_size; i++)
	{
		sem_wait(info->sems->main_wallet->mutex);
		if (buffs->buff_main_wallets->ptrs[i])
		{
			old = buffs->buff_main_wallets->buffer[i].change_time.created;
			time_t sec = now.tv_sec - old.tv_sec - (old.tv_nsec > now.tv_nsec);
			long nsec = now.tv_nsec - old.tv_nsec + (old.tv_nsec > now.tv_nsec) * 1000000000;
			printf("%d %ld.%03ld\n", buffs->buff_main_wallets->buffer[i].id, sec, nsec / 1000000);
		}
		sem_post(info->sems->main_wallet->mutex);
		sem_wait(info->sems->wallet_server->mutex);
		if (((buffs->buff_wallets_servers->ptrs->in >= buffs->buff_wallets_servers->ptrs->out) ? (i >= buffs->buff_wallets_servers->ptrs->out && i < buffs->buff_wallets_servers->ptrs->in) : (i >= buffs->buff_wallets_servers->ptrs->out || i < buffs->buff_wallets_servers->ptrs->in)))
		{
			old = buffs->buff_wallets_servers->buffer[i].change_time.created;
			time_t sec = now.tv_sec - old.tv_sec - (old.tv_nsec > now.tv_nsec);
			long nsec = now.tv_nsec - old.tv_nsec + (old.tv_nsec > now.tv_nsec) * 1000000000;
			printf("%d %ld.%03ld\n", buffs->buff_wallets_servers->buffer[i].id, sec, nsec / 1000000);
		}
		sem_post(info->sems->wallet_server->mutex);
		sem_wait(info->sems->server_main->mutex);
		if (buffs->buff_servers_main->ptrs[i])
		{
			old = buffs->buff_servers_main->buffer[i].change_time.created;
			time_t sec = now.tv_sec - old.tv_sec - (old.tv_nsec > now.tv_nsec);
			long nsec = now.tv_nsec - old.tv_nsec + (old.tv_nsec > now.tv_nsec) * 1000000000;
			printf("%d %ld.%03ld\n", buffs->buff_servers_main->buffer[i].id, sec, nsec / 1000000);
		}
		sem_post(info->sems->server_main->mutex);
	}
}

void init_signal_handlers()
{
	sigaction(SIGINT, &sa_SIGINT, NULL);
	sigaction(SIGALRM, &sa_SIGALARM, NULL);
}