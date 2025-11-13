#include "shell.h"

/**
 * get_path_value - find "PATH=" in envp without using getenv
 * @envp: environment
 * Return: pointer to PATH value (after "PATH=") or NULL
 */
static char *get_path_value(char * const envp[])
{
	int i;
	size_t len = 5; /* strlen("PATH=") */

	if (!envp)
		return (NULL);

	for (i = 0; envp[i]; i++)
	{
		if (strncmp(envp[i], "PATH=", len) == 0)
			return (envp[i] + len);
	}
	return (NULL);
}

/**
 * dup_string - simple strdup replacement using allowed functions
 * @s: string to duplicate
 * Return: malloc'd copy or NULL
 */
static char *dup_string(const char *s)
{
	size_t len;
	char *copy;

	if (!s)
		return (NULL);

	len = strlen(s);
	copy = malloc(len + 1);
	if (!copy)
		return (NULL);

	memcpy(copy, s, len + 1);
	return (copy);
}

/**
 * resolve_path - build an absolute path using PATH and check executability
 * @cmd: command name (no slash)
 * @envp: environment
 *
 * Return: malloc'd full path (caller frees) or NULL if not found
 */
char *resolve_path(const char *cmd, char * const envp[])
{
	char *path_env, *path_copy, *dir;
	size_t cmd_len, dir_len;
	char *full;
	size_t need;

	if (!cmd || !*cmd)
		return (NULL);

	/* if cmd already has a slash, caller should have handled it,
	 * but be safe anyway
	 */
	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (dup_string(cmd));
		return (NULL);
	}

	path_env = get_path_value(envp);
	if (!path_env || !*path_env)
		return (NULL);

	path_copy = dup_string(path_env);
	if (!path_copy)
		return (NULL);

	cmd_len = strlen(cmd);
	dir = strtok(path_copy, ":");

	while (dir)
	{
		dir_len = strlen(dir);
		need = dir_len + 1 + cmd_len + 1; /* dir + '/' + cmd + '\0' */

		full = malloc(need);
		if (!full)
		{
			free(path_copy);
			return (NULL);
		}

		memcpy(full, dir, dir_len);
		full[dir_len] = '/';
		memcpy(full + dir_len + 1, cmd, cmd_len);
		full[need - 1] = '\0';

		if (access(full, X_OK) == 0)
		{
			free(path_copy);
			return (full); /* caller frees */
		}

		free(full);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

