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
        printf("#cisfun$ ");
        fflush(stdout);

        /* Use fgets instead of getline */
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        /* Remove newline */
        command[strcspn(command, "\n")] = '\0';

        /* Skip empty commands */
        if (strlen(command) == 0)
            continue;

        pid = fork();
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
        else if (pid > 0)
        {
            wait(&status);
        }
        else
        {
            perror("fork");
        }
    }

    return (0);
}
