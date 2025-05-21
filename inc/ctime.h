/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#ifndef CTIME_H_GUARD
#define CTIME_H_GUARD

#include <time.h>

struct timestamps
{
	time_t created;
	time_t signed_by_server;
	time_t signed_by_wallet;
};

char *get_timestamp();

#endif