#include "shell.h"

/**
 * input_buf - Reads input and buffers chained commands.
 * @info: Parameter struct containing shell information.
 * @buf: Address of the buffer to store the input.
 * @len: Address of the length variable.
 *
 * Return: Number of bytes read.
 */
ssize_t input_buf(param_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* If nothing left in the buffer, fill it. */
	{
		/* bfree((void **)info->cmd_buf); */
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);

#if GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = _getline(info, buf, &len_p);
#endif

		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* Remove trailing newline. */
				r--;
			}
			info->linecount_flag = 1;
			del_comments(*buf);
			build_history_list(info, *buf, info->histcount++);
			/* if (_strchr(*buf, ';')) Is this a command chain? */
			{
				*len = r;
				info->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * get_input - Gets a line of input without the newline character.
 * @info: Parameter struct containing shell information.
 *
 * Return: Number of bytes read.
 */
ssize_t get_input(param_t *info)
{
	static char *buf; /* The ';' command chain buffer. */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUFFER_FLUSH);
	r = input_buf(info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len) /* We have commands left in the chain buffer. */
	{
		j = i; /* Initialize new iterator to the current buf position. */
		p = buf + i; /* Get pointer for return. */

		check_chain(info, buf, &j, i, len);
		while (j < len) /* Iterate to semicolon or end. */
		{
			if (is_chain(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* Increment past nulled ';'' */
		if (i >= len) /* Reached the end of the buffer? */
		{
			i = len = 0; /* Reset position and length. */
			info->cmd_buf_type = NORMAL_CMD;
		}

		*buf_p = p; /* Pass back the pointer to the current command position. */
		return (_strlen(p)); /* Return the length of the current command. */
	}

	*buf_p = buf; /* Else not a chain, pass back the buffer from _getline(). */
	return (r); /* Return the length of the buffer from _getline(). */
}

/**
 * read_buf - Reads a buffer from a file descriptor.
 * @info: Parameter struct containing shell information.
 * @buf: Buffer to store the read data.
 * @i: Size of the buffer.
 *
 * Return: Number of bytes read.
 */
ssize_t read_buf(param_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->readfd, buf, BUF_READ_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - Gets the next line of input from STDIN.
 * @info: Parameter struct containing shell information.
 * @ptr: Address of a pointer to buffer, preallocated or NULL.
 * @length: Size of the preallocated ptr buffer if not NULL.
 *
 * Return: Number of bytes read.
 */
int _getline(param_t *info, char **ptr, size_t *length)
{
	static char buf[BUF_READ_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - Blocks the Ctrl-C signal.
 * @sig_num: The signal number.
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused)) int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUFFER_FLUSH);
}
