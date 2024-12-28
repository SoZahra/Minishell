/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:59:18 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/28 15:04:15 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	operators_proc_utils(t_token **tokens)
{
	(void)tokens;
}

int	check_operator_sequence(t_token **tokens, char type, int x)
{
	t_token	*last;

	if (*tokens && is_token(get_last_token(*tokens)->type, OPERATORS))
	{
		last = get_last_token(*tokens);
		if (!((last->type == 'H' && type == '|') || (last->type == '|'
					&& type == '<' && x == 2)))
			return (1);
	}
	return (0);
}

char	get_final_type(char type, int x)
{
	if (type == '<' && x > 1)
		return ('H');
	if (type == '>' && x > 1)
		return ('A');
	return (type);
}

int	handle_empty_token(t_token **tokens, t_token *token)
{
	if (token == *tokens && token->value && !*token->value)
	{
		*tokens = token->next;
		if (token->next)
			token->next->prev = NULL;
		free(token->value);
		free(token);
		return (1);
	}
	return (0);
}

int	process_expandable_token(t_token **tokens, t_token *token, t_ctx *ctx)
{
	if ((token->type == 'S' || token->type == '"') && ft_strchr(token->value,
			'$'))
	{
		if (!token->prev || token->prev->type != 'H')
		{
			if (expand_str(token, ctx) == -1)
				return (-1);
			if (handle_empty_token(tokens, token))
				return (1);
		}
	}
	return (0);
}
