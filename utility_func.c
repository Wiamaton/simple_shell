#include "shell.h"

/**
 * check_interactive - Determine if the shell is in interactive mode
 * @info: Pointer to the info_t structure
 *
 * Return: 1 if the shell is in interactive mode, 0 otherwise
 */
int check_interactive(info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * check_delimiter - Check if a character is a delimiter
 * @c: The character to check
 * @delimiter: The delimiter string
 *
 * Return: 1 if the character is a delimiter, 0 otherwise
 */
int check_delimiter(char c, char *delimiter)
{
	while (*delimiter)
	{
		if (*delimiter++ == c)
			return (1);
	}

	return (0);
}

/**
 * check_alpha_char - Determine if a character is alphabetic
 * @c: The character to check
 *
 * Return: 1 if the character is alphabetic, 0 otherwise
 */
int check_alpha_char(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * convert_to_int - Convert a string to an integer
 * @s: The string to be converted
 *
 * Return: The converted integer if successful,
 *	0 if there are no numbers in the string
 */
int convert_to_int(char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0; s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}
