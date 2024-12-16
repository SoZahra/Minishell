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
# include <termios.h>
# include <stdbool.h>

extern char				**environ;

# define PROMPT "\033[1;34mMiniBG>\033[0m "
#define TOKENS "<>|\"'"
#define OPERATORS "AH<>|"
#define UNJOIN "<>|"

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

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
	struct termios		term;
	// char				*name;
	// char				*value;

}						t_ctx;

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
	struct s_token		*prev;
	char				*content;
	int					quoted;
	int					had_space;
}						t_token;

// typedef struct s_pipe_cmd
// {
// 	t_token				*cmd;
// 	int					input_fd;
// 	int					output_fd;
// 	int					pipe_read;
// 	int					pipe_write;
// 	struct s_pipe_cmd	*next;
// }						t_pipe_cmd;

typedef struct s_redirection {
    char type;              // '>' pour >, 'A' pour >>, '<' pour <, 'H' pour
    char *file;            // Nom du fichier ou délimiteur
    int fd;                // File descriptor
    struct s_redirection *next;
} t_redirection;

typedef struct s_command {
	pid_t pid;
    char **args;           // Les arguments de la commande
	char *path;
    t_redirection *redirs; // Liste des redirections
	int pfd[2];
    // int pipe_in;           // FD pour le pipe d'entrée (si existe)
    // int pipe_out;          // FD pour le pipe de sortie (si existe)
	struct s_command *next;     // Commande suivante dans le pipe
    struct s_command *prev;
	int			*had_spaces;
	int			arg_count;
} t_command;

typedef struct s_pipeline {
    int prev_fd;
    int pipe_fd[2];
    t_token *cmd_start;
    t_token *cmd_end;
	char	*cmd_line;
} t_pipeline;

typedef struct s_redir {
    int input;
    int output;
} t_redir;

// pipe(pfd);

t_token *find_pipe_token(t_token *start);
void execute_pipeline(t_command *cmd, t_ctx *ctx);
int prepare_command(t_command *cmd, t_ctx *ctx);
int execute_piped_command(t_command *cmd, t_ctx *ctx);
t_command *create_command_from_tokens_range(t_token *start, t_token *end);

// -------------------------------------------------------------

int tokenizer(t_token **tokens, char *input);
int is_builtin(const char *cmd);
char *expand_full_string(const char *str, char quote_type, t_ctx *ctx);

char *tokens_to_string(t_token *tokens);
char *tokens_to_string_from_command(t_command *cmd);
// char *prepare_command(t_token *tokens, t_ctx *ctx);
int join_proc(t_token **tokens, bool limiter);
int join_tokens(t_token *prev, t_token *current);
void token_del(t_token *token);
int join_str(t_token *token, bool limiter);
int expand_proc(t_token **tokens, t_ctx *ctx);
int expand_str(t_token *token, t_ctx *ctx);
char *args_to_string(t_command *cmd);
// int execute_builtin(char **args, t_ctx *ctx);
// int execute_builtin(const char *cmd_line, t_ctx *ctx);

//----------------------------------------------------------------

// export
int handle_export_builtin(const char *input, t_ctx *ctx);
// int process_export_args(char **args, t_ctx *ctx);
int handle_with_equal_sign(char *arg, char *equal_sign, t_ctx *ctx);
int handle_no_equal_sign(char *arg, t_ctx *ctx);
int handle_error(const char *arg, t_ctx *ctx);
int handle_no_args(t_ctx *ctx);
int handle_multiple_args(const char *args, t_ctx *ctx);

//echo
int handle_echo_builtin(const char *input, t_ctx *ctx);

//env
int handle_env_builtin(const char *input, t_ctx *ctx);

//cd
int handle_cd_builtin(const char *input, t_ctx *ctx);
void free_array(char **array);

//pwd
int handle_pwd_builtin(const char *input, t_ctx *ctx);

//exit
int handle_exit_builtin(const char *input, t_ctx *ctx);
int process_exit_arg(char **args, t_ctx *ctx);


//exec

char	**create_env_array(t_env_var *env_vars, int count);
char *find_command_path(const char *cmd, t_ctx *ctx);
char *get_env_path(t_env_var *env_vars);
char **create_command_array(const char *cmd_str);

//redir sortantes
// t_redir *get_redirections(char **cmd_array);
// int apply_redirections(t_redir *redirs);
// void free_redirections(t_redir *redirs);
// void print_redirections(t_redir *redirs);

t_command *create_command(t_token *tokens, t_ctx *ctx);
void	execute_command(char **args, char *path, t_ctx *ctx);
int apply_redirections(t_redirection *redirs);
void restore_fds(int stdin_fd, int stdout_fd);
void execute_builtin_command(t_command *cmd, t_ctx *ctx);
void execute_external_command(t_command *cmd, t_ctx *ctx);
int add_redirection(t_redirection **redirs, char type, char *file);
void free_command(t_command *cmd);

t_token *prepare_tokens(t_token *tokens, t_ctx *ctx);
t_command *create_command_from_tokens(t_token *tokens);
int add_arg_to_command(t_command *cmd, char *arg);
int handle_line_for_loop(char *line, t_ctx *ctx);
int append_arg_value(char **current_arg, const char *value, int had_space);


// -------------------------------------------------------------

void print_tokens(t_token *tokens);
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
int is_valid_var_name(const char *name);
// int						is_valid_id(const char *var);
// void	write_echo_content(t_token *token_list, int n_option);
void					write_echo_content(t_token *token_list, int n_option);
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
// t_token					*parse_command_line(char *line, t_ctx *ctx);
t_token *parse_command_line(char *line, t_ctx *ctx);
t_token					*add_pipe_token(t_token **head, t_token **tail);
// int	process_token(t_token **head, t_token **tail, char *start, char *ptr,
// 		int first_token);
// void					process_tokens(t_token *cmd_tokens, int num_pipes,
// 							t_ctx *ctx);
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
int exec_simple_cmd(t_token *tokens,  t_ctx *ctx);
// void split_env_v(const char *input, char **var, char **value);
// int exec_builtin_cmd(char **args, char **env);
int						split_env_v(char *arg, char **var, char **value);
// int exec_builtin_cmd(char **args, char **env, t_ctx *ctx);
// int						exec_builtin_cmd(char **args, t_env_var *env, t_ctx *ctx);
int exec_builtin_cmd(char **args, char **env, t_ctx *ctx);
t_token	*create_token_list(char **args, t_token_type type);
// int read_and_exec(char **env);
int						read_and_exec(char **env, t_ctx *ctx);
char					*strip_quotes(char *arg);
int						is_numeric_argument(const char *arg);
// int						process_exit_arg(char **args, t_ctx *ctx);
void					handle_exit_command(char *line, t_ctx *ctx);
// int	loop(char **env);
int						loop(char **env, t_ctx *ctx);
int						validate_pipe_syntax(t_token *tokens);
int						validate_pipes(t_token *tokens);
int						exc_error(t_token *tokens);
int						exc_pipe(t_token *tokens);
t_token					*extract_command(t_token *tokens);
t_token					*extract_command_after(t_token *tokens);

void execute_builtin_with_redirection(t_token *cmd_start, t_token *cmd_end, int *prev_fd, int *pipe_fd, t_ctx *ctx);
int process_pline(t_token *tokens, t_ctx *ctx);
void					*free_tab(char **tab);
// char					*find_in_env(char *name, char **env);
char *find_in_env_array(char *name, char **env);
char	*find_in_env(char *name, char **env);
char					*join_path_cmd(char *path, char *cmd);
char					*get_path(char *cmd, char **env);
// char **prepare_args(t_token *tokens, int *exit_status);
char					**prepare_args(t_token *tokens, t_ctx *ctx);
int						exec(t_token *cmd_tokens, t_ctx *ctx);
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
void 					handle_input_redirection(t_token *redir_token, t_redir *redir);
void 					handle_output_redirection(t_token *redir_token, t_redir *redir);
void					setup_redirects(int prev_fd, int *pipe_fd, t_token *cmd_end, t_redir *redir);
void					process_single_builtin(t_pipeline *pl, t_ctx *ctx);
void					execute_in_child(t_pipeline *pl, t_ctx *ctx);
void					process_pipeline_segment(t_pipeline *pl, t_ctx *ctx);
void					process_pipeline_stage(t_pipeline *pl, t_ctx *ctx);
void					close_pipe(t_pipeline *pl);
void					wait_for_children(void);
void					prepare_child_execution(t_pipeline *pl, t_ctx *ctx);
void					setup_pipe(t_pipeline *pl);
void					initialize_pipe_if_needed(int *pipe_fd,
							t_token *cmd_end);
void					collect_exec_tokens(t_token *cmd_start, t_token *cmd_end,
							t_token **exec_tokens, t_redir **redir);
int 					count_env_vars(t_env_var *env_vars);
int						check_for_pipe(t_pipeline *pl);
void 					setup_redirects_single_builtin(t_pipeline *pl, t_ctx *ctx);
int 					check_for_pipe_builtin(t_pipeline *pl);
void 					adjust_cmd_line_to_builtin(t_pipeline *pl);
char 					*reconstruct_cmd_line_from_token(t_token *start_token);
t_token 				*find_builtin_start(t_token *tokens);
void					apply_redirect_and_execute(t_pipeline *pl, t_ctx *ctx, int saved_stdin,
							int saved_stdout);
void					process_redirections(t_token *current);
void					handle_redirection(t_token *current);
int						save_and_restore_fds(int *saved_stdin, int *saved_stdout, int restore);
int						is_pipe_after_builtin(t_pipeline *pl);
int						has_redirect(t_pipeline *pl);
// char **create_env_array(t_env_var *env_vars, int count);
char **ctx_to_env_array(t_ctx *ctx);

// int loop_with_pipes(char **env, t_ctx *ctx);
// int						loop_with_pipes(t_env_var *env, t_ctx *ctx);
int loop_with_pipes(t_ctx *ctx);

// t_pipe_cmd				*create_pipe_cmd(t_token *cmd_tokens);
// int						execute_pipe_sequence(t_pipe_cmd *cmds, char **env,
							// t_ctx *ctx);
t_token					*get_next_command(t_token *start);
char					*ft_strjoin_free(char *s1, char *s2, int free_str);
char					*ft_strjoin_char(char *str, char c);
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

// int						is_builtin(char *cmd);

void free_ctx(t_ctx *ctx);
void free_env(t_env_var *env_var);

t_env_var *build_env_list(char **envp);
void print_env(t_ctx *ctx);

t_env_var *get_last_env_node(t_env_var **env);
t_ctx *get_ctx(void);
void print_export(t_ctx *ctx);
char *reconstruct_line(t_token *token_list);

t_token *create_single_token(char *value, t_token_type type);
t_token *handle_special_cases(const char *line, t_ctx *ctx);
char *clean_dollar_quotes(const char *str);
char **convert_env_to_array(t_ctx *ctx);
int check_invalid_quotes(char *line);

// int	handle_exit_builtin(char **args, t_ctx *ctx);
// int	handle_echo_builtin(char **args, t_ctx *ctx);
// int	handle_export_builtin(char **args, t_ctx *ctx);
int	handle_export_loop(char **args, t_ctx *ctx);


void	add_env_var_to_list(t_env_var **head, t_env_var *new_var);

int handle_export_loop(char **args, t_ctx *ctx);
// int	handle_export_builtin(char **args, t_ctx *ctx);
// int	handle_echo_builtin(char **args, t_ctx *ctx);
//
int process_var_assignment(char *arg, t_ctx *ctx);
int handle_special_case(char *arg, t_ctx *ctx);
// int	handle_cd_builtin(char **args, t_ctx *ctx);
int	handle_invalid_identifier(char *arg, char *var, char *value);
int	create_and_add_var(t_ctx *ctx, char *var, char *value);

int	execute_builtin(const char *cmd_line, t_ctx *ctx);
int	validate_args(char **args, t_ctx *ctx);
void	print_command_not_found(const char *cmd, t_ctx *ctx);


char *handle_dollar(const char *str, t_ctx *ctx, const char **str_ptr, char *expanded);
char *expand_exit_code(t_ctx *ctx, char *expanded);
char *expand_env_variable(const char **str_ptr, t_ctx *ctx, char *expanded);
char *append_literal_dollar(char *expanded);
char *handle_non_dollar(const char *str, const char **str_ptr, char *expanded);


// void	handle_line_for_loop(char *line, t_ctx *ctx);

// void	handle_unclosed_quote(char *temp, t_token *token_list);
// t_token_type	update_quote_type(char current_quote, int *i, char *line);
// void	add_combined_token(char *temp, char *buffer, t_token **token_list, t_token_type type);
// void	handle_quotes_4parse(char *line, int *i, char *buffer, t_token **token_list, t_token_type *type);
// void	handle_whitespace_4parse(char *buffer, int *j, t_token **token_list);

t_token_type get_quote_type(char quote);
void handle_empty_quotes(char **line, int *i, t_token **token_list, char *current_quote_type);

// t_token_type get_quote_type(char quote);
// void handle_empty_quotes(char **line, int *i, t_token **token_list, char *current_quote_type);
// int handle_quotes_content_(char **line, int *i, int *j, char *buffer, char quote_type);
// void handle_token_expansion(t_token *token_list, t_ctx *ctx);
// void handle_quoted_content(char *buffer, int *j, char *temp, t_token **list, t_token_type type);
// int handle_echo_dollar(char *line, int i, t_token **list);
// t_token *parse_tokens(char *line, t_ctx *ctx);
// int parse_line(int *i, int *j, char *line, t_token **list, char **temp, char *quote_type);
// int handle_quotes_(char *line, int *i, int *j, t_token **list,
//                  char **temp, char *quote_type);
// void add_remaining_buffer(t_token **list, int j, char *buffer);
// void handle_quote_end(int *i, char *quote_type, char **temp);
// int handle_temp_buffer(int *j, char *temp, char *buffer);

int count_tokens(t_token *tokens);

t_token *create_new_token(t_token_type type, const char *value);
t_token *add_token(t_token **token_list, t_token_type type, const char *value);

t_token *create_token_node(char *arg, t_token_type type);
int is_valid_n(t_token *current);

int set_term_attr();
int get_term_attr();

#endif