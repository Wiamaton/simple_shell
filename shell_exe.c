#include "shell.h"

/**
 * shell - The main shell loop.
 * @info: Pointer to the parameter and return info struct.
 * @av: The argument vector from main().
 *
 * This function runs the main loop of the shell.
 *
 * Return: 0 on success, 1 on error, or an error code.
 */
int shell(param_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_param(info);
		if (is_interactive(info))
			_puts("$ ");
		_eputchar(BUFFER_FLUSH);
		r = get_input(info);
		if (r != -1)
		{
			set_param(info, av);
			builtin_ret = find_builtin(info);
			if (builtin_ret == -1)
				find_cmd(info);
		}
		else if (is_interactive(info))
			_putchar('\n');
		free_param(info, 0);
	}
	write_history(info);
	free_param(info, 1);
	if (!is_interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - Find a builtin command.
 * @info: Pointer to the parameter & return info struct.
 *
 * This function finds and executes a builtin command if present.
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         2 if builtin signals exit().
 */
int find_builtin(param_t *info)
{
	int i, built_in_ret = -1;
	builtin_t builtintbl[] = {
		{"exit", _myexit},
		{"env", _env},
		{"help", _help},
		{"history", _history},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", change_dir},
		{"alias", _alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - Find a command in PATH.
 * @info: Pointer to the parameter & return info struct.
 *
 * This function attempts to find a command in the PATH environment variable.
 *
 * Return: void.
 */
void find_cmd(param_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_cmd(info);
	}
	else
	{
		if ((is_interactive(info) || _getenv(info, "PATH=")
					|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
			fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			_perror(info, "not found\n");
		}
	}
}

/**
 * fork_cmd - Fork a child process to execute a command.
 * @info: Pointer to the parameter & return info struct.
 *
 * This function forks a child process to execute the command.
 *
 * Return: void.
 */
void fork_cmd(param_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			free_param(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				_perror(info, "Permission denied\n");
		}
	}
}
