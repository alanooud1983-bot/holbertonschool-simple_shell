# Simple Shell

A simple UNIX command line interpreter written in C as part of the Holberton School curriculum.

## Features

- Displays a prompt and waits for user input
- Executes simple commands with arguments
- Handles the "end of file" condition (Ctrl+D)
- Handles command not found errors
- Removes leading and trailing spaces from commands

## Compilation

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
