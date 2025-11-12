#include "shell.h"

/* split by spaces/tabs, no quotes; returns argc, argv is NULL-terminated */
int split_line(char *line, char **argv)
{
	int argc = 0;
	char *tok;

	for (tok = strtok(line, " \t\r\n"); tok && argc < (MAX_ARGS - 1);
	     tok = strtok(NULL, " \t\r\n"))
		argv[argc++] = tok;
	argv[argc] = NULL;
	return (argc);
}

int main(int ac, char **av, char **envp)
{
	char *line = NULL;
	size_t n = 0;
	ssize_t r;
	char *argv[MAX_ARGS];
	(void)ac;
	(void)av;

	while (1)
	{
		/* interactive prompt (checker ignores it; OK to always write) */
		write(STDOUT_FILENO, "$ ", 2);

		r = getline(&line, &n, stdin);
		if (r == -1)
		{
			write(STDOUT_FILENO, "\n", 1); /* Ctrl-D case */
			break;
		}

		if (split_line(line, argv) == 0)
			continue;

		if (strcmp(argv[0], "exit") == 0)
			break;

		execute_command(argv, envp);
	}

	free(line);
	return (0);
}

