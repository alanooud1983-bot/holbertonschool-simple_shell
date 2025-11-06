#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

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
 * split_arguments - Split command into arguments
 * @command: The command string
 * @args: Array to store arguments
 * Return: Number of arguments
 */
int split_arguments(char *command, char *args[])
{
    int i = 0;
    char *token = strtok(command, " ");

    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return i;
}

/**
 * main - Simple Shell 0.1
 * Return: Always 0
 */
int main(void)
{
    char command[MAX_COMMAND_LENGTH];
    char command_copy[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];
    char *line;
    pid_t pid;
    int status;
    ssize_t bytes_read;
    int interactive;
    int i;

    interactive = isatty(STDIN_FILENO);

    while (1)
    {
        if (interactive)
            printf("#cisfun$ ");
        fflush(stdout);

        bytes_read = read(STDIN_FILENO, command, MAX_COMMAND_LENGTH - 1);
        
        if (bytes_read == -1)
            break;
        else if (bytes_read == 0)
        {
            if (interactive)
                printf("\n");
            break;
        }

        command[bytes_read] = '\0';
        
        /* Remove newline */
	line = strtok(command, "\n");
	while (line != NULL)
{
    int j = 0;
    while (line[j] == ' ' || line[j] == '\t')
        j++;
    if (line[j] == '\0')
    {
        line = strtok(NULL, "\n");
        continue;
    }

    strcpy(command_copy, line);
    split_arguments(command_copy, args);

    if (file_exists(args[0]) == 0)
    {
        fprintf(stderr, "./shell: No such file or directory\n");
        line = strtok(NULL, "\n");
        continue;
    }

    pid = fork();
    if (pid == 0)
    {
        if (execve(args[0], args, NULL) == -1)
        {
            perror("./shell");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid > 0)
        wait(&status);
    else
        perror("fork");

    line = strtok(NULL, "\n");
}

	/* Skip empty lines or lines with only spaces */
        for (i = 0; command[i]; i++)
        {
            if (command[i] != ' ')
                break;
        }
        if (command[i] == '\0')
            continue;

        /* Make copy for argument splitting */
        strcpy(command_copy, command);
        
        /* Split into arguments */
        split_arguments(command_copy, args);

        /* Check if command exists */
        if (file_exists(args[0]) == 0)
        {
            fprintf(stderr, "./shell: No such file or directory\n");
            continue;
        }

        pid = fork();
        if (pid == 0)
        {
            /* Child process - execute with all arguments */
            if (execve(args[0], args, NULL) == -1)
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
