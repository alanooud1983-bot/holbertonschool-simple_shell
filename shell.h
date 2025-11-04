#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 100

/* Function prototypes */
void display_prompt(void);
ssize_t read_command(char *command, size_t size);
void execute_command(char *command);
void shell_loop(void);

#endif
