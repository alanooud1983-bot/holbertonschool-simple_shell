#include "shell.h"

extern char **environ;

static void rstrip_newline(char *s)
{
	size_t n;
	if (!s) return;
	n = strlen(s);
	if (n && s[n - 1] == '\n') s[n - 1] = '\0';
}

static int is_blank(const char *s)
{
	while (*s)
	{
		if (*s != ' ' && *s != '\t')
			return 0;
		s++;
	}
	return 1;
}

int main(void)
{
	char *line = NULL;
	size_t cap = 0;
	ssize_t nread;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);

		nread = getline(&line, &cap, stdin);
		if (nread == -1)   /* Ctrl-D */
			break;

		rstrip_newline(line);
		if (*line == '\0' || is_blank(line))
			continue;

		/* Task 4 expects one-word commands, no args */
		if (strcmp(line, "exit") == 0) /* harmless for task 4; needed in task 5 */
			break;

		execute_command(line, environ);
	}
	free(line);
	return 0;
}

