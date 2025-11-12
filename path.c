#include "shell.h"

char *resolve_path(char *cmd)
{
    char *path_env = getenv("PATH");
    char *path_copy, *token;
    static char full_path[1024];

    if (cmd == NULL)
        return (NULL);
    if (strchr(cmd, '/'))
        return (cmd);

    path_copy = strdup(path_env);
    token = strtok(path_copy, ":");

    while (token)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", token, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return (full_path);
        }
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return (cmd);
}

