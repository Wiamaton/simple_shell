#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define BUF_READ_SIZE 1024
#define BUF_WRITE_SIZE 1024
#define BUFFER_FLUSH -1

/* for command chaining */
#define NORMAL_CMD	0
#define OR_CMD		1
#define AND_CMD		2
#define CHAIN_CMD	3

/* for convert_num_to_str() */
#define CONVERT_LC	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define GETLINE 0
#define _STRTOK 0

#define HISTORY_FILE	".shell_history"
#define HISTORY_MAX	4096

extern char **environ;


/**
 * struct s_linked_list - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct s_linked_list
{
	int num;
	char *str;
	struct s_linked_list *next;
} list_t;

/**
 * struct param - contains pseudo-arguements to pass into a function,
 * allowing uniform prototype for function pointer struct
 * @arg: a string generated from getline containing arguements
 * @argv:an array of strings generated from arg
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount_flag: if on count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: the history node
 * @alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @histcount: the history line number count
 */
typedef struct param
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} param_t;

#define PARAM_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(param_t *);
} builtin_t;


/* toem_shloop.c */
int shell(param_t *, char **);
int find_builtin(param_t *);
void find_cmd(param_t *);
void fork_cmd(param_t *);

/* toem_parser.c */
int is_cmd(param_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(param_t *, char *, char *);

/* loop.c */
int loop(char **);

/* toem_errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* toem_tokenizer.c */
char **_strtok(char *, char *);
char **_strtok2(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bfree(void **);

/* toem_atoi.c */
int is_interactive(param_t *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* toem_errors1.c */
int _erratoi(char *);
void _perror(param_t *, char *);
int print_d(int, int);
char *convert_num_to_str(long int, int, int);
void del_comments(char *);

/* toem_builtin.c */
int _myexit(param_t *);
int change_dir(param_t *);
int _help(param_t *);

/* toem_builtin1.c */
int _history(param_t *);
int _alias(param_t *);

/*toem_getline.c */
ssize_t get_input(param_t *);
int _getline(param_t *, char **, size_t *);
void sigintHandler(int);

/* toem_getinfo.c */
void clear_param(param_t *);
void set_param(param_t *, char **);
void free_param(param_t *, int);

/* toem_environ.c */
char *_getenv(param_t *, const char *);
int _env(param_t *);
int _mysetenv(param_t *);
int _myunsetenv(param_t *);
int populate_env_list(param_t *);

/* toem_getenv.c */
char **get_environ(param_t *);
int _unsetenv(param_t *, char *);
int _setenv(param_t *, char *, char *);

/* toem_history.c */
char *get_history_file(param_t *info);
int write_history(param_t *info);
int read_history(param_t *info);
int build_history_list(param_t *info, char *buf, int linecount);
int renumber_history(param_t *info);

/* toem_lists.c */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* toem_lists1.c */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* toem_vars.c */
int is_chain(param_t *, char *, size_t *);
void check_chain(param_t *, char *, size_t *, size_t, size_t);
int replace_alias(param_t *);
int replace_vars(param_t *);
int replace_string(char **, char *);

#endif
