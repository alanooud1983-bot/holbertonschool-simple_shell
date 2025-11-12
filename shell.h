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
#define MAX_TOKENS 64

/* core */
int   tokenize(char *line, char **argv, int max_tokens);
char *get_env_from_envp(const char *name, char **envp);
char *resolve_path(const char *cmd, char **envp); /* malloc'd; NULL if not found */
void  execute_command(char **argv, char **envp);  /* does nothing if argv[0] unfound */

#endif

