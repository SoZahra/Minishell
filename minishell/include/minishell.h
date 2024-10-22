/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:42:18 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/22 14:55:26 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <limits.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>

extern char **environ;

#define PROMPT "MiniBG> "

typedef enum token_type
{
    TOKEN_COMMAND,
    TOKEN_ARGUMENT,
    TOKEN_HEREDOC,
    TOKEN_REDIRECT_INPUT,   // <
    TOKEN_REDIRECT_OUTPUT,  // >
    TOKEN_REDIRECT_APPEND,  // >>
    TOKEN_PIPE,             // |
    TOKEN_FILENAME,         // Ajoute ce type pour les fichiers après redirection
    TOKEN_REDIRECT_HEREDOC  // <<
}	t_token_type;

typedef struct s_token
{
	char	*value;
	t_token_type type;
    struct s_token *next;
}	t_token;

/// read
// void loop(void);
void loop(char **env);
void process_tokens(t_token *cmd_tokens, int num_pipes);
// char **prepare_args(t_token *tokens);
int check_consecutive_pipes(t_token *tokens);
int contains_pipe(t_token *tokens);

/// signal

void handle_sigint(int sig);

///parsing

void ft_cd(char **args);

/// lexer

t_token	*create_token(t_token_type type, const char *value);
t_token *lexer(const char *input);
void add_token(t_token **head, t_token_type type, const char *value);
t_token_type get_token_type(const char *str);
int is_whitespace(char c);
t_token *parse_command_line(char *line);

///free

void	free_tokens(t_token *tokens);

///main

void print_tokens(t_token *tokens);

//exc_pipe

// int validate_pipe_syntax(t_token *tokens);
// int valide_pipes(t_token *tokens);
// void exc_error(t_token *tokens);
// void exc_pipe(t_token *tokens);

//pipex

void	exec(t_token *cmd_tokens, char **env);
void	child(t_token *tokens, int *pipe_fd, char **env);
void	parent(t_token *tokens, int *pipe_fd, char **env);
size_t	ft_strlen(const char *c);
char	*my_getenv(char *name, char **env);
char	*get_path(char *cmd, char **env);
char	*join_path_cmd(char *path, char *cmd);
char	*find_in_env(char *name, char **env);
void	*free_tab(char **tab);
void	exit_error(void);
char **prepare_args(t_token *tokens);
t_token *extract_command(t_token *tokens);
t_token *extract_command_after(t_token *tokens);
int process_pline(t_token *tokens, char **env);
char **get_environment(char **envp);

///free

void *free_tab_2(char **tab);

#endif