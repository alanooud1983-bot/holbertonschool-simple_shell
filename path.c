#include "shell.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * resolve_path - search PATH for an executable
 * @cmd: command name (no '/')
 * @envp: environment
 * Return: malloc'd absolute path if found, otherwise NULL
 */
char *resolve_path(const char *cmd, char **envp)
{
	char *pathvar = NULL, *copy = NULL, *dir;
	size_t len_cmd;
	int i;

	if (!cmd || strchr(cmd, '/')) /* caller will handle absolute/relative */
		return NULL;

	for (i = 0; envp && envp[i]; i++)
		if (strncmp(envp[i], "PATH=", 5) == 0) { pathvar = envp[i] + 5; break; }
	if (!pathvar)
		return NULL;

	copy = strdup(pathvar);
	if (!copy)
		return NULL;

	len_cmd = strlen(cmd);

	for (dir = strtok(copy, ":"); dir; dir = strtok(NULL, ":"))
	{
		size_t len_dir = strlen(dir);
		char *full = malloc(len_dir + 1 + len_cmd + 1);

		if (!full) { free(copy); return NULL; }

		memcpy(full, dir, len_dir);
		full[len_dir] = '/';
		memcpy(full + len_dir + 1, cmd, len_cmd + 1);

		if (access(full, X_OK) == 0) { free(copy); return full; }

		free(full);
	}
	free(copy);
	return NULL;
}
