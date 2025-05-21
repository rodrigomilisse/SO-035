/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#include <stdio.h>
#include <stdlib.h>

#include "csettings.h"
#include "main.h"

static struct settings settings;

int init_settings(char *filename)
{
	// printf("Initializing settings... %s\n", filename);
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Error opening file");
		return 1;
	}
	char line[64];
	if (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(line, "%s", &settings.log_file);
	}
	else
	{
		perror("Error reading line");
		fclose(file);
		return 1;
	}
	if (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(line, "%s", &settings.stats_file);
	}
	else
	{
		perror("Error reading line");
		fclose(file);
		return 1;
	}
	if (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(line, "%d", &settings.period);
	}
	else
	{
		perror("Error reading line");
		fclose(file);
		return 1;
	}
	fclose(file);
	// printf("Settings initialized.\n");
	return 0;
}

int init_args(struct info_container *info, char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Error opening file");
		return 1;
	}
	char line[64];
	if (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(line, "%f", &info->init_balance);
	}
	else
	{
		perror("Error reading line");
		fclose(file);
		return 1;
	}
	if (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(line, "%d", &info->n_wallets);
	}
	else
	{
		perror("Error reading line");
		fclose(file);
		return 1;
	}
	if (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(line, "%d", &info->n_servers);
	}
	else
	{
		perror("Error reading line");
		fclose(file);
		return 1;
	}
	if (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(line, "%d", &info->buffers_size);
	}
	else
	{
		perror("Error reading line");
		fclose(file);
		return 1;
	}
	if (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(line, "%d", &info->max_txs);
	}
	else
	{
		perror("Error reading line");
		fclose(file);
		return 1;
	}
	fclose(file);
	return 0;
}

struct settings *get_settings()
{
	return &settings;
}