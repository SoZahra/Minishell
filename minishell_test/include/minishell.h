#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern char				**environ;

# define PROMPT "\033[1;34mMiniBG>\033[0m "

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef struct s_env_var
{
	char				*name;
	char				*value;
	struct s_env_var	*next;
}						t_env_var;
typedef struct s_ctx
{
    t_env_var *env_vars;  // Liste des variables d'environnement
	int exit_status;
} t_ctx;

typedef enum token_type
{
	TOKEN_COMMAND,
	TOKEN_ARGUMENT,
	TOKEN_HEREDOC,
	TOKEN_REDIRECT_INPUT,  // <
	TOKEN_REDIRECT_OUTPUT, // >
	TOKEN_REDIRECT_APPEND, // >>
	TOKEN_PIPE,            // |
	TOKEN_FILENAME,	// Ajoute ce type pour les fichiers après redirection
	TOKEN_REDIRECT_HEREDOC, // <<
	TOKEN_END,
	STRING,
	DOUBLEQUOTE
}						t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*next;
}						t_token;


char	**get_environment(char **envp);
int export_v(char ***env_copy, const char *var, const char *value);
int unset_v(char ***env_copy, const char *var);
char *expand_env_var(char *token, int *exit_status);
char *ps_get_before_env(char *str, char *found);
char *ps_get_env_var(char *var_name, t_ctx *ctx);
char *ps_get_after_env(char *found);
char *ps_convert_to_env(char *str, char *found, t_ctx *ctx);
int ft_count_exp(char *str);
int ps_handle_env(t_token *token, t_ctx *ctx);
void ps_expand_env(t_token *tokens, t_ctx *ctx);
int	ft_strncmp_export(const char *s1, const char *s2, unsigned int n);
int	is_valid_id(const char *var);
void	write_echo_content(t_token *token_list, int n_option);
void	handle_echo(t_token *token_list);
int	handle_quotes(char **line, int *in_quotes, char *quote_char, char *buffer,
		int *i, t_token **token_list);
int	handle_whitespace(char **line, char *buffer, int *i, t_token **token_list,
		int in_quotes);
int	handle_special_chars(char **line, char *buffer, int *i,
		t_token **token_list, int in_quotes);
int	is_whitespace(char c);
int	handle_space(char **ptr);
t_token	*create_token_from_pipe(t_token **head, t_token **tail);
t_token	*parse_command_line(char *line);
t_token	*add_pipe_token(t_token **head, t_token **tail);
int	process_token(t_token **head, t_token **tail, char *start, char *ptr,
		int first_token);
void	handle_token(t_token **head, t_token **tail, char **ptr,
		int *first_token);
int	handle_env_var(char **line, t_token **token_list);
t_token	*lexer(const char *input);
t_token	*create_token(t_token_type type, const char *value);
int	add_token(t_token **head, t_token_type type, const char *value);
t_token_type	get_token_type(const char *str);
int	finalize_tokens(int in_quotes, char quote_char, char *buffer, int *i,
		t_token **token_list);
void	handle_child(t_token *cmd_tokens, int fd_in, int pipe_fd[2],
		int last_cmd);
void	handle_parent(int pipe_fd[2], int *fd_in, pid_t pid);
int handle_line(char *line, char **env, t_ctx *ctx);
int	ft_cd(char **args);
int	ft_cd_home(void);
int	ft_cd_oldpwd(char **oldpwd);
int	ft_update_pwd(char **oldpwd);
void	print_all_cmds(t_token *cmd_tokens, int num_pipes);
void	create_pipe(int pipe_fd[2]);
char	**prepare_print_args(t_token *cmd);
void	exec_cmd(t_token *cmd, int fd_in, int pipe_fd[2], int last_cmd);
void	process_tokens(t_token *cmd_tokens, int num_pipes);
int count_tokens(t_token *tokens);
int exec_simple_cmd(t_token *tokens, char **env, t_ctx *ctx);
void split_env_v(const char *input, char **var, char **value);
int exec_builtin_cmd(char **args, char **env);
int read_and_exec(char **env);
int	loop(char **env);
int	validate_pipe_syntax(t_token *tokens);
int	validate_pipes(t_token *tokens);
int	exc_error(t_token *tokens);
int	exc_pipe(t_token *tokens);
t_token	*extract_command(t_token *tokens);
t_token	*extract_command_after(t_token *tokens);
int	process_pline(t_token *tokens, char **env);
void	exit_error(void);
void	*free_tab(char **tab);
char	*find_in_env(char *name, char **env);
char	*join_path_cmd(char *path, char *cmd);
char *get_path(char *cmd, char **env);
char **prepare_args(t_token *tokens, int *exit_status);
int	exec(t_token *cmd_tokens, char **env);
void	child(t_token *tokens, int *pipe_fd, char **env);
void	parent(t_token *tokens, int *pipe_fd, char **env);
int	check_consecutive_pipes(t_token *tokens);
int	contains_pipe(t_token *tokens);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	init_sig(void);
void	free_tokens(t_token *tokens);
int	main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
		char **envp);
void	print_tokens(t_token *tokens);

#endif