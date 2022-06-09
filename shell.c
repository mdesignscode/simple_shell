#include "main.h"
#include "main-2.h"

/**
 * execute - free's up shell arguments or execute shell arguments.
 * @flag: 1 to execute arguments, 0 to free arguments, 2 to free linked list.
 * @args: shell arguments.
 * @line: pointer to store the address of the buffer containing the text.
 *
 * Return: nothing.
 */
void execute(char flag, char **args, char *line)
{
	int i;

	if (!flag)
	{
		for (i = 0; args[i]; i++)
			free(args[i]);
		free(args);
	}
	else
	{
		printf("./shell: No such file or directory\n");
		free(line);
		execute(0, args, NULL);
		exit(98);
	}
}

/**
 * non_inter - executes when shell is in non-interactive mode.
 * @line: pointer to store the address of the buffer containing the text.
 * @args: shell arguments.
 * @id: child process id.
 *
 * Return: nothing.
 */
void non_inter(char *line, char **args, pid_t id)
{
	if (!isatty(STDIN_FILENO))
	{
		if (!id)
		{
			execute(1, args, line);
		}
		else
			wait(NULL);
	}
}

/**
 * displayAndRun - waits for a user to enter a command and executes it.
 * @line: pointer to store the  address  of  the  buffer containing the text.
 * @id: child process id.
 * @args: shell arguments.
 *
 * Return: nothing.
 */
void displayAndRun(char *line, pid_t id, char **args)
{
	char *to_exe;

	if (isatty(STDIN_FILENO))
	{
		if (id)
		{
			wait(NULL);
			printf("#cisfun$ ");
		}
		else
		{
			if (execve(args[0], args, NULL) == -1)
			{
				to_exe = exe_find(args[0]);
				if (!to_exe)
					execute(1, args, line);
				if (execve(to_exe, args, NULL) == -1)
					execute(1, args, line);
			}
		}
	}
}

/**
 * get_prompt - gets the command from the user.
 * @line: pointer to store the  address  of  the  buffer containing the text.
 * @n: address to size of line.
 *
 * Return: nothing.
 */
void get_prompt(char *line, size_t n)
{
	ssize_t nread;
	pid_t id;
	char **split;
	extern char **environ;
	int i;

	while ((nread = getline(&line, &n, stdin)) != -1)
	{
		if (!strncmp(line, "exit", 4))
		{
			free(line);
			exit(0);
		}

		split = strsplt(line, " \n", '\0');

		id = fork();
		if (id == -1)
			exit(98);

		if (!strncmp(line, "env", 3) && !id)
		{
			free(line);
			for (i = 0; environ[i]; i++)
				printf("%s\n", environ[i]);
			execute(0, split, NULL);
			exit(98);
		}
		displayAndRun(line, id, split);
		non_inter(line, split, id);
		execute(0, split, NULL);
	}
	free(line);
}
/**
 * main - a UNIX command line interpreter.
 * @argc: argument count.
 * @argv: argument count.
 *
 * Return: 0.
 */

int main(int argc, char **argv)
{
	char *line;
	size_t n;

	if (argc != 1)
	{
		printf("Usage: %s\n", argv[0]);
		return (1);
	}
	n = 0;
	line = NULL;

	if (!isatty(STDIN_FILENO))
		printf("#cisfun$\n");

	printf("#cisfun$ ");

	get_prompt(line, n);

	return (0);
}
