#include "shell.h"

/**
 * execute_command - parses a line and runs the command
 * @input: whole line typed by the user
 * @envp: environment
 * @prog_name: program name (argv[0] from main)
 * @line_num: line number counter
 */
void execute_command(char *input, char *const envp[], char *prog_name, int line_num)
{
    char *argv[MAX_ARGS];
    int argc = 0;
    char *tok, *path = NULL;
    pid_t pid;
    int status;

    /* tokenize by spaces/tabs */
    tok = strtok(input, " \t");
    while (tok && argc < MAX_ARGS - 1)
    {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t");
    }
    argv[argc] = NULL;

    if (argc == 0)
        return;

    /* built-in: exit */
    if (strcmp(argv[0], "exit") == 0)
        exit(0);

    /* find executable: absolute/relative or via PATH */
    if (strchr(argv[0], '/'))
    {
        if (access(argv[0], X_OK) == 0)
            path = strdup(argv[0]);
    }
    else
        path = resolve_path(argv[0], (char *const *)envp);

    if (!path)
    {
        /* Requirement: do NOT fork when command doesn't exist */
        dprintf(STDERR_FILENO, "%s: %d: %s: not found\n", prog_name, line_num, argv[0]);
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(path);
        return;
    }
    if (pid == 0)
    {
        execve(path, argv, (char *const *)envp);
        perror("execve"); /* only if execve fails */
        _exit(127);
    }
    else
    {
        waitpid(pid, &status, 0);
    }

    free(path);
}
