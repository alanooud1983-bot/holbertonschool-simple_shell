#include "shell.h"

/* get value of PATH from envp (no getenv so it’s easy to test) */
static char *get_env(char **envp, const char *name)
{
	size_t nlen = strlen(name);

	if (!envp)
		return NULL;

	for (; *envp; envp++)
	{
		if (strncmp(*envp, name, nlen) == 0 && (*envp)[nlen] == '=')
			return *envp + nlen + 1; /* pointer to value */
	}
	return NULL;
}

/* Join dir + "/" + cmd into a malloc'ed string */
static char *join3(const char *a, const char *b, const char *c)
{
	size_t la = strlen(a), lb = strlen(b), lc = strlen(c);
	char *s = malloc(la + lb + lc + 1);

	if (!s) return NULL;
	memcpy(s, a, la);
	memcpy(s + la, b, lb);
	memcpy(s + la + lb, c, lc);
	s[la + lb + lc] = '\0';
	return s;
}

/*
 * resolve_path - if cmd has '/', return strdup(cmd) (no search)
 *                else search PATH for an executable.
 * Return: malloc'ed absolute path or NULL if not found.
 * NOTE: If PATH is NULL or an empty string, we MUST return NULL without forking.
 */
char *resolve_path(const char *cmd, char **envp)
{
	struct stat st;
	char *path, *copy, *tok;

	if (!cmd || *cmd == '\0')
		return NULL;

	/* Absolute/relative path given */
	if (strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && (st.st_mode & S_IXUSR))
			return strdup(cmd);
		return NULL;
	}

	/* PATH lookup */
	path = get_env(envp, "PATH");
	if (!path || *path == '\0') /* <== empty PATH: do not fork later */
		return NULL;

	copy = strdup(path);
	if (!copy)
		return NULL;

	for (tok = strtok(copy, ":"); tok; tok = strtok(NULL, ":"))
	{
		/* Empty entry means current directory "." */
		if (*tok == '\0')
			tok = ".";

		char *full = join3(tok, "/", cmd);
		if (!full)
			break;

		if (stat(full, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(copy);
			return full; /* found */
		}
		free(full);
	}

	free(copy);
	return NULL;
}

