#include "shell.h"

/**
 * main - Entry point of the simple shell
 * @ac: argument count (unused)
 * @av: argument vector
 * @envp: environment variables
 * Return: 0 on success
 */
int main(int ac, char **av, char **envp)
{
	char *line = NULL;
	size_t n = 0;
	ssize_t r = 0;
	int line_number = 0;

	(void)ac;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);

		r = getline(&line, &n, stdin);
		if (r == -1)
			break;

		if (r > 0 && line[r - 1] == '\n')
			line[r - 1] = '\0';

		if (line[0] == '\0')
			continue;

		line_number++;
		execute_command(line, envp, av[0], line_number);
	}

	free(line);
	return (0);
}
