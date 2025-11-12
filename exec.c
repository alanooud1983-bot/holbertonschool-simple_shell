#include "shell.h"

void print_not_found(const char *cmd)
{
    dprintf(STDERR_FILENO, "%s: 1: %s: not found\n",
            g_progname ? g_progname : "hsh", cmd);
}

void execute_command(char *command, char **envp)
{
    pid_t pid;
    int status;
    char full[1024];
    char *argvv[2];

    /* try to resolve before forking */
    if (!resolve_path(command, full, sizeof(full), envp))
    {
        /* not found -> print exact format, no fork */
        print_not_found(command);
        return;
    }

    pid = fork();
    if (pid == -1) { perror("fork"); return; }

    if (pid == 0)
    {
        argvv[0] = command;
        argvv[1] = NULL;
        execve(full, argvv, envp);
        /* only reached if execve fails */
        perror(g_progname ? g_progname : "hsh");
        _exit(127);
    }
    else
    {
        wait(&status);
    }
}

