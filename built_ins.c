#include "shell.h"

/**
 * display_history - Displays the command history list, with line numbers
 * starting from 0.
 * @info: Structure containing the command history list.
 *
 * Return: Always 0
 */
int display_history(info_t *info)
{
	print_list(info->history);
	return (0);
}

/**
 * unset_alias - Unsets an alias by removing it from the alias list.
 * @info: Structure containing the alias list.
 * @str: The alias string to unset.
 *
 * Return: Always 0 on success, 1 on error.
 */
int unset_alias(info_t *info, char *str)
{
	char *equal_sign, temp;
	int ret;

	equal_sign = _strchr(str, '=');
	if (!equal_sign)
		return (1);
	temp = *equal_sign;
	*equal_sign = '\0';
	ret = delete_node_at_index(&(info->alias),
		get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
	*equal_sign = temp;
	return (ret);
}

/**
 * set_alias - Sets an alias by adding it to the alias list.
 * @info: Structure containing the alias list.
 * @str: The string alias to set.
 *
 * Return: Always 0 on success, 1 on error.
 */
int set_alias(info_t *info, char *str)
{
	char *equal_sign;

	equal_sign = _strchr(str, '=');
	if (!equal_sign)
		return (1);
	if (!*++equal_sign)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - Prints the contents of an alias.
 * @node: The alias node to print.
 *
 * Return: Always 0 on success, 1 on error.
 */
int print_alias(list_t *node)
{
	char *equal_sign = NULL, *alias_string = NULL;

	if (node)
	{
		equal_sign = _strchr(node->str, '=');
		for (alias_string = node->str; alias_string <= equal_sign; alias_string++)
			_putchar(*alias_string);
		_putchar('\'');
		_puts(equal_sign + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * manage_alias - Manages the alias functionality, mimicking the alias
 * builtin (man alias).
 * @info: Structure containing the alias list and potential arguments.
 *
 * Return: Always 0.
 */
int manage_alias(info_t *info)
{
	int i = 0;
	char *equal_sign = NULL;
	list_t *alias_node = NULL;

	if (info->argc == 1)
	{
		alias_node = info->alias;
		while (alias_node)
		{
			print_alias(alias_node);
			alias_node = alias_node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		equal_sign = _strchr(info->argv[i], '=');
		if (equal_sign)
			set_alias(info, info->argv[i]);
		else
			print_alias(node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
