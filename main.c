#include "shell.h"

const char *g_progname;

int main(int argc, char **argv, char **envp)
{
    char command[MAX_COMMAND_LENGTH];
    ssize_t nread;

    (void)argc;
    g_progname = argv[0] ? argv[0] : "hsh";

    while (1)
    {
        write(STDOUT_FILENO, "$ ", 2);

        nread = read(STDIN_FILENO, command, sizeof(command) - 1);
        if (nread <= 0) { write(STDOUT_FILENO, "\n", 1); break; }

        command[nread - 1] = '\0';

        if (strcmp(command, "exit") == 0)
            break;

        if (command[0] != '\0')
            execute_command(command, envp);
    }
    return 0;
}

