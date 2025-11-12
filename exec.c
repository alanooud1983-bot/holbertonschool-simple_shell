#include "shell.h"

/**
 * execute_command - Executes a command if found in PATH or directly
 * @command: command to execute (one word, no args)
 *
 * Task 4: Handle PATH and do not fork if command doesn’t exist
 */
void execute_command(char *command)
{
	char full[1024];
	pid_t pid;
	int status;
	char *argv[2];

	if (!command || !*command)
		return;

	/* Find command: check PATH or direct access */
	if (!resolve_command(command, full, sizeof(full)))
	{
		fprintf(stderr, "%s: not found\n", command);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}

	if (pid == 0)
	{
		argv[0] = full;
		argv[1] = NULL;
		execve(argv[0], argv, environ);
		perror("./hsh");
		_exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);
}

