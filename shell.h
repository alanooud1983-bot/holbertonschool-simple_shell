#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>     /* size_t, NULL */
#include <stdio.h>      /* perror */
#include <stdlib.h>     /* malloc, free, exit */
#include <string.h>     /* strlen, strcmp, strtok */
#include <unistd.h>     /* read, write, access, fork, execve, isatty */
#include <sys/types.h>  /* pid_t */
#include <sys/wait.h>   /* wait */

#define MAX_COMMAND_LENGTH 1024

extern char **environ;

/* core */
void execute_command(char *command);

/* PATH handling */
char *resolve_path(const char *cmd, char **envp);

/* small utils */
char *strdup2(const char *s);
char *join3(const char *a, const char *b, const char *c);

#endif /* SHELL_H */

