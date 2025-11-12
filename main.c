#include "shell.h"

int tokenize(char *line, char **argv, int max_tokens)
{
    int n = 0;
    char *tok = strtok(line, " \t\r\n");

    while (tok && n < max_tokens - 1)
    {
        argv[n++] = tok;
        tok = strtok(NULL, " \t\r\n");
    }
    argv[n] = NULL;
    return n;
}

int main(int ac, char **av, char **envp)
{
    char line[MAX_COMMAND_LENGTH];
    ssize_t nread;
    char *argv[MAX_TOKENS];

    (void)ac; (void)av;

    while (1)
    {
        write(STDOUT_FILENO, "$ ", 2);

        nread = read(STDIN_FILENO, line, sizeof(line) - 1);
        if (nread <= 0) { write(STDOUT_FILENO, "\n", 1); break; }

        line[nread] = '\0';

        if (tokenize(line, argv, MAX_TOKENS) == 0)
            continue;

        if (strcmp(argv[0], "exit") == 0)
            break;

        execute_command(argv, envp);
    }
    return 0;
}

