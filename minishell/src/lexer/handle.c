/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:10:28 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/25 18:03:43 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_quotes(char **line, int *in_quotes, char *quote_char, char *buffer,
		int *i, t_token **token_list)
{
	if (**line == '"' || **line == '\'')
	{
		if (*in_quotes && **line == *quote_char)
		{
			*in_quotes = 0;
			buffer[*i] = '\0';
			add_token(token_list, TOKEN_ARGUMENT, buffer);
			*i = 0;
		}
		else if (!*in_quotes)
		{
			*in_quotes = 1;
			*quote_char = **line;
		}
		(*line)++;
		return (1);
	}
	return (0);
}

int	handle_whitespace(char **line, char *buffer, int *i, t_token **token_list,
		int in_quotes)
{
	if (is_whitespace(**line) && !in_quotes)
	{
		if (*i > 0)
		{
			buffer[*i] = '\0';
			add_token(token_list, TOKEN_ARGUMENT, buffer);
			*i = 0;
		}
		(*line)++;
		return (1);
	}
	return (0);
}

int	handle_special_chars(char **line, char *buffer, int *i,
		t_token **token_list, int in_quotes)
{
	char	special[3];

	ft_bzero(special, sizeof(special));
	if ((**line == '|' || **line == '<' || **line == '>') && !in_quotes)
	{
		if (*i > 0)
		{
			buffer[*i] = '\0';
			add_token(token_list, TOKEN_ARGUMENT, buffer);
			*i = 0;
		}
		special[0] = **line;
		(*line)++;
		if ((special[0] == '>' && **line == '>') || (special[0] == '<'
				&& **line == '<'))
			special[1] = *(*line)++;
		add_token(token_list, get_token_type(special), special);
		return (1);
	}
	return (0);
}
