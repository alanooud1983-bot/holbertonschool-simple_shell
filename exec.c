#include "shell.h"

/**
 * parse_args - tokenize a line into argv
 * @line: mutable input line
 * @argv: output array
 * @max_args: capacity of argv
 * Return: argc (number of tokens)
 */
int parse_args(char *line, char *argv[], size_t max_args)
{
	size_t i = 0;
	char *tok = strtok(line, " \t\r");

	while (tok && i + 1 < max_args)
	{
		argv[i++] = tok;
		tok = strtok(NULL, " \t\r");
	}
	argv[i] = NULL;
	return ((int)i);
}

/**
 * execute_command - resolves via PATH and, if found, forks then execve's
 *                  (does not fork if command is missing)
 * @line: raw command line (modified by tokenizer)
 * @envp: environment
 */
void execute_command(char *line, char *const envp[])
{
	char *argv[MAX_ARGS];
	char *full;

	if (parse_args(line, argv, MAX_ARGS) == 0)
		return; /* nothing to do */

	/* Task 4 requirement: do NOT fork if command does not exist */
	full = resolve_path(argv[0], envp);
	if (!full)
	{
		/* print the canonical error and try again */
		write(STDERR_FILENO, "Command not found\n", 18);
		return;
	}

	{
		pid_t pid = fork();
		int status;

		if (pid == -1)
		{
			perror("fork");
			free(full);
			return;
		}
		if (pid == 0)
		{
			execve(full, argv, envp);
			/* If execve returns, it failed */
			perror(argv[0]);
			free(full);
			_exit(127);
		}
		free(full);
		waitpid(pid, &status, 0);
	}
}

