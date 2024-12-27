#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define PROMPT "\033[1;34mMiniBG>\033[0m "
# define TOKENS "<>|\"'"
# define REDIRS "<>HA"
# define OPERATORS "AH<>|"
# define UNJOIN "<>|"
extern int g_heredoc_active;
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# define BOOL	char
# define TRUE	1
# define FALSE	0

typedef struct s_env_var
{
	char					*name;
	char					*value;
	struct s_env_var		*next;
}							t_env_var;

typedef enum token_type
{
	TOKEN_HEREDOC = 'H',
	TOKEN_REDIRECT_INPUT = '<',  // <
	TOKEN_REDIRECT_OUTPUT = '>', // >
	TOKEN_REDIRECT_APPEND = 'A', // >>
	TOKEN_PIPE = '|',            // |
	STRING = 'S',
	CMD = 'C',
	DOUBLEQUOTE = '"',
	SINGLE_QUOTE = '\'',
}							t_token_type;

typedef struct s_redirection
{
	char type;  // '>' pour >, 'A' pour >>, '<' pour <, 'H' pour
	char *file; // Nom du fichier ou délimiteur
	int fd;     // File descriptor
	struct s_redirection	*next;
	int						heredoc_fd;
}							t_redirection;

typedef struct s_command
{
	pid_t					pid;
	char **args; // Les arguments de la commande
	char					*path;
	t_redirection *redirs; // Liste des redirections
	int						in_fd;
	int						out_fd;
	int						pfd[2];
	int						arg_count;
	struct s_command *next; // Commande suivante dans le pipe
	struct s_command		*prev;
	int						*had_spaces;
}							t_command;

typedef struct s_token
{
	char					*value;
	t_token_type			type;
	struct s_token			*next;
	struct s_token			*prev;
	char					*content;
	int						quoted;
	int						had_space;
}							t_token;

typedef struct s_ctx
{
	t_token					*tokens;
	t_env_var				*env_vars;
	unsigned char			exit_status;
	int						num_pipes;
	char					*oldpwd;
	char					*pwd;
	struct termios			term;
	struct sigaction		sa_int;
	struct sigaction		sa_quit;
	t_command				*current_command;
	int						save_stdin;
	int						save_stdout;
	int						pfd[2];
}							t_ctx;

// -------------------------------------------------------------

// expand

char						*expand_full_string(const char *str,
								char quote_type, t_ctx *ctx);
char						*tokens_to_string(t_token *tokens);
int							join_proc(t_token **tokens, bool limiter);
int							join_tokens(t_token *prev, t_token *current);
void						token_del(t_token *token);
int							join_str(t_token *token, bool limiter);
int							expand_proc(t_token **tokens, t_ctx *ctx);
int							expand_str(t_token *token, t_ctx *ctx);
t_command					*link_commands(t_command *first_cmd,
								t_command *new_cmd);

// builtin

int							execute_builtin(char *input, t_ctx *ctx);
int							is_builtin(const char *cmd);

// exit
int							handle_exit_builtin(const char *input, t_ctx *ctx);
int							process_exit_arg(char **args, t_ctx *ctx);
int							handle_exit_with_argument(char **args, t_ctx *ctx);
int							clear_exit(t_ctx *ctx);

// export
int							handle_export_builtin(const char *input,
								t_ctx *ctx);
int							handle_error(const char *arg, t_ctx *ctx);
int							handle_no_args(t_ctx *ctx);
int							handle_multiple_args(const char *args, t_ctx *ctx);
int							handle_single_arg(char *args, t_ctx *ctx);
int							export_single_var(const char *arg, t_ctx *ctx);
int							handle_no_equal(const char *arg, t_ctx *ctx);
void						print_export(t_ctx *ctx);
int							create_var_with_value(const char *name,
								const char *value, t_ctx *ctx);

// echo
int							handle_echo_builtin(const char *input, t_ctx *ctx);
int							handle_echo_builtin_n(const char *args, t_ctx *ctx);
int							is_valid_n(t_token *current);

// env
int							handle_env_builtin(const char *input, t_ctx *ctx);
void						print_env(t_ctx *ctx);

// cd
int							handle_cd_builtin(const char *input, t_ctx *ctx);
int							ft_cd_home(t_ctx *ctx);
int							ft_cd_oldpwd(t_ctx *ctx);
int							ft_update_pwd(t_ctx *ctx);

// pwd
int							handle_pwd_builtin(const char *input, t_ctx *ctx);

// unset

int							unset_v(t_env_var **env_vars, const char *var);
int							handle_unset_builtin(const char *input, t_ctx *ctx);

// utils
int							ft_fprintf(int fd, const char *format, ...);
size_t						ft_strspn(const char *str, const char *accept);
int							count_args(char **args);
void						print_tokens(t_token *tokens);
void						print_command_debug(t_command *cmd);
int							is_valid_var_name(const char *name);
int							is_whitespace(char c);
int							is_numeric_argument(const char *arg);

// convert

t_command					*create_command_from_tokens_range(t_token *start,
								t_token *end);
char						*tokens_to_string_from_command(t_command *cmd);

// exec

int							exec_loop(t_ctx *ctx, t_command *cmd);

//heredoc
void close_heredocs(t_command *cmd);

// loop

int							handle_line_for_loop(char *line, t_ctx *ctx);
int							process(t_ctx *ctx);
t_command					*parse_pipe_sequence(t_token *tokens);

// signaux

//void						handle_sigint(int sig);
//void						handle_sigquit(int sig);
//void						sighndl_sigint_main(int sig);
//void sigset_sigint_main(int sig);
//void sigset_sigquit_main(int sig);
//void sigset_sigint_child(int sig);
//void sigset_sigquit_child(int sig);
//void sigset_sigint_heredoc(int sig);
//void sigset_sigquit_heredoc(int sig);
//void init_heredoc_sig(void);
//void restore_main_sig(void);
void init_child_sig(void);
void    init_sig(void);
void init_heredoc_sig(void);
void restore_main_sig(void);

void sigint_main_ignore(void);
void sigint_main_handl(void);

void    setsig(struct sigaction *sa, int signum, void (*f)(int), int flags);

// env

t_env_var					*build_env_list(char **envp);
int						add_env_var_to_list(t_env_var **head,
								t_env_var *new_var);
t_env_var					*get_last_env_node(t_env_var **env);
int							create_and_add_var(t_ctx *ctx, char *var,
								char *value);
char						**create_env_array(t_env_var *env_vars);
char						*find_command_path(const char *cmd, t_ctx *ctx);
char						*find_env_value(const char *name,
								t_env_var *env_vars);
char	*ps_get_env_var(char *var_name, t_ctx *ctx);

// Tokens

int							tokenizer(t_token **tokens, char *input);
int							add_token(t_token **token_list, t_token_type type,
								char *value);
t_token						*create_new_token(t_token_type type, char *value);
t_token						*tokenize_input(char *line);
t_token						*create_token_node(char *arg, t_token_type type);
void count_tokens(t_token *start, t_token *end, int *arg_c, int *red_c);
bool						is_token(char c, char *str);

// free

void						free_pipeline_pipes(int **pipes, int num_commands);
void						cmd_clean_and_exit(t_ctx *ctx, t_command *cmd,
								char **env_v, int exit_code);
void						free_ctx(t_ctx *ctx);
void						free_env(t_env_var *env_var);
void						free_command_list(t_command *cmd);
void						free_command(t_command *cmd);
void						*free_tab(char **tab);
void						clear(pid_t *pids, t_command *cmds, int exit_code);
void						free_args(char **args);
void						free_tokens(t_token *tokens);
void						free_env_copy(char **env_copy);
void						clear_and_exit(pid_t *pids, t_command *cmds,
								int exit_code);
void						free_env_list(t_env_var *head);
void						cleanup_shell(t_ctx *ctx);
void						free_array(char **array);

// attribut / init
int							set_term_attr(void);
int							get_term_attr(void);
int							initialize_ctx(t_ctx *ctx);
t_ctx						*get_ctx(void);
void						init_sig(void);

#endif
