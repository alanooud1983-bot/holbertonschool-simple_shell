#ifndef SHELL_H
#define SHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 100

/* expose program name set in main */
extern const char *g_progname;

/* APIs */
void execute_command(char *command, char **envp);

/* returns 1 if resolved into 'out', 0 if not */
int resolve_path(const char *cmd, char *out, size_t outsz, char **envp);

/* prints "./hsh: 1: <cmd>: not found" */
void print_not_found(const char *cmd);

#endif

