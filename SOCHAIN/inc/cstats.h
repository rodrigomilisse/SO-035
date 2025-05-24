/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Marcio Caetano nº 61799
 */

#ifndef CSTATS_H_GUARD
#define CSTATS_H_GUARD

#include "main.h"
#include "memory.h"

/*
 * Escreve as estatísticas que resumem a utilizam corrente do programa no ficheiro
 * especificado nas definições
 */
void write_stats(struct info_container *info, struct buffers *buffs);

#endif