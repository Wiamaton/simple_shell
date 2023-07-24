#include "shell.h"

/**
 * split_string - Splits a given string into words based on a delimiter.
 *               Repeated delimiters are ignored.
 * @str: The input string to be split.
 * @delimiter: The delimiter string.
 *
 * Return: A pointer to an array of strings (words), or NULL on failure.
 */
char **split_string(char *str, char *delimiter)
{
	int i, j, k, m, num_words = 0;
	char **words;

	if (str == NULL || str[0] == '\0')
		return (NULL);

	if (!delimiter)
		delimiter = " ";

	for (i = 0; str[i] != '\0'; i++)
	{
		if (!check_delimiter(str[i], delimiter) &&
			(check_delimiter(str[i + 1], delimiter) || !str[i + 1]))
			num_words++;
	}

	if (num_words == 0)
		return (NULL);

	words = malloc((1 + num_words) * sizeof(char *));
	if (!words)
		return (NULL);

	for (i = 0, j = 0; j < num_words; j++)
	{
		while (check_delimiter(str[i], delimiter))
			i++;

		k = 0;
		while (!check_delimiter(str[i + k], delimiter) && str[i + k])
			k++;

		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j])
		{
			for (k = 0; k < j; k++)
				free(words[k]);
			free(words);
			return (NULL);
		}

		for (m = 0; m < k; m++)
			words[j][m] = str[i++];

		words[j][m] = '\0';
	}

	words[j] = NULL;
	return (words);
}

/**
 * split_string2 - Splits a given string into words
 *	based on a single delimiter character.
 * @str: The input string to be split.
 * @delimiter: The delimiter character.
 *
 * Return: A pointer to an array of strings (words), or NULL on failure.
 */
char **split_string2(char *str, char delimiter)
{
	int i, j, k, m, num_words = 0;
	char **words;

	if (str == NULL || str[0] == '\0')
		return (NULL);

	for (i = 0; str[i] != '\0'; i++)
	{
		if ((str[i] != delimiter && str[i + 1] == delimiter) ||
			(str[i] != delimiter && !str[i + 1]) || str[i + 1] == delimiter)
			num_words++;
	}

	if (num_words == 0)
		return (NULL);

	words = malloc((1 + num_words) * sizeof(char *));
	if (!words)
		return (NULL);

	for (i = 0, j = 0; j < num_words; j++)
	{
		while (str[i] == delimiter && str[i] != delimiter)
			i++;

		k = 0;
		while (str[i + k] != delimiter && str[i + k] && str[i + k] != delimiter)
			k++;

		words[j] = malloc((k + 1) * sizeof(char));
		if (!words[j])
		{
			for (k = 0; k < j; k++)
				free(words[k]);
			free(words);
			return (NULL);
		}

		for (m = 0; m < k; m++)
			words[j][m] = str[i++];

		words[j][m] = '\0';
	}

	words[j] = NULL;
	return (words);
}
