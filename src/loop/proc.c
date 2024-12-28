/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 16:27:41 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 13:37:05 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_proc(t_token **tokens, t_ctx *ctx)
{
	t_token	*token;
	t_token	*next;

	if (!tokens || !*tokens)
		return (0);
	token = *tokens;
	while (token)
	{
		next = token->next;
		if ((token->type == 'S' || token->type == '"')
			&& ft_strchr(token->value, '$'))
		{
			if (!token->prev || token->prev->type != 'H')
			{
				if (expand_str(token, ctx) == -1)
					return (-1);
				if (token == *tokens && token->value && !*token->value)
				{
					*tokens = token->next;
					if (token->next)
						token->next->prev = NULL;
					free(token->value);
					free(token);
					token = *tokens;
					continue ;
				}
			}
		}
		token = next;
	}
	return (0);
}

int	quotes_proc(t_token **tokens, char *input, int *i)
{
	char	*value;
	char	type;
	int		j;

	type = input[(*i)++];
	j = (*i);
	while (input[(*i)] && input[(*i)] != type)
		(*i)++;
	if (input[(*i)++] != type)
		return (printf("error: syntax\n"), -1);
	value = ft_substr(input, j, *i - j - 1);
	if (!value)
		return (-1);
	if (add_token(tokens, type, value) != 0)
		return (free(value), -1);
	if (j > 1 && tokens && !ft_isspace(input[j - 2])
		&& !is_token(input[j - 2], UNJOIN))
		get_last_token(*tokens)->had_space = 1;
	free(value);
	return (0);
}

void	operators_proc_utils(t_token **tokens)
{
	(void)tokens;
}

int	operators_proc(t_token **tokens, char *input, int *i, int n)
{
	char	type;
	int		x;
	int		start;
	char	*value;
	int		ret;
	t_token	*last;

	start = *i;
	type = input[(*i)++];
	x = 1;
	while (input[*i] && input[*i] == type)
	{
		(*i)++;
		if (++x > n)
			return (printf("error: syntax3\n"), -1);
	}
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (-1);
	if (*tokens && is_token(get_last_token(*tokens)->type, OPERATORS))
	{
		last = get_last_token(*tokens);
		if (!((last->type == 'H' && type == '|')
				|| (last->type == '|' && type == '<' && x == 2)))
			return (free(value), printf("error: syntax2\n"),
				free_tokens(*tokens), -1);
	}
	if (type == '<' && x > 1)
		type = 'H';
	else if (type == '>' && x > 1)
		type = 'A';
	ret = add_token(tokens, type, value);
	free(value);
	if (ret)
		return (1);
	return (0);
}
//return (ret ? 1 : 0);

int	token_proc(t_token **tokens, char *input, int *i)
{
	if (input[*i] == '<' || input[*i] == '>' )
	{
		if (operators_proc(tokens, input, i, 2))
			return (-1);
	}
	else if (input[*i] == '|')
	{
		if (operators_proc(tokens, input, i, 1))
			return (-1);
	}
	else if (input[*i] == '"' || input[*i] == '\'')
	{
		if (quotes_proc(tokens, input, i))
			return (-1);
	}
	return (0);
}

int	word_proc(t_token **tokens, char *input, int *i)
{
	char	*value;
	int		j;
	int		result;

	j = *i;
	while (input[(*i)] && !ft_isspace(input[(*i)])
		&& !is_token(input[*i], TOKENS))
		(*i)++;
	if (j == *i)
		return (0);
	value = ft_substr(input, j, *i - j);
	if (!value)
		return (-1);
	result = add_token(tokens, 'S', value);
	free(value);
	if (result != 0)
	{
		free_tokens(*tokens);
		*tokens = NULL;
		return (-1);
	}
	if (j > 0 && input[j - 1] && !ft_isspace(input[j - 1])
		&& !is_token(input[j - 1], UNJOIN))
		get_last_token(*tokens)->had_space = 1;
	return (0);
}
