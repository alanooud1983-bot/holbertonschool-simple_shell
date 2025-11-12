#include "shell.h"

/**
 * resolve_command - Finds full path of a command using PATH
 * @cmd: command name
 * @out: buffer to store full path
 * @out_sz: buffer size
 *
 * Return: 1 if found and executable, 0 if not
 */
int resolve_command(const char *cmd, char *out, size_t out_sz)
{
    char *path, *copy, *dir;

    if (!cmd || !*cmd)
        return (0);

    /* Direct path */
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
        {
            snprintf(out, out_sz, "%s", cmd);
            return (1);
        }
        return (0);
    }

    /* Search PATH */
    path = getenv("PATH");
    if (!path)
        return (0);

    copy = strdup(path);
    if (!copy)
        return (0);

    for (dir = strtok(copy, ":"); dir; dir = strtok(NULL, ":"))
    {
        if (snprintf(out, out_sz, "%s/%s", dir, cmd) >= (int)out_sz)
            continue;

        if (access(out, X_OK) == 0)
        {
            free(copy);
            return (1);
        }
    }

    free(copy);
    return (0);
}

