#include "shell.h"

/**
 * get_path_value - locate PATH=... entry in envp
 * @envp: environment
 *
 * Return: pointer to value after "PATH=" or NULL
 */
static char *get_path_value(char **envp)
{
	int i;

	if (!envp)
		return (NULL);

	for (i = 0; envp[i]; i++)
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
	}
	return (NULL);
}

/**
 * resolve_path - build absolute path to cmd using PATH
 * @cmd: command name (no slash) or NULL
 * @envp: environment
 *
 * Return: malloc'd full path (caller frees) or NULL if not found
 */
char *resolve_path(const char *cmd, char **envp)
{
	char *pathvar, *paths, *dir, *full;
	size_t need;

	if (!cmd || !*cmd)
		return (NULL);

	/* if cmd already has a slash, caller should not be here */
	if (strchr(cmd, '/'))
		return (NULL);

	pathvar = get_path_value(envp);
	if (!pathvar || !*pathvar)
		return (NULL);

	paths = strdup(pathvar);
	if (!paths)
		return (NULL);

	dir = strtok(paths, ":");
	while (dir)
	{
		need = strlen(dir) + 1 + strlen(cmd) + 1;
		full = malloc(need);
		if (!full)
		{
			free(paths);
			return (NULL);
		}

		strcpy(full, dir);
		strcat(full, "/");
		strcat(full, cmd);

		if (access(full, X_OK) == 0)
		{
			free(paths);
			return (full); /* caller frees */
		}

		free(full);
		dir = strtok(NULL, ":");
	}

	free(paths);
	return (NULL);
}

