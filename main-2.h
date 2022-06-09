#ifndef _MAIN_2_H_
#define _MAIN_2_H_

#include "main.h"
/**
 * free_ll - free's a singly linked list.
 * @head: head of linked list to be free'd.
 *
 * Return: nothing.
 */
void free_ll(Node **head)
{
	Node *current, *aux;

	current = *head;
	while (current)
	{
		aux = current;
		current = current->next;
		free(aux->key);
		free(aux);
	}
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

	return (head);
}

/**
 * exe_find - creates the path of an executable.
 * @exe: executable to search for.
 *
 * Return: a pointer to the argument to be given to execve function.
 */
char *exe_find(char *exe)
{
	Node *head = linked_path(), *temp, *current;
	char *cwd = NULL, *exe_dir = NULL, *to_exe;
	int is_ok;

	temp = head;

	cwd = getcwd(cwd, 0);
	while (temp)
	{
		chdir(temp->key);
		is_ok = access(exe, F_OK);
		if (is_ok == -1)
		{
			current = temp;
			temp = temp->next;
			free(current->key);
			free(current);
		}
		if (!is_ok)
		{
			exe_dir = temp->next->key;
			break;
		}
	}
	if (!exe_dir)
	{
		return (NULL);
	}

	chdir(cwd);
	to_exe = malloc(sizeof(char) * (strlen(exe_dir) + strlen(exe) + 2));
	if (!to_exe)
		return (NULL);
	strcpy(to_exe, exe_dir);
	strcat(to_exe, "/");
	strcat(to_exe, exe);

	return (to_exe);
}

#endif
