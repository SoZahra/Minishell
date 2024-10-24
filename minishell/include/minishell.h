/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:42:18 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/24 15:00:11 by fzayani          ###   ########.fr       */
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

extern char			**environ;

# define PROMPT "MiniBG> "

typedef enum token_type
{
	TOKEN_COMMAND,
	TOKEN_ARGUMENT,
	TOKEN_HEREDOC,
	TOKEN_REDIRECT_INPUT,  // <
	TOKEN_REDIRECT_OUTPUT, // >
	TOKEN_REDIRECT_APPEND, // >>
	TOKEN_PIPE,            // |
	TOKEN_FILENAME,        // Ajoute ce type pour les fichiers après redirection
	TOKEN_REDIRECT_HEREDOC // <<
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

/// read
// void				loop(char **env);
// void				process_tokens(t_token *cmd_tokens, int num_pipes);
int					check_consecutive_pipes(t_token *tokens);
int					contains_pipe(t_token *tokens);

void				print_cmd(t_token *cmd, int index);
void				print_cmds(t_token *cmd_tokens, int num_pipes);
void				create_pipe(int pipe_fd[2]);
char				**prepare_print_args(t_token *cmd);
void				exec_cmd(t_token *cmd, int fd_in, int pipe_fd[2],
						int last_cmd);
void				handle_child(t_token *cmd_tokens, int fd_in, int pipe_fd[2],
						int last_cmd);
void				handle_parent(int pipe_fd[2], int *fd_in, pid_t pid);
void				process_tokens(t_token *cmd_tokens, int num_pipes);

void				handle_line(char *line, char **env);
void				exec_simple_cmd(t_token *tokens, char **env);
void				read_and_exec(char **env);
void				loop(char **env);

/// signal

void				handle_sigint(int sig);
void				handle_sigquit(int sig);
void				init_sig(void);

/// parsing

void				ft_cd(char **args);

/// lexer

t_token				*create_token(t_token_type type, const char *value);
t_token				*lexer(const char *input);
void				add_token(t_token **head, t_token_type type,
						const char *value);
t_token_type		get_token_type(const char *str);
int					is_whitespace(char c);
t_token				*parse_command_line(char *line);

/// free

void				free_tokens(t_token *tokens);

/// main

void				print_tokens(t_token *tokens);

// exc_pipe

// int validate_pipe_syntax(t_token *tokens);
// int valide_pipes(t_token *tokens);
// void exc_error(t_token *tokens);
// void exc_pipe(t_token *tokens);

// pipex

void				exec(t_token *cmd_tokens, char **env);
void				child(t_token *tokens, int *pipe_fd, char **env);
void				parent(t_token *tokens, int *pipe_fd, char **env);
size_t				ft_strlen(const char *c);
char				*my_getenv(char *name, char **env);
char				*get_path(char *cmd, char **env);
char				*join_path_cmd(char *path, char *cmd);
char				*find_in_env(char *name, char **env);
void				*free_tab(char **tab);
void				exit_error(void);
char				**prepare_args(t_token *tokens);
t_token				*extract_command(t_token *tokens);
t_token				*extract_command_after(t_token *tokens);
int					process_pline(t_token *tokens, char **env);
char				**get_environment(char **envp);

///pipe_utils_2

int process_pline(t_token *tokens, char **env);

/// free

void				*free_tab_2(char **tab);

#endif