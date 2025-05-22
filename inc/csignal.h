/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#ifndef CSIGNAL_H_GUARD
#define CSIGNAL_H_GUARD

#define _POSIX_C_SOURCE 200809L // TODO necessario?
#include <signal.h>
#include "csettings.h"
#include <stdbool.h>

extern bool TERMINATE;

extern bool ALARM;

void interrupt_handler(int);

void alarm_handler(int);

void reset_alarm();

void print_alarm_stats(struct buffers *buffs);

void init_signal_handlers();

#endif