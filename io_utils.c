#include "shell.h"

/**
 * printStringToStandardError - Prints an input string to standard error
 * @str: The string to be printed
 *
 * Return: Nothing
 */
void printStringToStandardError(char *str)
{
	int i = 0;

	if (!str)
		return;

	while (str[i] != '\0')
	{
		printCharacterToStandardError(str[i]);
		i++;
	}
}

/**
 * printCharacterToStandardError - Writes the
 * character 'c' to standard error
 * @c: The character to print
 *
 * Return: On success 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int printCharacterToStandardError(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}

	if (c != BUF_FLUSH)
		buf[i++] = c;

	return (1);
}

/**
 * printCharacterToFileDescriptor - Writes the
 * character 'c' to the given file descriptor
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int printCharacterToFileDescriptor(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}

	if (c != BUF_FLUSH)
		buf[i++] = c;

	return (1);
}

/**
 * printStringToFileDescriptor - Prints an input
 * string to the given file descriptor
 * @str: The string to be printed
 * @fd: The file descriptor to write to
 *
 * Return: The number of characters put
 */
int printStringToFileDescriptor(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);

	while (*str)
	{
		i += printCharacterToFileDescriptor(*str++, fd);
	}

	return (i);
}
