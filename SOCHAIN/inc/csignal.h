/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#ifndef CSIGNAL_H_GUARD
#define CSIGNAL_H_GUARD

#include "csettings.h"
#include <signal.h>
#include <stdbool.h>

/*
 * Indica se houve um SIGINT enviado
 */
bool get_interrupt();

/*
 * Indica se houve um SIGALRM enviado
 */
bool get_alarm();

/*
 * Trata o sinal de interrupçao mudando a variável INTERRUPT para true
 */
void interrupt_handler(int);

/*
 * Trata o sinal de alarm mudando a variável de ALARM para true
 */
void alarm_handler(int);

/*
 * Repõe tanto a variável de alarm como o prórprio temporizador
 */
void reset_alarm();

/*
 * Imprime o estado da transasçao no formato: id elapsed_time
 */
void print_alarm_stats(struct info_container *info, struct buffers *buffs);

/*
 * Redireciona o tratamento de sinais para as funções definidas
 */
void init_signal_handlers();

#endif