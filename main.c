#include "shell.h"

/**
 * main - entry point of a very small shell
 * @ac: argc (unused)
 * @av: argv (unused)
 * @envp: environment passed from kernel
 * Return: 0 on success
 */
int main(int ac, char **av, char **envp)
{
    char *line = NULL;
    size_t n = 0;
    ssize_t r;

    (void)ac;
    (void)av;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 2);

        r = getline(&line, &n, stdin);
        if (r == -1) /* EOF (Ctrl+D) or read error */
            break;

        if (r > 0 && line[r - 1] == '\n')
            line[r - 1] = '\0'; /* strip newline */

        if (*line == '\0')
            continue; /* empty line: prompt again */
	if (strcmp(line, "exit") == 0)
	 break;

        execute_command(line, envp);
    }

    free(line);
    return 0;
}

