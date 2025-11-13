#include "shell.h"

/**
 * print_env - prints the current environment
 * @envp: environment
 */
void print_env(char **envp)
{
	int i;

	if (!envp)
		return;

	for (i = 0; envp[i]; i++)
	{
		write(STDOUT_FILENO, envp[i], strlen(envp[i]));
		write(STDOUT_FILENO, "\n", 1);
	}
}

