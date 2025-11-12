#include "shell.h"

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf(":) ");

		nread = getline(&line, &len, stdin);
		if (nread == -1)
			break;

		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		if (*line)
			execute_command(line);
	}

	free(line);
	return (0);
}
