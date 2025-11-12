#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

/* Prototypes */
int  parse_args(char *line, char *argv[], size_t max_args);
char *resolve_path(const char *cmd, char *const envp[]);
void execute_command(char *line, char *const envp[]);

#endif /* SHELL_H */

