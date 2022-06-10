#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

extern char **environ;

char *find_executable(char *exe);
char **strsplt(char *str, char *token, char new_delim);
int wrdcnt(char *str, char *token);
void free_arguments(char flag, char **args, char *line);

/**
 * struct Node - linked list node for environment variables
 * @key: a string
 * @next: A pointer to the next node of the List
 */
typedef struct Node
{
	char *key;
	struct Node *next;
} Node;

#endif
