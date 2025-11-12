#include "shell.h"

/* find PATH=... in envp without using getenv (forbidden) */
static const char *find_path_var(char *const envp[])
{
    int i;

    if (!envp)
        return NULL;

    for (i = 0; envp[i]; i++)
    {
        if (strncmp(envp[i], "PATH=", 5) == 0)
            return envp[i] + 5;
    }
    return NULL;
}

/**
 * resolve_path - returns malloc'd absolute path to cmd if executable
 * @cmd: command name (no slash) or NULL
 * @envp: environment
 * Return: malloc'd full path (caller frees) or NULL if not found
 */
char *resolve_path(const char *cmd, char *const envp[])
{
    const char *pathvar;
    char *paths, *dir, *full;
    size_t need;

    if (!cmd || !*cmd)
        return NULL;

    /* if cmd already has a slash, check it directly */
    if (strchr(cmd, '/'))
        return (access(cmd, X_OK) == 0) ? strdup(cmd) : NULL;

    pathvar = find_path_var(envp);
    if (!pathvar || !*pathvar)
        return NULL;

    paths = strdup(pathvar);
    if (!paths)
        return NULL;

    dir = strtok(paths, ":");
    while (dir)
    {
        need = strlen(dir) + 1 + strlen(cmd) + 1; /* dir + '/' + cmd + '\0' */
        full = malloc(need);
        if (!full)
        {
            free(paths);
            return NULL;
        }

        strcpy(full, dir);
        strcat(full, "/");
        strcat(full, cmd);

        if (access(full, X_OK) == 0)
        {
            free(paths);
            return full; /* caller frees */
        }

        free(full);
        dir = strtok(NULL, ":");
    }

    free(paths);
    return NULL;
}

