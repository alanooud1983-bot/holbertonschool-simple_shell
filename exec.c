/* exec.c - run a single command for Task 4 (PATH + no fork on missing cmd) */
#include "shell.h"

/**
 * execute_command - resolve a command and run it as a child
 * @command: command name (one word for this task)
 *
 * Behavior (Task 4):
 * - If the command cannot be found (even after searching PATH),
 *   print "<cmd>: not found" and *do not fork*.
 * - If it is found, fork/exec and wait for the child to finish.
 */
void execute_command(char *command)
{
	char full[1024];
	pid_t pid;
	int status;
	char *argv[2];

	if (!command || !*command)
		return;

	/* Do not fork if not resolvable */
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
		/* use the current environment */
		execve(argv[0], argv, environ);
		perror("./hsh");
		_exit(EXIT_FAILURE);
	}

	/* parent waits for child */
	waitpid(pid, &status, 0);
}

