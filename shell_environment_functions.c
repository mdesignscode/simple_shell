#include "main.h"

/**
 * _getenv - gets an environment variable. (without using getenv).
 * @name: name of the environment variable to get.
 *
 * Return: a pointer to the environment variable value.
 */
char *_getenv(const char *name)
{
	unsigned int i;
	size_t l;

	l = strlen(name);

	for (i = 0; environ[i]; i++)
	{
		if (!strncmp(environ[i], name, l) && (environ[i][l] == '='))
			return (&environ[i][l + 1]);
	}

	return (NULL);
}

/**
 * linked_path - builds a linked list of the PATH directories.
 *
 * Return: the head of the new linked list.
 */
Node *linked_path(void)
{
	const char *n = "PATH";
	char *path = _getenv(n);
	char **split_path;
	int index;
	Node *list, *new_node, *head;
	int wc;

	wc = wrdcnt(path, ":");
	split_path = strsplt(path, ":", '\0');

	list = malloc(sizeof(Node));
	if (!list)
		return (NULL);
	list->key = split_path[0];
	list->next = NULL;
	head = list;

	for (index = 1; index < wc; index++)
	{
		new_node = malloc(sizeof(Node));
		if (!list)
			return (NULL);
		new_node->key = split_path[index];
		new_node->next = NULL;

		list->next = new_node;
		list = list->next;
	}

	free(split_path);
	split_path = NULL;

	return (head);
}

/**
 * find_executable - creates the path of an executable.
 * @exe: executable to search for.
 *
 * Return: a pointer to the argument to be given to execve function.
 */
char *find_executable(char *exe)
{
	Node *head = linked_path(), *current;
	char *cwd = NULL, *exe_dir = NULL, *to_exe;
	int is_ok;

	cwd = getcwd(cwd, 0);
	while (head)
	{
		chdir(head->key);
		is_ok = access(exe, F_OK);
		if (is_ok == -1)
		{
			current = head;
			head = head->next;
			free_arguments(2, NULL, current->key);
			free(current);
			current = NULL;
		}
		if (!is_ok)
		{
			exe_dir = head->next->key;
			chdir(cwd);
			free_arguments(2, NULL, cwd);
			break;
		}
	}
	if (!exe_dir)
	{
		free_arguments(2, NULL, cwd);
		head = NULL;
		return (NULL);
	}
	to_exe = malloc(sizeof(char) * (strlen(exe_dir) + strlen(exe) + 2));
	if (!to_exe)
		return (NULL);
	strcpy(to_exe, exe_dir);
	strcat(to_exe, "/");
	strcat(to_exe, exe);
	return (to_exe);
}
