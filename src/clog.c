/* Grupo: 35
 * Membros: Francisco Lima: nº 61864, Márcio Caetano nº 61799
 */

#include <stdlib.h>
#include <stdarg.h>

#include <stdio.h>
#include "csettings.h"
#include "clog.h"
#include "ctime.h"

void log_format(const char *message, ...)
{
	va_list argp;
	va_start(argp, message);
	char buffer[1024];
	vsprintf(buffer, message, argp);

	FILE *file = fopen(get_settings().log_file, "a");
	if (file == NULL)
	{
		perror("Error opening log file");
		return;
	}
	char *time = get_timestamp();

	fprintf(file, "%s %s\n", time, buffer);
	free(time);
	fclose(file);
	va_end(argp);
}