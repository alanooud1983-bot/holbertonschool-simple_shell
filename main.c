#include "shell.h"

/**
 * main - entry point of the simple shell
 * @ac: argument count (unused)
 * @av: argument vector (unused)
 * @envp: environment
 *
 * Return: 0 on success
 */
int main(int ac, char **av, char **envp)
{
	char *line = NULL;
	size_t n = 0;
	ssize_t r;

	(void)ac;
	(void)av;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);

		r = getline(&line, &n, stdin);
		if (r == -1) /* EOF (Ctrl+D) or read error */
			break;

		if (r > 0 && line[r - 1] == '\n')
			line[r - 1] = '\0';

		if (line[0] == '\0')
			continue;

		execute_command(line, envp);
	}

	free(line);
	return (0);
}

