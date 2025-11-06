#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/**
 * main - Simple UNIX command line interpreter
 *
 * Return: Always 0.
 */
int main(void)
{
    char command[100];
    pid_t pid;
    int status;

    while (1)
    {
        printf("#cisfun$ ");
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            printf("\n");
            break; /* Handle Ctrl+D */
        }

        /* Remove newline character */
        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0)
            continue; /* Ignore empty input */

        pid = fork();

        if (pid == -1)
        {
            perror("Error");
            exit(1);
        }
        else if (pid == 0)
        {
            char *args[2];
	    args[0] = command;
	    args[1] = NULL;

            if (execve(command, args, NULL) == -1)
                perror("./shell");
            exit(1);
        }
        else
        {
            wait(&status);
        }
    }
    
    return(0);
}
