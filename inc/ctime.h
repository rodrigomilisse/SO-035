/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#include <time.h>
#include

#ifndef CTIME_H_GUARD
#define CTIME_H_GUARD

struct timestamps
{
	time_t created;
	time_t signed_by_server;
	time_t signed_by_wallet;
};

char *get_timestamp()
{
	struct timeval tv;
	struct tm *tm_info;
	char buffer[64];

	get_settings(&tv, NULL);
	tm_info = localtime(&tv.tv_sec);

	strftime(buffer, sizeof(buffer), "%Y%m%d %H:%M:%S", tm_info);
	char *finaltime = malloc(80);
	if (finaltime == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	snprintf(finaltime, 80, "%s.%06ld", buffer, tv.tv_usec);
}

#endif