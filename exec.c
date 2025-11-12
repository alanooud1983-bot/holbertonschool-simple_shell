#include "shell.h"

/**
 * execute_command - resolve cmd, fork/exec if found; else print not found
 * @argv: argument vector (argv[0] is cmd)
 * @envp: environment
 */
void execute_command(char **argv, char **envp)
{
	pid_t pid;
	int status;
	char *full;

	if (!argv || !argv[0] || argv[0][0] == '\0')
		return;

	full = resolve_path(argv[0], envp);
	if (!full)
	{
		/* Do NOT fork; just print error then return */
		dprintf(STDERR_FILENO, "%s: not found\n", argv[0]);
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
		/* child */
		execve(full, argv, envp);
		/* if execve returns, it failed */
		perror(full);
		_exit(127);
	}
	/* parent */
	free(full);
	waitpid(pid, &status, 0);
	(void)status;
}

