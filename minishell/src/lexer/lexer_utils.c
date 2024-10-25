/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:41:00 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/25 18:09:23 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	handle_space(char **ptr)
{
	if (**ptr == ' ')
	{
		(*ptr)++;
		return (1);
	}
	return (0);
}

t_token	*create_token_from_pipe(t_token **head, t_token **tail)
{
	return (add_pipe_token(head, tail));
}
