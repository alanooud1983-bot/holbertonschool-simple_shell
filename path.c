#include "shell.h"

/**
 * strdup2 - simple strdup (allowed funcs)
 * @s: source
 * Return: new heap string or NULL
 */
char *strdup2(const char *s)
{
	size_t n, i;
	char *cpy;

	if (!s)
		return (NULL);
	n = strlen(s);
	cpy = (char *)malloc(n + 1);
	if (!cpy)
		return (NULL);
	for (i = 0; i < n; i++)
		cpy[i] = s[i];
	cpy[n] = '\0';
	return (cpy);
}

/**
 * join3 - concatenate a + b + c into new heap string
 */
char *join3(const char *a, const char *b, const char *c)
{
	size_t na = (a ? strlen(a) : 0);
	size_t nb = (b ? strlen(b) : 0);
	size_t nc = (c ? strlen(c) : 0);
	size_t i, j;
	char *out = (char *)malloc(na + nb + nc + 1);

	if (!out)
		return (NULL);

	/* copy a */
	for (i = 0; i < na; i++)
		out[i] = a[i];
	/* copy b */
	for (j = 0; j < nb; j++)
		out[i + j] = b[j];
	i += nb;
	/* copy c */
	for (j = 0; j < nc; j++)
		out[i + j] = c[j];
	out[na + nb + nc] = '\0';
	return (out);
}

/**
 * resolve_path - build an executable full path according to PATH
 * @cmd: command (may be absolute/relative)
 * @envp: environment (use environ if NULL)
 * Return: malloc'd full path if executable, else NULL
 */
char *resolve_path(const char *cmd, char **envp)
{
	char *pathvar, *paths, *tok, *full;
	char **e = envp ? envp : environ;
	size_t i;

	/* absolute or relative path supplied */
	if (!cmd || *cmd == '\0')
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (access(cmd, X_OK) == 0 ? strdup2(cmd) : NULL);

	/* fetch PATH */
	pathvar = NULL;
	for (i = 0; e && e[i]; i++)
	{
		if (e[i][0] == 'P' && e[i][1] == 'A' && e[i][2] == 'T' &&
		    e[i][3] == 'H' && e[i][4] == '=')
		{
			pathvar = e[i] + 5;
			break;
		}
	}
	if (!pathvar || pathvar[0] == '\0')
		return (NULL); /* PATH missing or empty: do not search */

	paths = strdup2(pathvar);
	if (!paths)
		return (NULL);

	/* tokenize PATH on ':' */
	tok = strtok(paths, ":");
	while (tok)
	{
		full = join3(tok, "/", cmd);
		if (full && access(full, X_OK) == 0)
		{
			free(paths);
			return (full);
		}
		if (full)
			free(full);
		tok = strtok(NULL, ":");
	}

	free(paths);
	return (NULL);
}

