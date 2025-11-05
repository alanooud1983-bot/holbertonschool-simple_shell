#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_COMMAND_LENGTH 100

/**
 * file_exists - Check if a file exists and is executable
 * @filename: The file to check
 * Return: 1 if exists and executable, 0 otherwise
 */
int file_exists(char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
    {
        if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR))
            return (1);
    }
    return (0);
}

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
        if (isatty(STDIN_FILENO))
            printf("#cisfun$ ");
        fflush(stdout);

        bytes_read = read(STDIN_FILENO, command, MAX_COMMAND_LENGTH - 1);
        
        if (bytes_read == -1)
        {
            perror("read");
            break;
        }
        else if (bytes_read == 0)
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        command[bytes_read] = '\0';
        if (bytes_read > 0 && command[bytes_read - 1] == '\n')
            command[bytes_read - 1] = '\0';

        if (strlen(command) == 0)
            continue;

        /* Check if file exists before forking */
        if (file_exists(command) == 0)
        {
            fprintf(stderr, "./shell: No such file or directory\n");
            continue;
        }

        pid = fork();
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
