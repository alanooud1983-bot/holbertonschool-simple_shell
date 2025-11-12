#include "shell.h"

/* find PATH=... inside envp; return pointer to the value or NULL */
static char *get_path_value(char **envp)
{
    int i;
    if (!envp) return NULL;
    for (i = 0; envp[i]; i++)
        if (strncmp(envp[i], "PATH=", 5) == 0)
            return envp[i] + 5;
    return NULL;
}

/* C89-friendly strtok over a heap copy of PATH */
int resolve_path(const char *cmd, char *out, size_t outsz, char **envp)
{
    char *pathval, *copy, *tok;
    size_t need;

    if (!cmd || !*cmd) return 0;

    /* absolute/relative path already given */
    if (strchr(cmd, '/'))
    {
        size_t n = strlen(cmd);
        if (n >= outsz) return 0;
        memcpy(out, cmd, n + 1);
        return access(out, X_OK) == 0;
    }

    pathval = get_path_value(envp);
    if (!pathval || *pathval == '\0')
        return 0; /* will trigger "not found" with no fork */

    copy = (char *)malloc(strlen(pathval) + 1);
    if (!copy) return 0;
    strcpy(copy, pathval);

    /* strtok_r not in C89; use strtok with a private copy */
    tok = strtok(copy, ":");
    while (tok)
    {
        need = strlen(tok) + 1 + strlen(cmd) + 1; /* dir + '/' + cmd + '\0' */
        if (need <= outsz)
        {
            /* build candidate */
            out[0] = '\0';
            strcat(out, tok);
            strcat(out, "/");
            strcat(out, cmd);

            if (access(out, X_OK) == 0)
            {
                free(copy);
                return 1;
            }
        }
        tok = strtok(NULL, ":");
    }

    free(copy);
    return 0;
}

