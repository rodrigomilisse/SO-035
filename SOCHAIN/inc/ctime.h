/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#ifndef CTIME_H_GUARD
#define CTIME_H_GUARD

#include <time.h>

struct timestamps
{
	struct timespec created;
	struct timespec signed_by_server;
	struct timespec signed_by_wallet;
};

/*
 * Retorna o tempo atual num formato string predifinido
 */
char *get_timestamp();

#define ALGUNS_MILISSEGUNDOS (long)20 * 1000000
extern const struct timespec ts;

#endif