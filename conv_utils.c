#include "shell.h"

/**
 * convert_string_to_integer - Converts a string to an integer.
 * @s: The string to be converted.
 *
 * Return: The converted integer if the string contains numbers, 0 otherwise.
 *         -1 on error.
 */
int convert_string_to_integer(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++; /* TODO: Investigate why this causes main to return 255. */
	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * print_error_message - Prints an error message.
 * @info: A struct containing parameter and return info.
 * @error_type: String containing the specified error type.
 */
void print_error_message(info_t *info, char *error_type)
{
	print_string(info->fname);
	print_string(": ");
	print_decimal(info->line_count, STDERR_FILENO);
	print_string(": ");
	print_string(info->argv[0]);
	print_string(": ");
	print_string(error_type);
}

/**
 * print_decimal - Prints a decimal (integer) number (base 10).
 * @input: The input number.
 * @fd: The file descriptor to write to.
 *
 * Return: The number of characters printed.
 */
int print_decimal(int input, int fd)
{
	int (*put_char_func)(char) = write_character;
	int i, count = 0;
	unsigned int absolute_value, current;

	if (fd == STDERR_FILENO)
		put_char_func = write_character;
	if (input < 0)
	{
		absolute_value = -input;
		put_char_func('-');
		count++;
	}
	else
		absolute_value = input;
	current = absolute_value;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (absolute_value / i)
		{
			put_char_func('0' + current / i);
			count++;
		}
		current %= i;
	}
	put_char_func('0' + current);
	count++;

	return (count);
}

/**
 * convert_number_to_string - Converts a number to a string.
 * @num: The number to be converted.
 * @base: The base of the conversion.
 * @flags: The argument flags.
 *
 * Return: The resulting string.
 */
char *convert_number_to_string(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';
	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do {
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * remove_comments_from_string - Replaces the first instance of '#' with '\0'.
 * @buf: The address of the string to modify.
 *
 * Return: Always 0.
 */
void remove_comments_from_string(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
	{
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
	}
}
