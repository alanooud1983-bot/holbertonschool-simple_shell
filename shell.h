#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>

#define MAX_ARGS 64

/* core */
void execute_command(char *input, char *const envp[]);
char *resolve_path(const char *cmd, char *const envp[]);

/* built-ins (task 6) */
void print_env(char **envp);

#endif /* SHELL_H */

