/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#include <stdlib.h>

#include <stdio.h>
#include "csettings.h"
#include "clog.h"
#include "ctime.h"

void log(char *message)
{
	FILE *file = fopen(get_settings()->log_file, "a");
	if (file == NULL)
	{
		perror("Error opening log file");
		return;
	}
	char *time = get_timestamp();
	fprintf(file, "%s %s\n", time, message);
	free(time);
	fclose(file);
}