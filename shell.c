#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define BUFFER_SIZE 1024

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

    while (1)
    {
        /* Display prompt */
        if (isatty(STDIN_FILENO))
            printf("#cisfun$ ");
        fflush(stdout);

        /* Read command using read() instead of fgets() */
        bytes_read = read(STDIN_FILENO, command, MAX_COMMAND_LENGTH - 1);
        
        if (bytes_read == -1)
        {
            perror("read");
            break;
        }
        else if (bytes_read == 0)  /* EOF - Ctrl+D */
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        /* Null terminate the string */
        command[bytes_read] = '\0';

        /* Remove newline character */
        if (bytes_read > 0 && command[bytes_read - 1] == '\n')
            command[bytes_read - 1] = '\0';

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
            /* Parent process - wait for child */
            wait(&status);
        }
        else
        {
            perror("fork");
        }
    }

    return (0);
}
