/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#include "clog.h"
#include <stdio.h>
#include <stdlib.h>
#include "csettings.h"

#include "ctime.h"

void log(char *message)
{
	FILE *file = fopen(get_settings()->log_file, "a");
	if (file == NULL)
	{
		perror("Error opening log file");
		return;
	}
	fprintf(file, "%s %s\n", get_timestamp(), message);
	fclose(file);
}