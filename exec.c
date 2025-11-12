#include "shell.h"

void execute_command(char *command)
{
    pid_t pid;
    char *argv[2];
    char *path;

    argv[0] = command;
    argv[1] = NULL;

    path = resolve_path(command);

    pid = fork();
    if (pid == 0)
    {
        if (execve(path, argv, environ) == -1)
            perror("Error");
        exit(1);
    }
    else if (pid > 0)
        wait(NULL);
}

