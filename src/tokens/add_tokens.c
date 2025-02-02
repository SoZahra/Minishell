/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:38:19 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:13:57 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_new_token(t_token_type type, char *value)
{
	t_token	*new_token;

	if (!value)
		return (NULL);
	new_token = malloc(sizeof(t_token));
	*new_token = (t_token){0};
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	return (new_token);
}

int	add_token(t_token **tokens, t_token_type type, char *value)
{
	t_token	*new_token;
	t_token	*last;

	if (!tokens || !value)
		return (-1);
	new_token = create_new_token(type, value);
	if (!new_token)
		return (-1);
	if (!*tokens)
	{
		*tokens = new_token;
		return (0);
	}
	last = *tokens;
	while (last->next)
		last = last->next;
	last->next = new_token;
	new_token->prev = last;
	return (0);
}
