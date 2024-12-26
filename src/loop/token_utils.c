/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 11:46:05 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/26 15:10:09 by ymanchon         ###   ########.fr       */
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
