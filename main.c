#include "shell.h"

/**
 * main - tiny shell loop (tasks 4–6)
 * Return: 0
 */
int main(void)
{
	char command[MAX_COMMAND_LENGTH];
	ssize_t nread;

	while (1)
	{
		char *p;
		char *end;

		/* prompt only for interactive stdin */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, ":) ", 3);

		nread = read(STDIN_FILENO, command, MAX_COMMAND_LENGTH - 1);
		if (nread == -1)
			continue;

		/* EOF */
		if (nread == 0)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		command[nread] = '\0';

		/* trim leading / trailing spaces and newline */
		p = command;
		while (*p == ' ' || *p == '\t')
			p++;

		end = p + strlen(p);
		while (end > p &&
		       (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\n'))
			end--;
		*end = '\0';

		if (*p == '\0')
			continue;

		/* task 5: built-in exit (no args) */
		if (strcmp(p, "exit") == 0)
			break;

		execute_command(p);
	}
	return (0);
}

