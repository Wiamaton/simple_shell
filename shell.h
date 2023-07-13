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

/* Buffer sizes for reading and writing */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* Command chaining options */
#define CMD_NORM 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

/* Conversion options for numbers */
#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

/* Flag indicating use of system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE ".simple_shell_history"
#define HIST_MAX 4096

extern char **environ;

/**
 * Structure for a singly linked list
 * @num: number field
 * @str: string field
 * @next: pointer to the next node
 */
typedef struct liststr
{
    int num;
    char *str;
    struct liststr *next;
} list_t;

/**
 * Structure for passing pseudo-arguments to a function,
 * allowing a uniform prototype for a function pointer struct
 * @arg: string generated from getline containing arguments
 * @argv: array of strings generated from arg
 * @path: string path for the current command
 * @argc: argument count
 * @line_count: error count
 * @err_num: error code for exit()s
 * @linecount_flag: flag indicating whether to count this line of input
 * @fname: program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: history node
 * @alias: alias node
 * @env_changed: flag indicating whether environ was changed
 * @status: return status of the last executed command
 * @cmd_buf: address of pointer to cmd_buf, flag indicating chaining
 * @cmd_buf_type: CMD_type (||, &&, ;)
 * @readfd: file descriptor from which to read line input
 * @histcount: history line number count
 */
typedef struct passinfo
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

    char **cmd_buf; /* Pointer to cmd ; chain buffer, for memory management */
    int cmd_buf_type; /* CMD_type (||, &&, ;) */
    int readfd;
    int histcount;
} info_t;

#define INFO_INIT \
{ \
    NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0 \
}

/**
 * Structure for built-in commands
 * @type: built-in command flag
 * @func: function pointer
 */
typedef struct builtin
{
    char *type;
    int (*func)(info_t *);
} builtin_table;

/* Main shell loop */
int custom_shell(info_t *, char **);

/* Finding built-in commands */
int find_builtin_command(info_t *);

/* Finding the command */
void find_command(info_t *);

/* Forking the command */
void fork_command(info_t *);

/* Parsing the command */
int is_command(info_t *, char *);
char *duplicate_characters(char *, int, int);
char *find_path(info_t *, char *, char *);

/* Custom shell loop */
int custom_shell_loop(char **);

/* Error handling */
void custom_puts_error(char *);
int custom_putchar_error(char);
int custom_putfd(char c, int fd);
int custom_putsfd(char *str, int fd);

/* String manipulation */
int custom_strlen(char *);
int custom_strcmp(char *, char *);
char *starts_with_string(const char *, const char *);
char *custom_strcat(char *, char *);

/* String manipulation - part 1 */
char *custom_strcpy(char *, char *);
char *custom_strdup(const char *);
void custom_puts(char *);
int custom_putchar(char);

/* Exit handling */
char *custom_strncpy(char *, char *, int);
char *custom_strncat(char *, char *, int);
char *custom_strchr(char *, char);

/* Tokenizing the input */
char **split_string(char *, char *);
char **split_string_2(char *, char);

/* Memory reallocation */
char *custom_memset(char *, char, unsigned int);
void free_memory(char **);
void *custom_realloc(void *, unsigned int, unsigned int);

/* Memory management */
int custom_bfree(void **);

/* String to integer conversion */
int interactive_mode(info_t *);
int is_delimiter(char, char *);
int is_alpha(int);
int custom_atoi(char *);

/* Error handling - part 1 */
int error_atoi(char *);
void print_custom_error(info_t *, char *);
int print_decimal(int, int);
char *convert_numeric(long int, int, int);
void remove_comments(char *);

/* Built-in commands */
int custom_exit(info_t *);
int custom_cd(info_t *);
int custom_help(info_t *);

/* Built-in commands - part 1 */
int custom_history(info_t *);
int custom_alias(info_t *);

/* Getting input */
ssize_t get_input(info_t *);
int custom_getline(info_t *, char **, size_t *);
void signal_interrupt_handler(int);

/* Getting information */
void clear_information(info_t *);
void set_information(info_t *, char **);
void free_information(info_t *, int);

/* Environmental variables */
char *get_environment(info_t *, const char *);
int custom_env(info_t *);
int custom_setenv(info_t *, char *, char *);
int custom_unsetenv(info_t *, char *);

/* Getting environment variables */
char **get_environment_variables(info_t *);
int custom_unsetenv(info_t *, char *);
int custom_setenv(info_t *, char *, char *);

/* History handling */
char *get_history_file(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/* Linked lists */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_string_list(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* Linked lists - part 1 */
size_t list_length(const list_t *);
char **list_to_array(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* Variable replacement */
int is_command_chain(info_t *, char *, size_t *);
void check_command_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_variables(info_t *);
int replace_string(char **, char *);

#endif
