#include <stdio.h>
#include <stdlib.h>

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
    printf("#cisfun$ ");
    fflush(stdout);
}

/**
 * read_command - Reads a command from user
 * @command: Buffer to store command
 * @size: Size of buffer
 * Return: Number of characters read
 */
ssize_t read_command(char *command, size_t size)
{
    ssize_t read_chars = getline(&command, &size, stdin);
    return (read_chars);
}
