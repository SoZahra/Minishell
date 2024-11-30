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
# include <sys/stat.h> // Pour struct stat et stat()
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern char				**environ;

# define PROMPT "\033[1;34mMiniBG>\033[0m "

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

// typedef struct s_var
// {
//     char **env;
// } t_var;
typedef struct s_env_var
{
	char				*name;
	char				*value;
	struct s_env_var	*next;
	// struct t_ctx 		*env_vars;
}						t_env_var;

typedef struct s_ctx
{
	t_env_var 			*env_vars; // Liste des variables d'environnement
	unsigned char		exit_status;
	int					num_pipes;
	char				*oldpwd;
	char				*pwd;
	struct s_ctx		*next;
	// char				*name;
	// char				*value;

}						t_ctx;

// typedef struct s_var
// {
// 	char				**env;
// }						t_var;

typedef enum token_type
{
	TOKEN_HEREDOC = 'H',
	TOKEN_REDIRECT_INPUT = '<',  // <
	TOKEN_REDIRECT_OUTPUT = '>', // >
	TOKEN_REDIRECT_APPEND = 'A', // >>
	TOKEN_PIPE = '|',            // |
	STRING = 'T',
	DOUBLEQUOTE = '"',
	SINGLE_QUOTE = '\'',
}						t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*next;
	char				*content;
	int					quoted;
}						t_token;

typedef struct s_pipe_cmd
{
	t_token				*cmd;
	int					input_fd;
	int					output_fd;
	int					pipe_read;
	int					pipe_write;
	struct s_pipe_cmd	*next;
}						t_pipe_cmd;

char					**get_environment(char **envp);
// t_env_var *get_environment(char **envp);
// int						export_v(char ***env_copy, const char *var,
// 							const char *value);
t_env_var *export_v(t_env_var *env_vars, const char *var, const char *value);
// int unset_v(char ***env_copy, const char *var);
// int unset_v(char ***env_copy, const char *var);
// int unset_v(t_env_var **env_vars, const char *var, t_ctx *ctx);
int						unset_v(t_env_var **env_vars, const char *var);
// char *expand_env_var(char *token, int *exit_status);
char					*ps_get_before_env(char *str, char *found);
char					*ps_get_env_var(char *var_name, t_ctx *ctx);
char					*ps_get_after_env(char *found);
char					*ps_convert_to_env(char *str, char *found, t_ctx *ctx);
int						ft_count_exp(char *str);
int						ps_handle_env(t_token *token, t_ctx *ctx);
// void ps_expand_env(t_token *tokens, t_ctx *ctx);
int						ft_strncmp_export(const char *s1, const char *s2,
							unsigned int n);
int						is_valid_id(const char *var);
// void	write_echo_content(t_token *token_list, int n_option);
void					write_echo_content(t_token *token_list, int n_option,
							t_ctx *ctx);
// void	handle_echo(t_token *token_list);
void					handle_echo(t_token *token_list, t_ctx *ctx);
int						handle_quotes(char **line, int *in_quotes,
							char *quote_char, char *buffer, int *i,
							t_token **token_list);
int						handle_whitespace(char **line, char *buffer, int *i,
							t_token **token_list, int in_quotes);
int						handle_special_chars(char **line, char *buffer, int *i,
							t_token **token_list, int in_quotes);
int						is_whitespace(char c);
int						handle_space(char **ptr);
t_token					*create_token_from_pipe(t_token **head, t_token **tail);
// t_token	*parse_command_line(char *line);
// t_token	*parse_command_line(char *line, t_ctx *exit_status);
t_token					*parse_command_line(char *line, t_ctx *ctx);
t_token					*add_pipe_token(t_token **head, t_token **tail);
// int	process_token(t_token **head, t_token **tail, char *start, char *ptr,
// 		int first_token);
void					process_tokens(t_token *cmd_tokens, int num_pipes,
							t_ctx *ctx);
void					handle_token(t_token **head, t_token **tail, char **ptr,
							int *first_token);
// int	handle_env_var(char **line, t_token **token_list);
int						handle_env_var(char **line, t_token **token_list,
							t_ctx *ctx);
t_token					*lexer(const char *input);
t_token					*create_token(t_token_type type, const char *value);
// int	add_token(t_token **head, t_token_type type, const char *value);
t_token					*add_token(t_token **token_list, t_token_type type,
							const char *value);
t_token_type			get_token_type(const char *str);
int						finalize_tokens(int in_quotes, char quote_char,
							char *buffer, int *i, t_token **token_list);
// void	handle_child(t_token *cmd_tokens, int fd_in, int pipe_fd[2],
// 		int last_cmd);
void					handle_child(t_token *cmd_tokens, int fd_in,
							int pipe_fd[2], t_ctx *ctx);
void					handle_parent(int pipe_fd[2], int *fd_in, pid_t pid);
int						handle_line(char *line, char **env, t_ctx *ctx);
// int	ft_cd(char **args);
// int	ft_cd_home(void);
int						ft_cd_home(t_ctx *ctx);
int						ft_cd(char **args, t_ctx *ctx);
// int	ft_cd_oldpwd(char **oldpwd);
int						ft_cd_oldpwd(t_ctx *ctx);
int						ft_update_pwd(t_ctx *ctx);
// int	ft_update_pwd(char **oldpwd);
void					print_all_cmds(t_token *cmd_tokens, int num_pipes);
void					create_pipe(int pipe_fd[2]);
// char	**prepare_print_args(t_token *cmd);
char					**prepare_print_args(t_token *cmd, t_ctx *ctx);
// void	exec_cmd(t_token *cmd, int fd_in, int pipe_fd[2], int last_cmd);
void					exec_cmd(t_token *cmd, int fd_in, int pipe_fd[2],
							t_ctx *ctx);
// void	process_tokens(t_token *cmd_tokens, int num_pipes);
int						count_tokens(t_token *tokens);
int exec_simple_cmd(t_token *tokens, char **env, t_ctx *ctx);
// int exec_simple_cmd(t_token *tokens, char **env, t_ctx *ctx);
// void split_env_v(const char *input, char **var, char **value);
// int exec_builtin_cmd(char **args, char **env);
int						split_env_v(char *arg, char **var, char **value);
// int exec_builtin_cmd(char **args, char **env, t_ctx *ctx);
// int						exec_builtin_cmd(char **args, t_env_var *env, t_ctx *ctx);
int exec_builtin_cmd(char **args, char **env, t_ctx *ctx);
t_token					*create_token_list(char **args);

// int read_and_exec(char **env);
int						read_and_exec(char **env, t_ctx *ctx);
char					*strip_quotes(char *arg);
int						is_numeric_argument(const char *arg);
int						process_exit_arg(char **args, t_ctx *ctx);
void					handle_exit_command(char *line, t_ctx *ctx);
// int	loop(char **env);
int						loop(char **env, t_ctx *ctx);
int						validate_pipe_syntax(t_token *tokens);
int						validate_pipes(t_token *tokens);
int						exc_error(t_token *tokens);
int						exc_pipe(t_token *tokens);
t_token					*extract_command(t_token *tokens);
t_token					*extract_command_after(t_token *tokens);
// int	process_pline(t_token *tokens, char **env);
// int	process_pline(t_token *tokens, char **env, t_ctx *ctx);
// int process_pline(t_token *tokens, char **env, t_ctx *ctx);
// int						process_pline(t_token *tokens, t_var *myEnv,
// 							t_ctx *ctx);
// int process_pline(t_token *tokens, t_env_var *myEnv, t_ctx *ctx);
int process_pline(t_token *tokens, char **env, t_ctx *ctx);
void					*free_tab(char **tab);
// char					*find_in_env(char *name, char **env);
char *find_in_env_array(char *name, char **env);
char	*find_in_env(char *name, t_env_var *env_vars);
char					*join_path_cmd(char *path, char *cmd);
char					*get_path(char *cmd, char **env);
// char **prepare_args(t_token *tokens, int *exit_status);
char					**prepare_args(t_token *tokens, t_ctx *ctx);
// int	exec(t_token *cmd_tokens, char **env);
int						exec(t_token *cmd_tokens, char **env, t_ctx *ctx);
// void	child(t_token *tokens, int *pipe_fd, char **env);
void					child(t_token *tokens, int *pipe_fd, char **env,
							t_ctx *ctx);
// void	parent(t_token *tokens, int *pipe_fd, char **env);
void					parent(t_token *tokens, int *pipe_fd, char **env,
							t_ctx *ctx);
int						check_consecutive_pipes(t_token *tokens);
int						contains_pipe(t_token *tokens);
void					handle_sigint(int sig);
void					handle_sigquit(int sig);
void					init_sig(void);
void					free_tokens(t_token *tokens);
int						main(int argc __attribute__((unused)),
							char **argv __attribute__((unused)), char **envp);
void					print_tokens(t_token *tokens);
void					exit_error(void);

// char *expand_variables(const char *str, t_ctx *ctx);
t_env_var				*create_env_var(const char *env_entry);
char					*find_env_value(const char *name, t_env_var *env_vars);
char					**copy_envp(char **envp);
int						ft_export(t_ctx *ctx, char *key, char *value);
void					free_env_copy(char **env_copy);
void					free_env_vars(t_env_var *env_vars);
char					*expand_variables(const char *str, t_ctx *ctx,
							t_token_type token_type);

int						count_args(char **args);
void					free_args(char **args);

void					read_heredoc(int fd, char *limiter);
int						here_doc(char *limiter);
void					handle_input_redirection(t_token *redir_token,
							int *redirect, int *redirect_input);
void					handle_output_redirection(t_token *redir_token,
							int *redirect, int *redirect_output);
void					initialize_pipe_if_needed(int *pipe_fd,
							t_token *cmd_end);
void					execute_command_in_child(t_token *cmd_start,
							t_token *cmd_end, int prev_fd, int *pipe_fd,
							char **env, t_ctx *ctx);
void					cleanup_parent_resources(int *prev_fd, int *pipe_fd,
							t_token **cmd_start, t_token *cmd_end);
void					wait_for_all_children(void);
void					setup_pipe_for_child(int prev_fd, int *pipe_fd,
							int redirect_input, int redirect_output,
							t_token *cmd_end);
void					collect_exec_tokens(t_token *cmd_start,
							t_token *cmd_end, t_token **exec_tokens,
							int *redirect, int *redirect_input,
							int *redirect_output);
// int loop_with_pipes(char **env, t_ctx *ctx);
// int						loop_with_pipes(t_env_var *env, t_ctx *ctx);
int loop_with_pipes(char **env, t_ctx *ctx);

t_pipe_cmd				*create_pipe_cmd(t_token *cmd_tokens);
int						execute_pipe_sequence(t_pipe_cmd *cmds, char **env,
							t_ctx *ctx);
t_token					*get_next_command(t_token *start);
char					*ft_strjoin_free(char *s1, char *s2, int free_str);
char					*ft_strjoin_char(char *str, char c);
void					execute_command_in_child(t_token *cmd_start,
							t_token *cmd_end, int prev_fd, int *pipe_fd,
							char **env, t_ctx *ctx);
void					handle_output_redirection(t_token *redir_token,
							int *redirect, int *redirect_output);
// void					ps_expand_env(t_token *tokens, t_ctx *ctx,
// 							t_env_var *myEnv);
void	ps_expand_env(t_token *tokens, t_ctx *ctx);
int	expand_variable(char *token, char **result, int i, t_ctx *ctx);
// int						expand_variable(char *token, char **result, int i,
// 							t_env_var *myEnv);
int						extract_var_name(char *var_start, char *var_name);
void					append_env_value(char **result, char *env_value);
void					*free_tab_2(char **tab);
int initialize_ctx(t_ctx *ctx);

int						is_builtin(char *cmd);

void free_ctx(t_ctx *ctx);
void free_env(t_env_var *env_var);

t_env_var *build_env_list(char **envp);
void print_env(t_ctx *ctx);

t_env_var *get_last_env_node(t_env_var **env);
t_ctx *get_ctx(void);
void print_export(t_ctx *ctx);
char *reconstruct_line(t_token *token_list);

#endif