#include "shell.h"

/**
 * resolve_path - returns malloc'ed absolute path for cmd or NULL if not found
 * @cmd: command to resolve
 * @envp: environment (used for PATH)
 *
 * Return: pointer to malloc'ed full path (caller frees) or NULL
 */
char *resolve_path(const char *cmd, char *const envp[])
{
	char *path, *dup, *tok, buf[MAX_COMMAND_LENGTH];

	(void)envp; /* we’ll use getenv which is allowed */
	if (!cmd || *cmd == '\0')
		return (NULL);

	/* if cmd already has a slash, check directly */
	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}

	path = getenv("PATH");
	if (!path || *path == '\0')
		return (NULL);

	dup = strdup(path);
	if (!dup)
		return (NULL);

	for (tok = strtok(dup, ":"); tok; tok = strtok(NULL, ":"))
	{
		size_t len = strlen(tok);

		if (len + 1 + strlen(cmd) + 1 >= sizeof(buf))
			continue;

		strcpy(buf, tok);
		if (len && buf[len - 1] != '/')
			strcat(buf, "/");
		strcat(buf, cmd);

		if (access(buf, X_OK) == 0)
		{
			char *ret = strdup(buf);
			free(dup);
			return (ret);
		}
	}

	free(dup);
	return (NULL);
}

