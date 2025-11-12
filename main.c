#include "shell.h"

int main(void)
{
	char command[MAX_COMMAND_LENGTH];
	ssize_t nread;

	while (1)
	{
		write(STDOUT_FILENO, ":) ", 3);

		nread = read(STDIN_FILENO, command, sizeof(command) - 1);
		if (nread <= 0) { write(STDOUT_FILENO, "\n", 1); break; }

		command[nread - 1] = '\0';
		if (strcmp(command, "exit") == 0) break;
		if (command[0] != '\0') execute_command(command);
	}
	return (0);
}

