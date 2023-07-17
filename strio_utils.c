#include "shell.h"

/**
 * copy_string - Copies the contents of one string to another.
 * @dest: The destination string buffer.
 * @src: The source string to be copied.
 * Return: A pointer to the destination string.
 */
char *copy_string(char *dest, char *src)
{
	int i = 0;

	if (dest == src || src == 0)
		return (dest);

	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/**
 * duplicate_string - Duplicates a given string.
 * @str: The string to be duplicated.
 * Return: A pointer to the duplicated string.
 */
char *duplicate_string(const char *str)
{
	int length = 0;
	char *ret;

	if (str == NULL)
		return (NULL);

	while (*str++)
		length++;

	ret = malloc(sizeof(char) * (length + 1));
	if (!ret)
		return (NULL);

	for (length++; length--;)
		ret[length] = *--str;

	return (ret);
}

/**
 * print_string - Prints the input string to the standard output.
 * @str: The string to be printed.
 * Return: Nothing.
 */
void print_string(char *str)
{
	int i = 0;

	if (!str)
		return;

	while (str[i] != '\0')
	{
		write_character(str[i]);
		i++;
	}
}

/**
 * write_character - Writes the character to the standard output.
 * @c: The character to be written.
 * Return: On success, 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_character(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(1, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;

	return (1);
}
