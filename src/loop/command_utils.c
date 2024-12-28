/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 15:16:10 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/28 15:16:39 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*find_next_command(t_token *current)
{
	while (current && current->type != '|')
		current = current->next;
	if (!current)
		return (NULL);
	return (current->next);
}

void	count_tokens(t_token *start, t_token *end, int *arg_c, int *red_c)
{
	t_token	*curr;

	curr = start;
	*arg_c = 0;
	*red_c = 0;
	while (curr && curr != end && curr->type != '|')
	{
		if (is_redirection(curr->type))
		{
			(*red_c)++;
			curr = curr->next;
		}
		else if (is_argument(curr->type))
			(*arg_c)++;
		if (curr)
			curr = curr->next;
	}
}
