/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#include <time.h>
#include "ctime.h"
#include "csettings.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

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
	snprintf(finaltime, 80, "%s.%03ld", buffer, tv.tv_usec / 1000);
	return finaltime;
}