#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_COMMAND_LENGTH 100

/* PATH resolution */
char *resolve_path(const char *cmd, char **envp);

/* Execute a single command (one word, no args) */
void execute_command(char *command, char **envp);

#endif /* SHELL_H */


