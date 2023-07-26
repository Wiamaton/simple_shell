#include "shell.h"

/**
 * bfree - Frees a pointer and sets it to NULL.
 * @ptr: Address of the pointer to free.
 *
 * This function takes a pointer to a memory location and frees the memory
 * it points to using the standard library function `free()`. After freeing
 * the memory, the pointer is set to NULL to avoid potential dangling pointers.
 *
 * Note: The function checks if the pointer is not
 *	NULL before attempting to free it.
 *
 * Return: 1 if the memory is freed successfully,
 *	otherwise 0 (e.g., if the pointer was already NULL).
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
