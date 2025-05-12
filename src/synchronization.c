/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#include "synchronization.h"
#include "memory.h"
#include <semaphore.h> //sem_t
#include <fcntl.h>	   /* For O_* constants */
#include <sys/stat.h>  /* For mode constants */

#define MAIN_WALLET_SEM_NAME "main_wallet"
#define WALLET_SERVER_SEM_NAME "wallet_server"
#define SERVER_MAIN_SEM_NAME "server_main"
#define TERMINATE_MUTEX_SEM_NAME "terminate_mutex"

/* Função que cria *um* semaforo , inicializado a <value> */
sem_t *create_semaphore(char *name, unsigned v)
{
	return sem_open(name, O_CREAT | O_EXCL, 0644, v);
}

/* Função para desligar/destruir um semaforo , em funcão do seu nome e pointer */
void destroy_semaphore(char *name, sem_t *sem)
{
	sem_close(sem);
	sem_unlink(name);
}

/* Função que cria *todos* os semaforos do programa, inicializando a <v> os semaforos free_space */
struct semaphores *create_all_semaphores(unsigned v)
{
	struct semaphores *sems = allocate_dynamic_memory(
		sizeof(struct triplet_sems *) * 3 + sizeof(struct semaphores *));

	sems->main_wallet = create_semaphore(MAIN_WALLET_SEM_NAME, v);
	sems->wallet_server = create_semaphore(WALLET_SERVER_SEM_NAME, v);
	sems->server_main = create_semaphore(SERVER_MAIN_SEM_NAME, v);

	sems->terminate_mutex = create_semaphore(TERMINATE_MUTEX_SEM_NAME, v);
}

/* Imprimir o valor de *todos* os semaforos em <sems> */
void print_semaphores(struct semaphores *sems);

/* Função que destroi *todos* os semaforos na estrutura <sems> */
void destroy_semaphores(struct semaphores *sems)
{
	destroy_semaphore(sems->main_wallet, MAIN_WALLET_SEM_NAME);
	destroy_semaphore(sems->wallet_server, WALLET_SERVER_SEM_NAME);
	destroy_semaphore(sems->server_main, SERVER_MAIN_SEM_NAME);

	destroy_semaphore(sems->terminate_mutex, TERMINATE_MUTEX_SEM_NAME);
}

/* função genérica que cria 3 semaforos usados na lógica Produtor-Consumidor
1º argumento: v - valor inicial do semaforo free_space
Restantes argumentos: os 3 nomes a dar aos semáforos.
Retorna: um pointer para a estrutura que contem 3 semaforos. */
struct triplet_sems *create_triplet_sems(unsigned v,
										 char *freespace_name1, char *unread_name, char *mutex_name);

/* funcao que cria os 3 semaforos necessários para aceder ao buffer Main-Wallet */
struct triplet_sems *create_main_wallet_sems(unsigned v);

/* funcao que cria os 3 semaforos necessários para aceder ao buffer Wallet-Server */
struct triplet_sems *create_wallet_server_sems(unsigned v);

/* funcao que cria os 3 semaforos necessários para aceder ao buffer Server-Main */
struct triplet_sems *create_server_main_sems(unsigned v);