#ifndef _SHELL_H_
#define _SHELL_H_

/*
 * File: shell.h
 * Auth: Boughrarah
 */

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

/* Read/write buffer sizes */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* Command chaining types */
#define CMD_NORM        0
#define CMD_OR          1
#define CMD_AND         2
#define CMD_CHAIN       3

/* Convert number types */
#define CONVERT_LOWERCASE       1
#define CONVERT_UNSIGNED        2

/* Maximum length of the history file */
#define HIST_FILE       ".simple_shell_history"
#define HIST_MAX        4096

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

extern char **environ;

/* Linked list node structure */
typedef struct liststr
{
        int num;
        char *str;
        struct liststr *next;
} list_t;

/* Information struct to pass pseudo-arguments to functions */
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
        int cmd_buf_type; /* CMD_type ||, &&, ; */
        int readfd;
        int histcount;
} info_t;

#define INFO_INIT \
{ \
    NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0 \
}
/* Builtin command struct */
typedef struct builtin
{
        char *type;
        int (*func)(info_t *);
} builtin_table;

/* Function prototypes */

/* toem_shloop.c */
int hsh(info_t *, char **);
int find_builtin(info_t *);
void find_cmd(info_t *);
void fork_cmd(info_t *);

/* toem_parser.c */
int is_executable(info_t *, char *);
char *duplicate_substring(char *, int, int);
char *find_cmd_in_path(info_t *, char *, char *);

/* loophsh.c */
int run_shell(char **);

/* toem_errors.c */
void print_error(info_t *, const char *);
int printCharacterToStandardError(char c);
int printCharacterToFileDescriptor(char c, int fd);
int printStringToFileDescriptor(char *str, int fd);

/* toem_string.c */
int str_length(char *);
int str_compare(char *, char *);
char *str_starts_with(const char *, const char *);
char *str_concatenate(char *, char *);

/* toem_string1.c */
char *copy_string(char *, char *);
char *duplicate_string(const char *);
void print_string(char *);
int write_character(char c);

/* toem_exits.c */
char *my_strncpy(char *, char *, int);
char *my_strncat(char *, char *, int);
char *my_strchr(char *, char);

/* toem_tokenizer.c */
char **split_string(char *, char *);
char **split_string2(char *, char);

/* toem_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bfree(void **);

/* toem_atoi.c */
int check_interactive(info_t *);
int check_delimiter(char, char *);
int check_alpha_char(int);
int convert_to_int(char *);
int _erratoi(const char *);

/* toem_errors1.c */
int convert_string_to_integer(char *);
void print_error_messag(info_t *, const char *);
int print_decimal(int, int);
char *convert_number_to_string(long int, int, int);
void remove_comments_from_string(char *);

/* toem_builtin.c */
int _myexit(info_t *);
int _mycd(info_t *);
int _myhelp(info_t *);

/* toem_builtin1.c */
int display_history(info_t *);
int unset_alias(info_t *, char *);
int set_alias(info_t *, char *);
int print_alias(list_t *);
int manage_alias(info_t *);

/*toem_getline.c */
ssize_t get_user_input(info_t *);
ssize_t read_buffer(info_t *, char *, size_t *);
int _getline(info_t *, char **, size_t *);
ssize_t buffer_input(info_t *, char **, size_t *);
void sigintHandler(int);

/* toem_getinfo.c */
void initialize_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

/* toem_environ.c */
char *get_environment_variable(info_t *, const char *);
int print_environment(info_t *);
int set_environment_variable(info_t *);
int unset_environment_variable(info_t *);
int populate_environment_list(info_t *);

/* toem_getenv.c */
char **get_environ(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* toem_history.c */
char *get_history_file(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/* toem_lists.c */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* toem_lists1.c */
size_t list_length(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *find_node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* toem_vars.c */
int is_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_string(char **, char *);

#endif
