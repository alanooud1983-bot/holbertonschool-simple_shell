#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

/**
 * _getline - Simple getline implementation
 * @line: Buffer to store the line
 * @n: Size of buffer
 * @stream: File stream
 * Return: Number of characters read
 */
ssize_t _getline(char *line, size_t n, FILE *stream)
{
    int c;
    size_t i = 0;

    while (i < n - 1 && (c = getc(stream)) != EOF)
    {
        line[i++] = c;
        if (c == '\n')
            break;
    }
    line[i] = '\0';
    return (i > 0 ? (ssize_t)i : -1);
}

/**
 * main - Simple Shell 0.1
 * Return: Always 0
 */
int main(void)
{
    char line[BUFFER_SIZE];
    char *args[2];
    pid_t pid;
    int status;
    ssize_t read;
    int interactive = 1;

    while (1)
    {
        if (interactive)
            printf("#cisfun$ ");
        fflush(stdout);

        read = _getline(line, BUFFER_SIZE, stdin);
        
        if (read == -1)
            break;

        /* Remove newline */
        if (read > 0 && line[read - 1] == '\n')
            line[read - 1] = '\0';

        /* Skip empty lines */
        if (strlen(line) == 0)
            continue;

        args[0] = line;
        args[1] = NULL;

        pid = fork();
        if (pid == 0)
        {
            if (execve(line, args, NULL) == -1)
            {
                fprintf(stderr, "./shell: No such file or directory\n");
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
