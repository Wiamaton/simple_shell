#include "shell.h"

/**
 * hsh - main shell loop
 *
 * The main loop that interacts with the user and executes commands.
 *
 * @info: The parameter & return info struct.
 * @av: The argument vector from main().
 *
 * Return: 0 on success, 1 on error, or error code.
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		initialize_info(info);
		if (check_interactive(info))
			print_string("$ ");
		write_character(BUF_FLUSH);
		r = get_user_input(info);
		if (r != -1)
		{
			set_info(info, av);
			builtin_ret = find_builtin(info);
			if (builtin_ret == -1)
				find_cmd(info);
		}
		else if (check_interactive(info))
			write_character('\n');
		free_info(info, 0);
	}
	write_history(info);
	free_info(info, 1);
	if (!check_interactive(info) && info->status)
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
 * find_builtin - finds a builtin command
 *
 * Search for a builtin command in the command table and execute it if found.
 *
 * @info: The parameter & return info struct.
 *
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         -2 if builtin signals exit().
 */
int find_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", print_environment},
		{"help", _myhelp},
		{"history", display_history},
		{"setenv", set_environment_variable},
		{"unsetenv", unset_environment_variable},
		{"cd", _mycd},
		{"alias", manage_alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (str_compare(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 *
 * Search for a command in the system's PATH and execute it if found.
 *
 * @info: The parameter & return info struct.
 *
 * Return: void.
 */
void find_cmd(info_t *info)
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
		if (!check_delimiter(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_cmd_in_path(info, get_environment_variable(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_cmd(info);
	}
	else
	{
		if ((check_interactive(info) || get_environment_variable(info, "PATH=")
			|| info->argv[0][0] == '/') && is_executable(info, info->argv[0]))
			fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_error(info, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 *
 * Create a new process to execute the command.
 *
 * @info: The parameter & return info struct.
 *
 * Return: void.
 */
void fork_cmd(info_t *info)
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
			free_info(info, 1);
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
				print_error(info, "Permission denied\n");
		}
	}
}
