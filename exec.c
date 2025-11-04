#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * execute_command - Executes a single command
 * @command: The command to execute
 */
void execute_command(char *command)
{
    pid_t pid = fork();
    int status;

    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        char *args[2];
        args[0] = command;
        args[1] = NULL;
        
        if (execve(command, args, NULL) == -1)
        {
            perror("./shell");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        wait(&status);
    }
}
