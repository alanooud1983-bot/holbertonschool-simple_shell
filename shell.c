#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

/**
 * main - Simple Shell 0.1
 * Return: Always 0
 */
int main(void)
{
    char command[MAX_COMMAND_LENGTH];
    pid_t pid;
    int status;

    while (1)
    {
        /* Display prompt only if we're in the interactive mode */
        if (isatty(STDIN_FILENO))
            printf("#cisfun$ ");
        
        fflush(stdout);

        /* Read command */
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        /* Remove newline */
        command[strcspn(command, "\n")] = '\0';

        /* Skip empty commands */
        if (strlen(command) == 0)
            continue;

        /* Fork and execute */
        pid = fork();
        if (pid == 0)
        {
            /* Child process */
            char *args[2];
            args[0] = command;
            args[1] = NULL;
            
            if (execve(command, args, NULL) == -1)
            {
                perror("./shell");
                exit(EXIT_FAILURE);
            }
        }
        else if (pid > 0)
        {
            /* Parent process - wait for child to finish */
            wait(&status);
            
            /* Display prompt again only in interactive mode */
            if (isatty(STDIN_FILENO))
                printf("#cisfun$ ");
        }
        else
        {
            perror("fork");
        }
    }

    return (0);
}
