/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#ifndef SYNCHRONIZATION_PRIVATE_H_GUARD
#define SYNCHRONIZATION_PRIVATE_H_GUARD

#include "main.h"
#include "memory.h"
#include "process.h"
#include "synchronization.h"
#include "csignal.h"
#include "csettings.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

#define SEMS_NAME "sems"
#define MAIN_WALLET_SEM_NAME "/main_wallet"
#define WALLET_SERVER_SEM_NAME "/wallet_server"
#define SERVER_MAIN_SEM_NAME "/server_main"
#define UNREAD_SUFFIX "_unread"
#define FREE_SPACE_SUFFIX "_free_space"
#define MUTEX_SUFFIX "_mutex"
#define TERMINATE_MUTEX_NAME "/terminate_mutex"

#define TERMINATE_MUTEX_SEM_NAME "/terminate_sem_mutex"

/*
 * Imprime o estado do semáfor especificado
 */
void sem_print(sem_t *sem, char *name);

/*
 * Lê a variável terminate com as verificações de sincronização necessárias
 */
int read_terminate(struct info_container *info);

#endif // PRIVATE
