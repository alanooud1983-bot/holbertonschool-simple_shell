#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

/* split */
int  split_line(char *line, char **argv);
/* path */
char *resolve_path(const char *cmd, char **envp);
/* exec */
void execute_command(char **argv, char **envp);

#endif /* SHELL_H */

