#include "shell.h"

extern char **environ;   /* use the global environment */

void execute_command(char *command)
{
	pid_t pid;
	int status;
	char *argv[2] = { NULL, NULL };
	char full[1024];

	/* Absolute/relative path or resolve via PATH */
	if (command[0] == '/' || command[0] == '.')
		strncpy(full, command, sizeof(full) - 1), full[sizeof(full) - 1] = '\0';
	else if (!resolve_path(command, environ))
	{
		/* If not found, do NOT fork */
		write(STDERR_FILENO, command, strlen(command));
		write(STDERR_FILENO, ": not found\n", 12);
		return;
	}
	else
	{
		strncpy(full, resolve_path(command, environ), sizeof(full) - 1);
		full[sizeof(full) - 1] = '\0';
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
		execve(argv[0], argv, environ);
		perror("./hsh");
		_exit(EXIT_FAILURE);
	}
	wait(&status);
}

