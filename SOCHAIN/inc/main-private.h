/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#ifndef MAIN_PRIVATE_H_GUARD
#define MAIN_PRIVATE_H_GUARD

#include "main.h"
#include "memory.h"
#include <stdbool.h>

bool alarmed = false;

/*
 * Tem um comportamento semelhante ao scanf mas verifica sempre se algum sinal
 * interrompeu a execuçao, retorna true apenas quando não foi interrompido por nenhum sinal,
 * ou seja `sage`
 */
static bool safe_scanf(struct info_container *info, struct buffers *buffs, const char *format, ...);

#endif
