/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 11:46:05 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:35:00 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_token(char c, char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == c)
			return (true);
	}
	return (false);
}

// result_tmp[0] --> result
// result_tmp[1] --> tmp
char	*tokens_to_string(t_token *tokens)
{
	t_token	*current;
	char	*result_tmp[2];

	if (!tokens)
		return (NULL);
	current = tokens;
	result_tmp[0] = ft_strdup("");
	if (!result_tmp[0])
		return (NULL);
	while (current)
	{
		if (*result_tmp[0] && !current->had_space)
		{
			result_tmp[1] = ft_strfjoin(result_tmp[0], " ");
			if (!result_tmp[1])
				return (NULL);
			result_tmp[0] = result_tmp[1];
		}
		result_tmp[1] = ft_strfjoin(result_tmp[0], current->value);
		if (!result_tmp[1])
			return (NULL);
		result_tmp[0] = result_tmp[1];
		current = current->next;
	}
	return (result_tmp[0]);
}

t_token	*get_last_token(t_token *token_list)
{
	t_token	*current;

	if (!token_list)
		return (NULL);
	current = token_list;
	while (current->next)
		current = current->next;
	return (current);
}

t_token	*get_last_node(t_token *tokens)
{
	t_token	*tmp;

	if (!tokens)
		return (NULL);
	tmp = tokens;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	token_del(t_token *token)
{
	if (!token)
		return ;
	if (token->prev)
		token->prev->next = token->next;
	if (token->next)
		token->next->prev = token->prev;
	free(token->value);
	free(token->content);
	free(token);
}
