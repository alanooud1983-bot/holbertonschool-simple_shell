#include "shell.h"

/*
 * execute_command - run a single command (no arguments).
 * Requirement for task 4:
 *   - If command cannot be resolved, print error and DO NOT fork.
 */
void execute_command(char *command, char **envp)
{
	pid_t pid;
	int status;
	char *full = NULL;

	if (!command || *command == '\0')
		return;

	/* Resolve BEFORE forking */
	full = resolve_path(command, envp);

	if (!full)
	{
		/* Not found: no fork */
		dprintf(STDERR_FILENO, "%s: not found\n", command);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(full != command ? full : NULL);
		return;
	}

	if (pid == 0)
	{
		/* child */
		char *argv[] = { command, NULL };
		execve(full, argv, envp);
		/* execve failed */
		perror("./hsh");
		_exit(EXIT_FAILURE);
	}

	/* parent */
	waitpid(pid, &status, 0);

	if (full != command)
		free(full);
}

