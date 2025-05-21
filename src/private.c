/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#include "private.h"
#include "main.h"
#include "memory.h"
#include "process.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "synchronization.h"
#include "csignal.h"
#include "csettings.h"

void sem_print(sem_t *sem, char *name)
{
	int val;
	sem_getvalue(sem, &val);
	printf("%s: %d\n", name, val);
}

const struct timespec ts = {.tv_sec = 0, .tv_nsec = ALGUNS_MILISSEGUNDOS};
