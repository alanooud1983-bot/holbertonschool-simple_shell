#include "shell.h"

/* Fork only when we HAVE a valid absolute/relative executable path */
void execute_command(char **argv, char **envp)
{
    pid_t pid;
    int status;
    char *full;

    if (!argv || !argv[0] || argv[0][0] == '\0')
        return;

    full = resolve_path(argv[0], envp);
    if (!full)
    {
        write(STDERR_FILENO, argv[0], strlen(argv[0]));
        write(STDERR_FILENO, ": not found\n", 12);
        return; /* IMPORTANT: do NOT fork on missing command */
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(full);
        return;
    }
    if (pid == 0)
    {
        execve(full, argv, envp);
        perror(argv[0]);
        _exit(127);
    }
    free(full);
    wait(&status);
}

