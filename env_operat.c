#include "shell.h"

/**
 * get_environ - Returns a copy of the string array 'environ'.
 * @info: Pointer to a structure containing potential arguments.
 *	Used to maintain constant function prototype.
 * Return: A pointer to the string array 'environ'.
 */
char **get_environ(param_t *info)
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
 * @info: Pointer to a structure containing potential arguments.
 *	Used to maintain constant function prototype.
 * @var: The name of the environment variable to be removed.
 * Return: 1 on successful deletion, 0 otherwise.
 */
int _unsetenv(param_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
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
 * _setenv - Initializes a new environment
 *	variable or modifies an existing one.
 * @info: Pointer to a structure containing potential arguments.
 *	Used to maintain constant function prototype.
 * @var: The name of the environment variable.
 * @value: The value to be assigned to the environment variable.
 * Return: Always returns 0.
 */
int _setenv(param_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = info->env;
	while (node)
	{
		p = starts_with(node->str, var);
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
