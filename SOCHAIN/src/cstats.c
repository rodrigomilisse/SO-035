/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#include <stdio.h>
#include <unistd.h>

#include "cstats.h"
#include "main.h"
#include "csettings.h"

/*
 * Escreve as estatísticas que resumem a utilizam corrente do programa no ficheiro
 * especificado nas definições
 */
void write_stats(struct info_container *info, struct buffers *buffs)
{
	// printf("Writing stats...\n");
	FILE *fp;
	fp = fopen(get_settings().stats_file, "w");
	if (fp == NULL)
	{
		perror("Error opening file");
		return;
	}
	fprintf(fp, "Process Statistics:\n");
	fprintf(fp, "Main has PID: [%d]\n", getpid());
	fprintf(fp, "There were %d Wallets, PIDs: [", info->n_wallets);
	for (int i = 0; i < info->n_wallets; i++)
	{
		fprintf(fp, "%d", info->wallets_pids[i]);
		if (i < info->n_wallets - 1)
			fprintf(fp, ", ");
	}
	fprintf(fp, "]\n");
	fprintf(fp, "There were %d Servers, PIDs: [", info->n_servers);
	for (int i = 0; i < info->n_servers; i++)
	{
		fprintf(fp, "%d", info->servers_pids[i]);
		if (i < info->n_servers - 1)
			fprintf(fp, ", ");
	}
	fprintf(fp, "]\n");
	int trx_count = 0;
	for (int i = 0; i < info->n_wallets; i++)
	{
		trx_count += info->wallets_stats[i];
	}
	fprintf(fp, "Main received %d transaction(s)!\n", trx_count);
	for (int i = 0; i < info->n_wallets; i++)
	{
		fprintf(fp, "Wallet %d signed %d transaction(s)!\n", i, info->wallets_stats[i]);
	}
	for (int i = 0; i < info->n_servers; i++)
	{
		fprintf(fp, "Server %d processed %d transaction(s)!\n", i, info->servers_stats[i]);
	}
	int unread = 0;
	for (int i = 0; i < info->buffers_size; i++)
	{
		if (buffs->buff_servers_main->ptrs[i])
			unread++;
	}
	fprintf(fp, "Main read %d receipts.\n", trx_count - unread);
	fprintf(fp, "Final Balances [");
	for (int i = 0; i < info->n_wallets; i++)
	{
		fprintf(fp, "%.2f", info->balances[i]);
		if (i < info->n_wallets - 1)
		{
			fprintf(fp, ", ");
		}
	}
	fprintf(fp, "] SOT\n");

	// printf("Stats written to file.\n");
}