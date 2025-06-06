/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#include "csettings.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

static struct settings settings;

/**
 * Extrai as definições do programa a partir do ficheiro cujo caminho é especificado
 */
int init_settings(char *filename)
{
	// fprintf(stderr, "[Debug] Initializing settings... %s\n", filename);
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Error opening settings file");
		return 1;
	}

	if (fscanf(file, "%s %s %d", settings.log_file, settings.stats_file, &settings.period) != 3)
	{
		perror("Error reading settings");
		fclose(file);
		return 1;
	}
	fclose(file);
	// fprintf(stderr, "[Debug] Settings initialized.\n");
	return 0;
}

/*
 * Extrai os argumentos do programa a partir do ficheiro cujo caminho é especificado
 */
int init_args(struct info_container *info, char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Error opening args file");
		return 1;
	}
	if (fscanf(file, "%f %d %d %d %d", &info->init_balance, &info->n_wallets, &info->n_servers, &info->buffers_size, &info->max_txs) != 5)
	{
		perror("Error reading args");
		fclose(file);
		return 1;
	}

	fclose(file);
	return 0;
}

struct settings get_settings()
{
	return settings;
}