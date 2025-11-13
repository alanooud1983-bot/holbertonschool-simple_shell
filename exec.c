#include "shell.h"

/**
 * execute_command - parse a line and run the command
 * @input: full line from the user (will be modified)
 * @envp: environment
 */
int last_status = 0;

void execute_command(char *input, char **envp)
{
	char *argv[MAX_ARGS];
	int argc = 0;
	char *tok, *path = NULL;
	pid_t pid;
	int status;

	/* split on spaces/tabs */
	tok = strtok(input, " \t");
	while (tok && argc < MAX_ARGS - 1)
	{
		argv[argc++] = tok;
		tok = strtok(NULL, " \t");
	}
	argv[argc] = NULL;

	if (argc == 0)
		return;

	/* built-in: exit */
	if (strcmp(argv[0], "exit") == 0)
		exit(0);

	/* built-in: env */
	/* built-in: exit */
	if (strcmp(argv[0], "exit") == 0)
	{
    		exit(last_status);
	}


	/* find executable: absolute/relative or via PATH */
	if (strchr(argv[0], '/'))
		path = strdup(argv[0]);
	else
		path = resolve_path(argv[0], envp);

	if (!path)
	{
		dprintf(STDERR_FILENO, "%s: not found\n", argv[0]);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(path);
		return;
	}
	if (pid == 0)
	{
		execve(path, argv, envp);
		perror("execve"); /* only if execve fails */
		_exit(127);
	}
	else
	{
    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
        last_status = WEXITSTATUS(status);
    else
        last_status = 1; /* generic error */
	}

	free(path);
	}

