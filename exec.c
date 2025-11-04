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
        /* Child process */
        char *args[] = {command, NULL};
        
        if (execve(command, args, NULL) == -1)
        {
            perror("./shell");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Parent process */
        wait(&status);
    }
}
