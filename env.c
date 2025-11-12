#include "shell.h"

void print_env(char **envp)
{
    int i = 0;
    while (envp && envp[i])
    {
        write(STDOUT_FILENO, envp[i], strlen(envp[i]));
        write(STDOUT_FILENO, "\n", 1);
        i++;
    }
}

