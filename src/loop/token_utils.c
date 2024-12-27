/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 11:46:05 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 14:54:27 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_insert(t_token **at, t_token *insertion)
{
	t_token	*tmp;
	t_token	*tmp_prev;

	if (!insertion || !at || !(*at))
		return ;
	tmp = (*at)->next;
	(*at)->next = insertion;
	insertion->prev = *at;
	tmp_prev = insertion;
	//insertion = insertion->next;
	while (insertion->next)
	{
		insertion = insertion->next;
		insertion->prev = tmp_prev;
		tmp_prev = insertion;
		//insertion = insertion->next;
	}
	insertion->prev = tmp_prev;
	insertion->next = tmp;
}

void	print_tokens(t_token *tokens)
{
	t_token	*tmp;

	if (!tokens)
		return ;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->value)
			printf("value: [%s] \t type: %c  \t flag: %d\n",
				tmp->value,
				tmp->type,
				tmp->had_space);
		tmp = tmp->next;
	}
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

t_token	*get_next_pipe_token(t_token *start) 
{
	while (start && start->type != '|')
		start = start->next;
	return (start);
}
