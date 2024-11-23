/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:42:18 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 12:08:47 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <sys/stat.h>

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
	unsigned char exit_status;
	int	num_pipes;
	char *oldpwd;
	char *pwd;
} t_ctx;

typedef struct s_var
{
    char **env;
	t_env_var *env_vars;
	struct s_env_var	*next;
	struct s_env_var	*name;
	struct s_env_var	*value;

} t_var;

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
	SINGLE_QUOTE,
	DOUBLEQUOTE
}						t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*next;
}						t_token;

/// loop

int						check_consecutive_pipes(t_token *tokens);
int						contains_pipe(t_token *tokens);

void					print_cmd(t_token *cmd, int index);
void					print_cmds(t_token *cmd_tokens, int num_pipes);
void					create_pipe(int pipe_fd[2]);
char					**prepare_print_args(t_token *cmd);
// char **prepare_args(t_token *tokens, int *exit_status);
char **prepare_args(t_token *tokens, t_ctx *ctx);
void					handle_child(t_token *cmd_tokens, int fd_in,
							int pipe_fd[2], int last_cmd);
void					handle_parent(int pipe_fd[2], int *fd_in, pid_t pid);
// void					handle_line(char *line, char **env);

int					loop(t_var *myEnv);
int handle_line(char *line, t_var *myEnv, t_ctx *ctx);

/// loop/read.c

void					process_tokens(t_token *cmd_tokens, int num_pipes);
void					exec_cmd(t_token *cmd, int fd_in, int pipe_fd[2],
							int last_cmd);
// void					exec_simple_cmd(t_token *tokens, char **env);
// void					exec_simple_cmd(t_token *tokens, char **env,
// 							int *exit_status);
// int exec_simple_cmd(t_token *tokens, t_var *myEnv, t_ctx *ctx);
// void					split_env_v(const char *input, char **var,
// 							char **value);
// int exec_simple_cmd(t_token *tokens, t_var *myEnv, t_ctx *ctx);
int exec_simple_cmd(t_token *tokens, t_var *myEnv, t_ctx *ctx);
int exec_builtin_cmd(char **args, t_var *myEnv, t_ctx *ctx);
char *strip_quotes(const char *arg);
void free_args(char **args);
int count_args(char **args);
int is_numeric_argument(const char *arg);
int process_exit_arg(char **args, t_ctx *ctx);
void print_env(char **env);
// void					read_and_exec(char **env);
int read_and_exec(t_var *myEnv);
int						count_tokens(t_token *tokens);
int split_env_v(char *arg, char **var, char **value);
t_ctx *initialize_ctx(void);
void free_ctx(t_ctx *ctx);
void free_env_vars(t_env_var *head);


/// env/enc.c

char					**export_v(char **env_copy, const char *var,
							const char *value);
// int						unset_v(char **env_copy, const char *var);
int unset_v(t_env_var **env_vars, const char *var);
t_env_var *build_env_vars(char **envp);

/// env/utils_env.c

int						ft_strncmp_export(const char *s1, const char *s2,
							unsigned int n);
int						is_valid_id(const char *var);
void 	free_environment(char **env_copy);


/// env/expand.c

char					*expand_env_var(char *token, int *exit_status);
char *ps_get_before_env(char *str, char *found);
char	*ps_get_env_var(char *var_name, t_ctx *ctx, char	**env);
char *ps_get_after_env(char *found) ;
char *ps_convert_to_env(char *str, char *found, t_ctx *ctx);
int ft_count_exp(char *str);
int ps_handle_env(t_token *token, t_ctx *ctx);
// void ps_expand_env(t_token *tokens, t_ctx *ctx);
void ps_expand_env(t_token *token, t_ctx *ctx, t_var *myEnv);
char *ps_strjoin(char *s1, const char *s2);
char *expand_variables(const char *str, t_ctx *ctx, t_token_type token_type);
char *find_env_value(const char *name, t_env_var *env_vars);

/// loop->parsing

// int					ft_cd_home(void);
int ft_cd(char **args, t_ctx *ctx);
int ft_cd_home(t_ctx *ctx);
int	ft_cd_oldpwd(t_ctx *ctx);
int ft_update_pwd(t_ctx *ctx);
t_token *create_token_list(char **args);
int ft_export(t_ctx *ctx, char *key, char *value);
t_env_var *create_env_var(const char *env_entry);
// int					ft_cd_oldpwd(char **oldpwd);
// int					ft_update_pwd(char **oldpwd);
// int					ft_cd(char **args);

/// signal

void					handle_sigint(int sig);
void					handle_sigquit(int sig);
void					init_sig(void);

/// parsing

/// lexer/tokens.c

t_token					*create_token(t_token_type type, const char *value);
// int					add_token(t_token **head, t_token_type type,
// 							const char *value);
t_token *add_token(t_token **token_list, t_token_type type, const char *value);
t_token_type			get_token_type(const char *str);
int						finalize_tokens(int in_quotes, char quote_char,
							char *buffer, int *i, t_token **token_list);

/// lexer/lexer.c

t_token *parse_command_line(char *line, t_ctx *ctx);

t_token					*add_pipe_token(t_token **head, t_token **tail);
int					process_token(t_token **head, t_token **tail,
							char *start, char *ptr, int first_token);
void					handle_token(t_token **head, t_token **tail, char **ptr,
							int *first_token);
t_token					*lexer(const char *input);

int	handle_env_var(char **ptr, t_token **head, t_token **tail, int *first_token);

/// lexer/handle.c

int						handle_quotes(char **line, int *in_quotes,
							char *quote_char, char *buffer, int *i,
							t_token **token_list);
int						handle_whitespace(char **line, char *buffer, int *i,
							t_token **token_list, int in_quotes);
int						handle_special_chars(char **line, char *buffer, int *i,
							t_token **token_list, int in_quotes);
void write_echo_content(t_token *token_list, int n_option, t_ctx *ctx);
void handle_echo(t_token *token_list, t_ctx *ctx);

/// lexer/lexer_utils.c

int						is_whitespace(char c);
int						handle_space(char **ptr);
t_token					*create_token_from_pipe(t_token **head, t_token **tail);
/// free

void					free_tokens(t_token *tokens);

/// main

void					print_tokens(t_token *tokens);

// exc_pipe

// int validate_pipe_syntax(t_token *tokens);
// int valide_pipes(t_token *tokens);
// void exc_error(t_token *tokens);
// void exc_pipe(t_token *tokens);

// pipex

// int	exec(t_token *cmd_tokens, char **env);
int exec(t_token *cmd_tokens, t_var *env, t_ctx *ctx);
// void	child(t_token *tokens, int *pipe_fd, char **env);
void    child(t_token *tokens, int *pipe_fd, t_var *env, t_ctx *ctx);
void	parent(t_token *tokens, int *pipe_fd,t_var *env, t_ctx *ctx);
size_t	ft_strlen(const char *c);
char	*my_getenv(char *name, char **env);
// char	*get_path(char *cmd, t_var *env);
char *get_path(char *cmd, t_env_var *env);
// char	*join_path_cmd(char *path, char *cmd);
char	*join_path_cmd(const char *path, const char *cmd);
char	*find_in_env(char *name, char **env);
void	*free_tab(char **tab);
void	exit_error(void);
//char **prepare_args(t_token *tokens);
//t_token *extract_command(t_token *tokens);
// int process_pline(t_token *tokens, char **env);
int process_pline(t_token *tokens, t_var *myEnv, t_ctx *ctx);
int	**pipe_tab(t_token *tokens);
int	count_commands(t_token *tokens);
char **get_environment(char **envp);
int get_output_fd(t_token *input_token);
int	is_pipe(t_token *tokens);
char **get_environment(char **envp);
char *search_in_path(const char *cmd, const char *path_env);


///free
// int					exec(t_token *cmd_tokens, char **env);
// void					parent(t_token *tokens, int *pipe_fd, char **env);
size_t					ft_strlen(const char *c);
char					*my_getenv(char *name, char **env);
// char					*get_path(char *cmd, t_var *env);
// char					*join_path_cmd(char *path, char *cmd);
char					*find_in_env(char *name, char **env);
void					*free_tab(char **tab);
void					exit_error(void);
// char					**prepare_args(t_token *tokens);
t_token					*extract_command(t_token *tokens);
t_token					*extract_command_after(t_token *tokens);
// int						process_pline(t_token *tokens, char **env);
char					**get_environment(char **envp);
char **convert_env_to_array(t_env_var *env);

/// pipe_utils_2
// int						process_pline(t_token *tokens, char **env);
void wait_for_all_children();
void cleanup_parent_resources(int *prev_fd, int *pipe_fd, t_token **cmd_start, t_token *cmd_end);
void execute_command_in_child(t_token *cmd_start, t_token *cmd_end, int prev_fd, int *pipe_fd, char **env);
void initialize_pipe_if_needed(int *pipe_fd, t_token *cmd_end);
void setup_pipe_for_child(int prev_fd, int *pipe_fd, int redirect_input, int redirect_output, t_token *cmd_end);
void collect_exec_tokens(t_token *cmd_start, t_token *cmd_end, t_token **exec_tokens, int *redirect, int *redirect_input, int *redirect_output);
void handle_output_redirection(t_token *redir_token, int *redirect, int *redirect_output);
void handle_input_redirection(t_token *redir_token, int *redirect, int *redirect_input);
char *get_env_value(const char *name, t_env_var *env);



/// free

void					*free_tab_2(char **tab);
void free_env_vars(t_env_var *env_vars);

#endif