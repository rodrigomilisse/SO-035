/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#ifndef CSIGNAL_H_GUARD
#define CSIGNAL_H_GUARD

#include <signal.h>
#include "csettings.h"
#include <stdbool.h>

bool get_terminate();

bool get_alarm();

void interrupt_handler(int);

void alarm_handler(int);

void reset_alarm();

void print_alarm_stats(struct buffers *buffs, struct info_container *info);

void init_signal_handlers();

#endif