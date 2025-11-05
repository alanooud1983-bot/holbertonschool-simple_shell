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
    ssize_t bytes_read;
    int interactive = 1;
    int i;

    while (1)
    {
        /* Display prompt */
        if (interactive)
            printf("#cisfun$ ");
        fflush(stdout);

        /* Read command */
        bytes_read = read(0, command, MAX_COMMAND_LENGTH - 1);
        
        /* Handle Ctrl+D */
        if (bytes_read == 0)
        {
            if (interactive)
                printf("\n");
            break;
        }
        
        if (bytes_read == -1)
        {
            perror("read");
            break;
        }

        /* Null terminate */
        command[bytes_read] = '\0';
        
        /* Remove newline */
        if (command[bytes_read - 1] == '\n')
            command[bytes_read - 1] = '\0';

        /* Skip empty commands */
        if (strlen(command) == 0)
            continue;

        /* Remove leading spaces */
        i = 0;
        while (command[i] == ' ')
            i++;
        
        if (i > 0)
            memmove(command, command + i, strlen(command) - i + 1);

        /* Remove trailing spaces */
        i = strlen(command) - 1;
        while (i >= 0 && command[i] == ' ')
            command[i--] = '\0';

        /* Skip if empty after removing spaces */
        if (strlen(command) == 0)
            continue;

        /* Fork process */
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            /* Child process */
            char *args[] = {NULL};
            
            if (execve(command, args, NULL) == -1)
            {
                fprintf(stderr, "./shell: No such file or directory\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            /* Parent process */
            wait(&status);
        }
    }

    return (0);
}
