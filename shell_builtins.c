#include "shell.h"
#include "error_functions.h"
#include "environment_functions.h"
#include <string.h>

/**
 * exit_shell - Function to exit the shell.
 * @info: Structure containing potential arguments. Used to maintain
 *         constant function prototype.
 *
 * Return: Exits with a given exit status.
 *         (0) if info->argv[0] != "exit".
 */
int exit_shell(info_t *info)
{
	int exit_status;

	if (info->argv[1])
	{
		exit_status = error_atoi(info->argv[1]);
		if (exit_status == -1)
		{
			info->status = 2;
			_puts(info, "Illegal number: ");
			puts(info->argv[1]);
			putchar('\n');
			return (1);
		}
		info->err_num = _erratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * change_directory - Function to change the current directory.
 * @info: Structure containing potential arguments. Used to maintain
 *         constant function prototype.
 *
 * Return: Always 0.
 */
int change_directory(info_t *info)
{
	char *current_dir, *dir, buffer[1024];
	int chdir_ret;

	current_dir = getcwd(buffer, 1024);
	if (!current_dir)
		puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = getenv(info, "HOME=");
		if (!dir)
			chdir_ret = chdir((dir = _getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!_getenv(info, "OLDPWD="))
		{
			_puts(current_dir);
			putchar('\n');
			return (1);
		}
		_puts(_getenv(info, "OLDPWD="));
		_putchar('\n');
		chdir_ret = chdir((dir = getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);

	if (chdir_ret == -1)
	{
		print_error(info, "can't cd to ");
		_eputs(info->argv[1]);
		_eputchar('\n');
	}
	else
	{
		_setenv(info, "OLDPWD", _getenv(info, "PWD="));
		_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * help_command - Function to display help information.
 * @info: Structure containing potential arguments. Used to maintain
 *         constant function prototype.
 *
 * Return: Always 0.
 */
int help_command(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* Temporary att_unused workaround */
	return (0);
}
