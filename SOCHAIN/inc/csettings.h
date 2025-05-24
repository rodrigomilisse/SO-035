/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#ifndef CSETTINGS_H_GUARD
#define CSETTINGS_H_GUARD

#include "main.h"

#define MAX_NAME_LENGTH 256

struct settings
{
	char log_file[MAX_NAME_LENGTH];
	char stats_file[MAX_NAME_LENGTH];
	int period;
};

/**
 * Extrai as definições do programa a partir do ficheiro cujo caminho é especificado
 */
int init_settings(char *filename);

/*
 * Extrai os argumentos do programa a partir do ficheiro cujo caminho é especificado
 */
int init_args(struct info_container *info, char *filename);

struct settings get_settings();

#endif