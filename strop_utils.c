#include "shell.h"

/**
 * str_length - Calculate the length of a string.
 * @s: The string to calculate the length for.
 *
 * Return: The length of the string as an integer.
 */
int str_length(char *s)
{
	int length = 0;

	if (!s)
		return (0);

	while (*s++)
		length++;
	return (length);
}

/**
 * str_compare - Compare two strings lexicographically.
 * @s1: The first string to compare.
 * @s2: The second string to compare.
 *
 * Return: A negative value if s1 < s2,
 *	a positive value if s1 > s2, or 0 if s1 == s2.
 */
int str_compare(char *s1, char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	if (*s1 == *s2)
		return (0);
	else
		return (*s1 < *s2 ? -1 : 1);
}

/**
 * str_starts_with - Check if a string starts with a specific substring.
 * @haystack: The string to search within.
 * @needle: The substring to find at the beginning of the haystack.
 *
 * Return: A pointer to the next character in haystack after the needle,
 *	or NULL if not found.
 */
char *str_starts_with(const char *haystack, const char *needle)
{
	while (*needle)
		if (*needle++ != *haystack++)
			return (NULL);
	return ((char *)haystack);
}

/**
 * str_concatenate - Concatenate two strings.
 * @dest: The destination buffer to hold the concatenated string.
 * @src: The source buffer to append to the destination.
 *
 * Return: A pointer to the destination buffer.
 */
char *str_concatenate(char *dest, char *src)
{
	char *result = dest;

	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = *src;
	return (result);
}
