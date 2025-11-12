#include "shell.h"

/**
 * execute_command - resolve and run a command
 * @command: command string (no args for task 4)
 */
void execute_command(char *command)
{
	pid_t pid;
	int status;
	char *full;
	char *argv[2];

	/* Do not fork if command cannot be resolved (task 4 rule) */
	full = resolve_path(command, environ);
	if (!full)
	{
		write(STDOUT_FILENO, command, strlen(command));
		write(STDOUT_FILENO, ": not found\n", 12);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(full);
		return;
	}
	if (pid == 0)
	{
		argv[0] = full;
		argv[1] = NULL;
		/* pass environ as required */
		if (execve(full, argv, environ) == -1)
			perror("./hsh");
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(&status);
		free(full);
	}
}

