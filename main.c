#include "shell.h"

/**
 * start_program - Entry point of the program.
 * @argc: Number of command-line arguments.
 * @argv: Array of command-line arguments.
 *
 * Returns: 0 on success, 1 on error.
 */
int main(int argc, char **argv)
{
	info_t info[] = { INFO_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				print_string(argv[0]);
				print_string(": 0: Failed to open ");
				print_string(argv[1]);
				write_character('\n');
				write_character(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->readfd = fd;
	}
	populate_environment_list(info);
	read_history(info);
	hsh(info, argv);
	return (EXIT_SUCCESS);
}
