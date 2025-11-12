#include "shell.h"

/**
 * main - entry point of a very small shell
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
        if (r == -1)
        {
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        if (r > 0 && line[r - 1] == '\n')
            line[r - 1] = '\0';

        if (line[0] == '\0')
            continue;

        /* task 5: exit built-in (no args) */
        if (strcmp(line, "exit") == 0)
            break;

        /* task 6: env built-in */
        if (strcmp(line, "env") == 0)
        {
            print_env(envp);
            continue;
        }

        execute_command(line, envp);
    }

    free(line);
    return 0;
}

