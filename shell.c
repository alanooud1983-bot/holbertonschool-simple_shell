#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

/**
 * main - Simple Shell 0.1
 * Return: Always 0
 */
int main(void)
{
    char command[MAX_COMMAND_LENGTH];
    char *args[2];
    pid_t pid;
    int status;
    ssize_t bytes_read;
    int interactive;
    int i;

    interactive = isatty(STDIN_FILENO);

    while (1)
    {
        /* Display prompt */
        if (interactive)
            printf("#cisfun$ ");
        fflush(stdout);

        /* Read command */
        bytes_read = read(STDIN_FILENO, command, MAX_COMMAND_LENGTH - 1);
        
        if (bytes_read == -1)
            break;
        else if (bytes_read == 0) /* Ctrl+D */
        {
            if (interactive)
                printf("\n");
            break;
        }

        /* Null terminate and remove newline */
        command[bytes_read] = '\0';
        if (bytes_read > 0 && command[bytes_read - 1] == '\n')
            command[bytes_read - 1] = '\0';

        /* Skip empty lines or lines with only spaces */
        for (i = 0; command[i]; i++)
        {
            if (command[i] != ' ')
                break;
        }
        if (command[i] == '\0')
            continue;

        /* Remove leading spaces */
        while (command[0] == ' ')
            memmove(command, command + 1, strlen(command));

        /* Remove trailing spaces */
        i = strlen(command) - 1;
        while (i >= 0 && command[i] == ' ')
            command[i--] = '\0';

        /* Prepare arguments */
        args[0] = command;
        args[1] = NULL;

        /* Fork and execute */
        pid = fork();
        if (pid == 0)
        {
            /* Child process */
            if (execve(command, args, NULL) == -1)
            {
                fprintf(stderr, "./shell: No such file or directory\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (pid > 0)
        {
            /* Parent process */
            wait(&status);
        }
        else
        {
            perror("fork");
        }
    }

    return (0);
}
