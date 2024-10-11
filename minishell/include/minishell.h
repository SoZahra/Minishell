/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:42:18 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/11 17:50:29 by fzayani          ###   ########.fr       */
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


#define PROMPT "MiniBG🌝> "

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
void loop(void);
void process_tokens(t_token *tokens);

/// signal

void handle_sigint(int sig);

///parsing


/// lexer
// t_token	*create_token(const char *value);
t_token	*create_token(t_token_type type, const char *value);
t_token *lexer(const char *input);

///lexer_utils


///free

void	free_tokens(t_token *tokens);

///main

void print_tokens(t_token *tokens);

#endif
