#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

extern char **environ;

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

int split_arguments(char *command, char *args[])
{
    int i = 0;
    char *token;

    token = strtok(command, " \t");
    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " \t");
    }
    args[i] = NULL;
    return (i);
}

int main(void)
{
    char command[MAX_COMMAND_LENGTH];
    char command_copy[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];
    pid_t pid;
    int status;
    ssize_t bytes_read;
    int interactive;
    char *line;
    int j;

    interactive = isatty(STDIN_FILENO);

    while (1)
    {
        if (interactive)
            write(STDOUT_FILENO, "#cisfun$ ", 9);

        bytes_read = read(STDIN_FILENO, command, MAX_COMMAND_LENGTH - 1);
        if (bytes_read == -1)
            break;
        else if (bytes_read == 0)
        {
            if (interactive)
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        command[bytes_read] = '\0';

        line = strtok(command, "\n");
        while (line)
        {
            j = 0;
            while (line[j] == ' ' || line[j] == '\t')
                j++;
            if (line[j] == '\0')
            {
                line = strtok(NULL, "\n");
                continue;
            }

            strcpy(command_copy, line);
            split_arguments(command_copy, args);

            if (args[0] == NULL)
            {
                line = strtok(NULL, "\n");
                continue;
            }

            if (file_exists(args[0]) == 0)
            {
                fprintf(stderr, "./shell: No such file or directory\n");
                line = strtok(NULL, "\n");
                continue;
            }

            pid = fork();
            if (pid == 0)
            {
                execve(args[0], args, environ);
                perror("./shell");
                exit(EXIT_FAILURE);
            }
            else if (pid > 0)
            {
                waitpid(pid, &status, 0);
            }
            else
            {
                perror("fork");
            }

            line = strtok(NULL, "\n");
        }
    }
    
    return (0);
}
