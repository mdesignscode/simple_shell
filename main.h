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

/**
 * wrdcnt - counts the words in a string separated by a delimiter
 * @str: string to be counted.
 * @token: string of delimiters.
 *
 * Return: the number of words in the string.
 */
int wrdcnt(char *str, char *token)
{
	int words, i, j;
	char flag = 0;

	words = 0;
	for (i = 0; str[i]; i++)
	{
		for (j = 0; token[j]; j++)
		{
			if (str[i] == token[j] && str[i + 1] == token[j])
				flag = 1;
			if (str[i] == token[j] && str[i + 1] != token[j])
				flag = 0;
			if (!flag)
			{
				if (str[i] == token[j] && str[i + 1])
					words++;
			}
		}
	}
	words++;

	return (words);
}
/**
 * before_delim - counts the letters befor a delimiter.
 * @str: string to be counted.
 * @token: string of delimiters.
 *
 * Return: an array of the letters before each delimiter.
 */
int *before_delim(char *str, char *token)
{
	int words, *before_delim, letters, i, j, k;
	char flag;

	words = wrdcnt(str, token);

	before_delim = malloc(sizeof(int) * words);
	if (!before_delim)
		return (NULL);

	i = 0;
	for (k = 0; k < words; k++)
	{
		flag = 0;
		letters = 0;
		for (; str[i]; i++)
		{
			for (j = 0; token[j]; j++)
			{
				if (str[i] == token[j])
				{
					flag = 1;
					i++;
				}
			}
			if (flag)
				break;
			if (!flag)
				letters++;
		}
		before_delim[k] = letters;
	}

	return (before_delim);
}

/**
 * strsplt - splits a string by delimiter into
 * an array of each word of the string.
 * @str: string to split.
 * @token: string of delimiters.
 * @new_delim: string to split new array by.
 *
 * Return: a new array of strings.
 */
char **strsplt(char *str, char *token, char new_delim)
{
	char **split;
	int *array;
	int index, watch, assign, letters;
	int wc;


	wc = wrdcnt(str, token);

	split = malloc(sizeof(char *) * (wc + 1));
	if (!split)
		return (NULL);

	array = before_delim(str, token);
	assign = 0;
	for (index = 0; index < wc; index++)
	{
		letters = array[index];
		split[index] = malloc(sizeof(char) * letters + 2);
		if (!split[index])
			return (NULL);

		for (watch = 0; watch < letters; watch++)
		{
			split[index][watch] = str[assign++];
		}
		assign++;
		split[index][watch] = new_delim;
		split[index][watch + 1] = 0;
	}
	split[index] = NULL;

	free(array);

	return (split);
}

/**
 * _getenv - gets an environment variable. (without using getenv).
 * @name: name of the environment variable to get.
 *
 * Return: a pointer to the environment variable value.
 */
char *_getenv(const char *name)
{
	extern char **environ;
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
