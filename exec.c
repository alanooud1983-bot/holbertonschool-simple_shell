#include "shell.h"
#include <errno.h>

/* declare environ for execve */
extern char **environ;

/**
 * execute_command - run a single command
 * @command: command name (no arguments for task 4)
 *
 * Rules: handle PATH, and do NOT fork if the command doesn't exist.
 */
void execute_command(char *command)
{
	pid_t pid;
	int status;
	char *full;
	char *argv[2];

	if (!command || *command == '\0')
		return;

	/* resolve from PATH or absolute/relative */
	full = resolve_path(command, environ);
	if (!full)
	{
		/* command not found: DO NOT fork */
		write(STDERR_FILENO, "./hsh: 1: ", 10);
		write(STDERR_FILENO, command, strlen(command));
		write(STDERR_FILENO, ": not found\n", 12);
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

		execve(full, argv, environ);
		/* only reached on error */
		perror("./hsh");
		_exit(127);
	}

	/* parent */
	wait(&status);
	free(full);
}

