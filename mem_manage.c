#include "shell.h"

/**
 * bfree - Frees a pointer and sets the address to NULL.
 * @ptr: A pointer to the address of the memory to be freed.
 *
 * This function takes a pointer to a pointer
 *	and frees the memory it points to,
 * then sets the pointer to NULL to avoid any potential dangling references.
 *
 * Return: 1 if the memory is freed, otherwise 0.
 */
int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
