#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stddef.h>     /* for size_t */

#define MAX_COMMAND_LENGTH 100

/* so exec.c can pass env to execve() */
extern char **environ;

/* prototypes used across the project */
void execute_command(char *command);

/* Fills full with an absolute path to cmd from PATH.
 * Returns 1 if found, 0 if not.
 */
int resolve_command(const char *cmd, char *full, size_t full_size);

#endif /* SHELL_H */

