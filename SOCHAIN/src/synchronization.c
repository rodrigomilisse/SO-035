/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#include "synchronization.h"
#include "synchronization-private.h"
#include "memory.h"
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

/* Função que cria *um* semaforo , inicializado a <value> */
sem_t *create_semaphore(char *name, unsigned v)
{
	// fprintf(stderr, "[Debug] A abrir semáforo %s\n", name);
	sem_unlink(name);
	return sem_open(name, O_CREAT | O_EXCL, 0644, v);
}

/* Função para desligar/destruir um semaforo , em funcão do seu nome e pointer */
void destroy_semaphore(char *name, sem_t *sem)
{
	// fprintf(stderr, "[Debug] A fechar semáforo %s\n", name);
	sem_close(sem);
	sem_unlink(name);
}

/* Função que cria *todos* os semaforos do programa, inicializando a <v> os semaforos free_space */
struct semaphores *create_all_semaphores(unsigned v)
{
	struct semaphores *sems = create_shared_memory(SEMS_NAME, sizeof(struct semaphores));

	sems->main_wallet = create_main_wallet_sems(v);
	sems->wallet_server = create_wallet_server_sems(v);
	sems->server_main = create_server_main_sems(v);
	sems->terminate_mutex = create_semaphore(TERMINATE_MUTEX_NAME, 1);
	return sems;
}

/* Imprimir o valor de *todos* os semaforos em <sems> */
void print_semaphores(struct semaphores *sems)
{
	sem_print(sems->main_wallet->unread, MAIN_WALLET_SEM_NAME UNREAD_SUFFIX);
	sem_print(sems->main_wallet->free_space, MAIN_WALLET_SEM_NAME FREE_SPACE_SUFFIX);
	sem_print(sems->main_wallet->mutex, MAIN_WALLET_SEM_NAME MUTEX_SUFFIX);

	sem_print(sems->wallet_server->unread, WALLET_SERVER_SEM_NAME UNREAD_SUFFIX);
	sem_print(sems->wallet_server->free_space, WALLET_SERVER_SEM_NAME FREE_SPACE_SUFFIX);
	sem_print(sems->wallet_server->mutex, WALLET_SERVER_SEM_NAME MUTEX_SUFFIX);

	sem_print(sems->server_main->unread, SERVER_MAIN_SEM_NAME UNREAD_SUFFIX);
	sem_print(sems->server_main->free_space, SERVER_MAIN_SEM_NAME FREE_SPACE_SUFFIX);
	sem_print(sems->server_main->mutex, SERVER_MAIN_SEM_NAME MUTEX_SUFFIX);

	sem_print(sems->terminate_mutex, TERMINATE_MUTEX_NAME);

	printf("\n\n");
}

/* Função que destroi *todos* os semaforos na estrutura <sems> */
void destroy_semaphores(struct semaphores *sems)
{
	destroy_semaphore(MAIN_WALLET_SEM_NAME FREE_SPACE_SUFFIX, sems->main_wallet->free_space);
	destroy_semaphore(MAIN_WALLET_SEM_NAME UNREAD_SUFFIX, sems->main_wallet->unread);
	destroy_semaphore(MAIN_WALLET_SEM_NAME MUTEX_SUFFIX, sems->main_wallet->mutex);

	destroy_semaphore(WALLET_SERVER_SEM_NAME FREE_SPACE_SUFFIX, sems->wallet_server->free_space);
	destroy_semaphore(WALLET_SERVER_SEM_NAME UNREAD_SUFFIX, sems->wallet_server->unread);
	destroy_semaphore(WALLET_SERVER_SEM_NAME MUTEX_SUFFIX, sems->wallet_server->mutex);

	destroy_semaphore(SERVER_MAIN_SEM_NAME FREE_SPACE_SUFFIX, sems->server_main->free_space);
	destroy_semaphore(SERVER_MAIN_SEM_NAME UNREAD_SUFFIX, sems->server_main->unread);
	destroy_semaphore(SERVER_MAIN_SEM_NAME MUTEX_SUFFIX, sems->server_main->mutex);

	destroy_shared_memory(MAIN_WALLET_SEM_NAME MUTEX_SUFFIX SEMS_NAME, sems->main_wallet, sizeof(sems->main_wallet));
	destroy_shared_memory(WALLET_SERVER_SEM_NAME MUTEX_SUFFIX SEMS_NAME, sems->wallet_server, sizeof(sems->wallet_server));
	destroy_shared_memory(SERVER_MAIN_SEM_NAME MUTEX_SUFFIX SEMS_NAME, sems->server_main, sizeof(sems->server_main));

	destroy_semaphore(TERMINATE_MUTEX_SEM_NAME, sems->terminate_mutex);

	destroy_shared_memory(SEMS_NAME, sems, sizeof(struct semaphores));
}

/* função genérica que cria 3 semaforos usados na lógica Produtor-Consumidor
1º argumento: v - valor inicial do semaforo free_space
Restantes argumentos: os 3 nomes a dar aos semáforos.
Retorna: um pointer para a estrutura que contem 3 semaforos. */
struct triplet_sems *create_triplet_sems(
	unsigned v, char *freespace_name1, char *unread_name, char *mutex_name)
{
	char *name = allocate_dynamic_memory(strlen(mutex_name) + strlen(SEMS_NAME) + 1);
	strcat(name, mutex_name);
	strcat(name, SEMS_NAME);

	struct triplet_sems *trip_sems = create_shared_memory(name, sizeof(struct triplet_sems));
	deallocate_dynamic_memory(name);

	trip_sems->unread = create_semaphore(unread_name, 0);
	trip_sems->free_space = create_semaphore(freespace_name1, v);
	trip_sems->mutex = create_semaphore(mutex_name, 1);

	return trip_sems;
}

/* funcao que cria os 3 semaforos necessários para aceder ao buffer Main-Wallet */
struct triplet_sems *create_main_wallet_sems(unsigned v)
{
	return create_triplet_sems(v,
							   MAIN_WALLET_SEM_NAME FREE_SPACE_SUFFIX,
							   MAIN_WALLET_SEM_NAME UNREAD_SUFFIX,
							   MAIN_WALLET_SEM_NAME MUTEX_SUFFIX);
}

/* funcao que cria os 3 semaforos necessários para aceder ao buffer Wallet-Server */
struct triplet_sems *create_wallet_server_sems(unsigned v)
{
	return create_triplet_sems(v,
							   WALLET_SERVER_SEM_NAME FREE_SPACE_SUFFIX,
							   WALLET_SERVER_SEM_NAME UNREAD_SUFFIX,
							   WALLET_SERVER_SEM_NAME MUTEX_SUFFIX);
}

/* funcao que cria os 3 semaforos necessários para aceder ao buffer Server-Main */
struct triplet_sems *create_server_main_sems(unsigned v)
{
	return create_triplet_sems(v,
							   SERVER_MAIN_SEM_NAME FREE_SPACE_SUFFIX,
							   SERVER_MAIN_SEM_NAME UNREAD_SUFFIX,
							   SERVER_MAIN_SEM_NAME MUTEX_SUFFIX);
}

// PRIVATE
void sem_print(sem_t *sem, char *name)
{
	int val;
	sem_getvalue(sem, &val);
	printf("%s: %d\n", name, val);
}

int read_terminate(struct info_container *info)
{
	int terminate = 0;
	sem_wait(info->sems->terminate_mutex);
	terminate = *info->terminate;
	sem_post(info->sems->terminate_mutex);
	return terminate;
}