#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_COMMAND_LENGTH 1024

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
 * trim_spaces - Remove leading and trailing spaces
 * @str: The string to process
 * Return: Pointer to the trimmed string
 */
char *trim_spaces(char *str)
{
    char *end;
    
    /* Remove leading spaces */
    while (*str == ' ')
        str++;
    
    /* Remove trailing spaces */
    end = str + strlen(str) - 1;
    while (end > str && *end == ' ')
        end--;
    
    *(end + 1) = '\0';
    
    return str;
}

/**
 * is_empty - Check if string contains only spaces
 * @str: The string to check
 * Return: 1 if empty, 0 otherwise
 */
int is_empty(char *str)
{
    while (*str)
    {
        if (*str != ' ')
            return (0);
        str++;
    }
    return (1);
}

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
    char *trimmed_cmd;

    interactive = isatty(STDIN_FILENO);

    while (1)
    {
        if (interactive)
            printf("#cisfun$ ");
        fflush(stdout);

        bytes_read = read(STDIN_FILENO, command, MAX_COMMAND_LENGTH - 1);
        
        if (bytes_read == -1)
        {
            break;
        }
        else if (bytes_read == 0)
        {
            if (interactive)
                printf("\n");
            break;
        }

        command[bytes_read] = '\0';
        
        /* Remove newline */
        if (bytes_read > 0 && command[bytes_read - 1] == '\n')
            command[bytes_read - 1] = '\0';

        /* Trim spaces and check if empty */
        trimmed_cmd = trim_spaces(command);
        
        if (is_empty(trimmed_cmd))
            continue;

        /* Check if file exists */
        if (file_exists(trimmed_cmd) == 0)
        {
            fprintf(stderr, "./shell: No such file or directory\n");
            continue;
        }

        /* Prepare arguments */
        args[0] = trimmed_cmd;
        args[1] = NULL;

        pid = fork();
        if (pid == 0)
        {
            /* Child process */
            if (execve(trimmed_cmd, args, NULL) == -1)
            {
                perror("./shell");
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
