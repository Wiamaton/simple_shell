#include "shell.h"

/**
 * get_environ - Retrieves a copy of the environment strings array.
 * @info: Pointer to the structure containing potential arguments.
 * Return: Pointer to the string array.
 */
char **get_environ(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _unsetenv - Removes an environment variable.
 * @info: Pointer to the structure containing potential arguments.
 * @var: Pointer to the string representing the environment variable.
 * Return: 1 if the variable is deleted, 0 otherwise.
 */
int _unsetenv(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = str_starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * _setenv - Initializes a new environment variable or modifies
 *	an existing one.
 * @info: Pointer to the structure containing potential arguments.
 * @var: Pointer to the string representing the environment variable name.
 * @value: Pointer to the string representing the environment variable value.
 * Return: Always 0.
 */
int _setenv(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(str_length(var) + str_length(value) + 2);
	if (!buf)
		return (1);
	copy_string(buf, var);
	str_concatenate(buf, "=");
	str_concatenate(buf, value);
	node = info->env;
	while (node)
	{
		p = str_starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
