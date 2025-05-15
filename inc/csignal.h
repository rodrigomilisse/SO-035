/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#ifndef CSIGNAL_H_GUARD
#define CSIGNAL_H_GUARD

#define _POSIX_C_SOURCE 200809L // TODO necessario?
#include <signal.h>

int *TERMINATE;

struct sigaction *sa_SIGINT;

void init_terminate(int *terminate_ptr);

void interrupt_handler(int);

#endif