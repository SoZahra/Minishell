/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:38:30 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 16:42:19 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_token_node(char *arg, t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		perror("malloc failed");
		return (NULL);
	}
	new_token->value = ft_strdup(arg);
	if (!new_token->value)
	{
		free(new_token);
		perror("ft_strdup failed");
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	new_token->quoted = (type == SINGLE_QUOTE || type == DOUBLEQUOTE);
	new_token->had_space = 0;
	return (new_token);
}
