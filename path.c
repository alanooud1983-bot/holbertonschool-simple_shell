#include "shell.h"

/* find PATH=... inside envp (no getenv) */
static char *get_path_from_envp(char **envp)
{
	int i;

	if (!envp)
		return (NULL);
	for (i = 0; envp[i]; i++)
		if (strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5); /* pointer to value after PATH= */
	return (NULL);
}

/**
 * resolve_path - return malloc'ed absolute path to cmd or NULL if not found
 * @cmd: command (possibly with '/')
 * @envp: environment
 */
char *resolve_path(const char *cmd, char **envp)
{
	char *pathval, *copy, *tok, *full;
	size_t need;

	if (!cmd || !*cmd)
		return (NULL);

	/* If command already contains '/', check directly */
	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}

	/* No '/', search PATH */
	pathval = get_path_from_envp(envp);
	if (!pathval || *pathval == '\0')
		return (NULL); /* empty or missing PATH -> treat as not found */

	copy = strdup(pathval);
	if (!copy)
		return (NULL);

	tok = strtok(copy, ":");
	while (tok)
	{
		/* empty token means current directory "." */
		if (*tok == '\0')
			tok = ".";

		need = strlen(tok) + 1 /* slash */ + strlen(cmd) + 1 /* NUL */;
		full = malloc(need);
		if (!full)
		{
			free(copy);
			return (NULL);
		}

		strcpy(full, tok);
		strcat(full, "/");
		strcat(full, cmd);

		if (access(full, X_OK) == 0)
		{
			free(copy);
			return (full); /* found */
		}
		free(full);
		tok = strtok(NULL, ":");
	}

	free(copy);
	return (NULL);
}

