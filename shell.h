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

/* Builtin command struct */
typedef struct builtin
{
        char *type;
        int (*func)(info_t *);
} builtin_table;

/* Function prototypes */

/* toem_shloop.c */
int shell_loop(info_t *, char **);
int find_builtin(info_t *);
void find_cmd(info_t *);
void fork_cmd(info_t *);

/* toem_parser.c */
int is_cmd(info_t *, char *);
char *duplicate_chars(char *, int, int);
char *find_path(info_t *, char *, char *);

/* loophsh.c */
int run_shell(char **);

/* toem_errors.c */
void print_error(info_t *, const char *);
int print_char(int c, int fd);
int print_str_fd(const char *str, int fd);

/* toem_string.c */
int string_length(const char *);
int string_compare(const char *, const char *);
char *starts_with(const char *, const char *);
char *string_concat(char *, char *);

/* toem_string1.c */
char *string_copy(char *, const char *);
char *string_duplicate(const char *);
void print_string(const char *);
int print_char(int c, int fd);

/* toem_exits.c */
char *string_copy_n(char *, const char *, int);
char *string_concat_n(char *, const char *, int);
char *string_find_char(const char *, char);

/* toem_tokenizer.c */
char **string_tokenize(char *, char *);
char **string_tokenize2(char *, char);

/* toem_realloc.c */
char *memory_set(char *, char, unsigned int);
void free_array(char **);
void *custom_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int custom_free(void **);

/* toem_atoi.c */
int is_interactive(info_t *);
int is_delimiter(char, char *);
int is_alpha(int);
int string_to_integer(char *);
int _erratoi(const char *);

/* toem_errors1.c */
int error_to_integer(char *);
void print_error_info(info_t *, const char *);
int print_decimal(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* toem_builtin.c */
int builtin_exit(info_t *);
int builtin_cd(info_t *);
int builtin_help(info_t *);

/* toem_builtin1.c */
int builtin_history(info_t *);
int builtin_alias(info_t *);

/*toem_getline.c */
ssize_t get_input(info_t *);
int custom_getline(info_t *, char **, size_t *);
void sigintHandler(int);

/* toem_getinfo.c */
void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

/* toem_environ.c */
char *get_environment(info_t *, const char *);
int builtin_env(info_t *);
int builtin_setenv(info_t *);
int builtin_unsetenv(info_t *);
int populate_env_list(info_t *);

/* toem_getenv.c */
char **get_environ(info_t *);
int custom_unsetenv(info_t *, char *);
int custom_setenv(info_t *, char *, char *);

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
size_t list_len(const list_t *);
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
