#include "shell.h"

/**
 * clear_param - Initializes the fields of the param_t struct.
 * @info: Pointer to the param_t struct to be initialized.
 */
void clear_param(param_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_param - Initializes the param_t struct and sets its fields.
 * @info: Pointer to the param_t struct to be initialized and set.
 * @av: The argument vector to be used for initialization.
 */
void set_param(param_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = _strtok(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		replace_alias(info);
		replace_vars(info);
	}
}

/**
 * free_param - Frees the fields of the param_t struct.
 * @info: Pointer to the param_t struct whose fields need to be freed.
 * @all: If true, free all fields, otherwise only specific fields are freed.
 */
void free_param(param_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			free_list(&(info->env));
		if (info->history)
			free_list(&(info->history));
		if (info->alias)
			free_list(&(info->alias));
		ffree(info->environ);
		info->environ = NULL;
		bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUFFER_FLUSH);
	}
}
