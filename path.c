#include "shell.h"

/* get value of an env var without getenv (to match project style) */
static char *get_env(char **envp, const char *name)
{
	size_t nlen = strlen(name);

	if (!envp)
		return (NULL);

	for (; *envp; envp++)
	{
		if (strncmp(*envp, name, nlen) == 0 && (*envp)[nlen] == '=')
			return (*envp + nlen + 1);
	}
	return (NULL);
}

/* Join dir + "/" + cmd into a malloc'ed string */
static char *join3(const char *a, const char *b, const char *c)
{
	size_t la, lb, lc;
	char *s;

	la = strlen(a);
	lb = strlen(b);
	lc = strlen(c);

	s = malloc(la + lb + lc + 1);
	if (!s)
		return (NULL);

	memcpy(s, a, la);
	memcpy(s + la, b, lb);
	memcpy(s + la + lb, c, lc);
	s[la + lb + lc] = '\0';
	return (s);
}

/**
 * resolve_path - if cmd has '/', return strdup(cmd)
 *                else search PATH for an executable
 * Return: malloc'ed path or NULL if not found
 */
char *resolve_path(const char *cmd, char **envp)
{
	struct stat st;
	char *path, *copy, *tok, *full;

	if (!cmd || *cmd == '\0')
		return (NULL);

	/* check if command includes '/' */
	if (strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && (st.st_mode & S_IXUSR))
			return (strdup(cmd));
		return (NULL);
	}

	path = get_env(envp, "PATH");
	if (!path || *path == '\0')
		return (NULL);

	copy = strdup(path);
	if (!copy)
		return (NULL);

	tok = strtok(copy, ":");
	while (tok)
	{
		if (*tok == '\0')
			tok = ".";

		full = join3(tok, "/", cmd);
		if (!full)
			break;

		if (stat(full, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(copy);
			return (full);
		}
		free(full);
		tok = strtok(NULL, ":");
	}

	free(copy);
	return (NULL);
}

