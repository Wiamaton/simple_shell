#include "shell.h"

/**
 * is_chain - Tests if the current character in the
 *		buffer is a chain delimiter.
 * @info: Pointer to the parameter struct.
 * @buf: The character buffer.
 * @p: Address of the current position in buf.
 *
 * Return: 1 if it is a chain delimiter, 0 otherwise.
 */
int is_chain(param_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = OR_CMD;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = AND_CMD;
	}
	else if (buf[j] == ';') /* Found the end of this command */
	{
		buf[j] = 0; /* Replace semicolon with null terminator */
		info->cmd_buf_type = CHAIN_CMD;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * check_chain - Checks if we should continue chaining
 *		based on the last status.
 * @info: Pointer to the parameter struct.
 * @buf: The character buffer.
 * @p: Address of the current position in buf.
 * @i: Starting position in buf.
 * @len: Length of buf.
 *
 * Return: Void.
 */
void check_chain(param_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->cmd_buf_type == AND_CMD)
	{
		if (info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->cmd_buf_type == OR_CMD)
	{
		if (!info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * replace_alias - Replaces an alias in the tokenized string.
 * @info: Pointer to the parameter struct.
 *
 * Return: 1 if replaced, 0 otherwise.
 */
int replace_alias(param_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars - Replaces variables in the tokenized string.
 * @info: Pointer to the parameter struct.
 *
 * Return: 1 if replaced, 0 otherwise.
 */
int replace_vars(param_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!_strcmp(info->argv[i], "$?"))
		{
			replace_string(&(info->argv[i]),
					_strdup(convert_num_to_str(info->status, 10, 0)));
			continue;
		}
		if (!_strcmp(info->argv[i], "$$"))
		{
			replace_string(&(info->argv[i]),
					_strdup(convert_num_to_str(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(info->env, &info->argv[i][1], '=');
		if (node)
		{
			replace_string(&(info->argv[i]),
					_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		replace_string(&info->argv[i], _strdup(""));
	}
	return (0);
}

/**
 * replace_string - Replaces a string.
 * @old: Address of the old string.
 * @new: New string.
 *
 * Return: 1 if replaced, 0 otherwise.
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
