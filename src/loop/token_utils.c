/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 11:46:05 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/22 14:14:46 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_insert(t_token **at, t_token *insertion)
{
	t_token	*tmp;
	t_token	*tmp_prev;

	tmp = (*at)->next;
	(*at)->next = insertion;
	insertion->prev = *at;
	tmp_prev = insertion;
	insertion = insertion->next;
	while (insertion->next)
	{
		insertion->prev = tmp_prev;
		tmp_prev = insertion;
		insertion = insertion->next;
	}
	insertion->prev = tmp_prev;
	insertion->next = tmp;
}

t_token	*token_create(char *value)
{
	t_token	*ret;

	ret = (t_token *)malloc(sizeof(t_token));
	ft_memset(ret, 0, sizeof(ret));
	ret->value = value;
	ret->type = STRING;
	ret->next = NULL;
	return (ret);
}
