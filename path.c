#include "shell.h"

/* return pointer inside envp or NULL */
char *get_env_from_envp(const char *name, char **envp)
{
    size_t nlen = strlen(name);
    int i;

    for (i = 0; envp && envp[i]; i++)
        if (strncmp(envp[i], name, nlen) == 0 && envp[i][nlen] == '=')
            return envp[i] + nlen + 1;
    return NULL;
}

/* join dir + "/" + cmd → malloc'd string (or NULL on failure) */
static char *join3(const char *a, const char *b, const char *c)
{
    size_t la = strlen(a), lb = strlen(b), lc = strlen(c);
    char *s = malloc(la + lb + lc + 1);
    size_t i, j;

    if (!s) return NULL;
    for (i = 0; i < la; i++) s[i] = a[i];
    for (j = 0; j < lb; j++) s[i + j] = b[j];
    i += lb;
    for (j = 0; j < lc; j++) s[i + j] = c[j];
    s[la + lb + lc] = '\0';
    return s;
}

/* Return malloc'd executable path or NULL */
char *resolve_path(const char *cmd, char **envp)
{
    struct stat st;
    char *path, *copy, *tok, *full;

    /* If cmd contains '/', treat it as a path (absolute or relative) */
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0 && stat(cmd, &st) == 0 && S_ISREG(st.st_mode))
            return strdup(cmd);
        return NULL;
    }

    /* No '/' → search PATH */
    path = get_env_from_envp("PATH", envp);
    if (!path || path[0] == '\0')  /* PATH absent or empty -> not found */
        return NULL;

    copy = strdup(path);
    if (!copy) return NULL;

    tok = strtok(copy, ":");
    while (tok)
    {
        full = join3(tok, "/", cmd);
        if (full)
        {
            if (access(full, X_OK) == 0 && stat(full, &st) == 0 && S_ISREG(st.st_mode))
            { free(copy); return full; }
            free(full);
        }
        tok = strtok(NULL, ":");
    }
    free(copy);
    return NULL;
}

