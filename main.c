#include "shell.h"

/**
 * main - simple shell loop (Task 4: 0.3)
 * @ac: arg count
 * @av: arg vector
 * @envp: environment
 * Return: 0 on success
 */
int main(int ac, char **av, char **envp)
{
	char line[MAX_COMMAND_LENGTH];

	(void)ac;
	(void)av;

	while (1)
	{
		/* interactive prompt */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, ":) ", 3);

		if (!fgets(line, sizeof(line), stdin))
			break; /* EOF (Ctrl-D) */

		/* remove trailing newline */
		{
			size_t n = strlen(line);
			if (n && line[n - 1] == '\n')
				line[n - 1] = '\0';
		}

		/* empty line -> prompt again */
		if (line[0] == '\0')
			continue;

		/* optional built-in exit */
		if (strcmp(line, "exit") == 0)
			break;

		execute_command(line, envp);
	}

	return (0);
}

