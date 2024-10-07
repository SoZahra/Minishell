/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:42:18 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/07 16:12:02 by fzayani          ###   ########.fr       */
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


typedef enum token_type
{
	T_WORD,
    T_PIPE,
    T_REDIRECT_IN,
    T_REDIRECT_OUT,
    T_APPEND_OUT,
    T_HEREDOC,
    T_ENV_VAR,
    T_SINGLE_QUOTE,
    T_DOUBLE_QUOTE,
    T_WHITESPACE,
    T_END
}	t_token_type;

typedef struct s_token
{
	char	*value;
	t_token_type type;
}	t_token;

/// read
void loop(void);

/// signal

void handle_sigint(int sig);

///parsing

int c_tokens(const char *input);
int toggle_quote(int in_quote, char quote_char, char current_char);
int count_tokens(const char *input);
char *copy_token(const char *start, const char *end);
t_token *parse_command(const char *input);
t_token_type identify_token(const char *str);
void process_tokens(t_token *tokens);

/// lexer
t_token *lexer(const char *input);

///free

void ft_free(void);


#endif