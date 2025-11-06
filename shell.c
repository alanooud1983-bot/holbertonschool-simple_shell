#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/**
 * main - Simple UNIX command line interpreter
 * Return: Always 0
 */
int main(void)
{
    char command[1024];
    char *cmd;
    pid_t pid;
    int status;
    int only_spaces;
    int i;
    char *args[2];

    while (1)
    {
        printf("#cisfun$ ");
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            printf("\n");
            break; /* handle Ctrl+D */
        }

        command[strcspn(command, "\n")] = '\0'; /* remove newline */

        only_spaces = 1;
        for (i = 0; command[i] != '\0'; i++)
        {
            if (command[i] != ' ' && command[i] != '\t')
            {
                only_spaces = 0;
                break;
            }
        }
        if (only_spaces)
            continue;

        cmd = command;
        while (*cmd == ' ' || *cmd == '\t')
            cmd++;

        pid = fork();
        if (pid == -1)
        {
            perror("Error:");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            args[0] = cmd;
            args[1] = NULL;

            if (execve(cmd, args, NULL) == -1)
                perror("./shell");
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(&status);
        }
    }
    
    return (0);
}
