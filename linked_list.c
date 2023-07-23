#include "shell.h"

/**
 * list_length - Calculates the number of nodes in a linked list.
 * @head: Pointer to the first node of the list.
 *
 * Return: The size of the linked list.
 */
size_t list_length(const list_t *head)
{
	size_t size = 0;

	while (head)
	{
		head = head->next;
		size++;
	}
	return (size);
}

/**
 * list_to_strings - Converts a linked list of
 *	strings into an array of strings.
 * @head: Pointer to the first node of the list.
 *
 * Return: An array of strings containing the data from the linked list.
 *         Returns NULL if the list is empty or if memory allocation fails.
 */
char **list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t size = list_length(head), i, j;
	char **strings_array;
	char *str;

	if (!head || !size)
		return (NULL);

	strings_array = malloc(sizeof(char *) * (size + 1));
	if (!strings_array)
		return (NULL);

	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(str_length(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strings_array[j]);
			free(strings_array);
			return (NULL);
		}

		str = copy_string(str, node->str);
		strings_array[i] = str;
	}
	strings_array[i] = NULL;
	return (strings_array);
}

/**
 * print_list - Prints all elements of a linked list of list_t nodes.
 * @head: Pointer to the first node of the list.
 *
 * Return: The size of the linked list.
 */
size_t print_list(const list_t *head)
{
	size_t size = 0;

	while (head)
	{
		print_string(convert_number_to_string(head->num, 10, 0));
		write_character(':');
		write_character(' ');
		print_string(head->str ? head->str : "(nil)");
		print_string("\n");
		head = head->next;
		size++;
	}
	return (size);
}

/**
 * find_node_starts_with -
 *	Finds a node whose string starts with a given prefix.
 * @head: Pointer to the head of the linked list.
 * @prefix: The prefix to match.
 * @c: The next character after the prefix to match.
 *
 * Return: The matching node or NULL if no match is found.
 */
list_t *find_node_starts_with(list_t *head, char *prefix, char c)
{
	char *p = NULL;

	while (head)
	{
		p = str_starts_with(head->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (head);
		head = head->next;
	}
	return (NULL);
}

/**
 * get_node_index - Gets the index of a node in a linked list.
 * @head: Pointer to the head of the linked list.
 * @node: Pointer to the node to find the index of.
 *
 * Return: The index of the node in the linked list
 *	or -1 if the node is not found.
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t index = 0;

	while (head)
	{
		if (head == node)
			return (index);
		head = head->next;
		index++;
	}
	return (-1);
}
