/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 16:30:48 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 16:31:06 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	join_str(t_token *token, bool limiter)
{
	t_token	*prev;

	if (!token || !token->prev)
		return (0);
	prev = token->prev;
	if (limiter)
	{
		if (!prev->prev || prev->prev->type != 'H')
			return (0);
		prev = prev->prev;
	}
	if (join_tokens(prev, token) == -1)
		return (-1);
	token_del(token);
	return (0);
}

int	join_tokens(t_token *prev, t_token *current)
{
	char	*tmp;
	char	*joined;

	if (!prev || !current || !prev->value || !current->value)
		return (-1);
	tmp = prev->value;
	joined = ft_strjoin(tmp, current->value);
	if (!joined)
		return (-1);
	prev->value = joined;
	free(tmp);
	if (current->type == '"' || current->type == '\'')
		prev->type = current->type;

	return (0);
}

t_token	*join_it(t_token *token)
{
	char	*tmp;
	t_token	*prev;

	if (!token)
		return (NULL);
	if (token->prev && token->prev->prev && token->prev->prev->type == 'H')
	{
		tmp = token->prev->value;
		token->prev->value = ft_strjoin(tmp, token->value);
		if (!token->prev->value)
		{
			token->prev->value = tmp;
			return (NULL);
		}
		free(tmp);
		token->prev->next = token->next;
		if (token->next)
			token->next->prev = token->prev;
		free(token->value);
		prev = token->prev;
		return (free(token), prev);
	}
	return (NULL);
}

int	_join_limiter(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->had_space)
		{
			tmp = join_it(tmp);
			if (!tmp)
				return (-1);
		}
		tmp = tmp->next;
	}
	return (0);
}
