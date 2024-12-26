/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:52:59 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/26 15:53:12 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_valid_n(t_token *current)
{
	int	i;

	i = 0;
	while (current->value[i])
	{
		if (current->value[0] != '-')
			return (0);
		if (i != 0 && current->value[i] != 'n')
			return (0);
		i++;
	}
	if (current->value[0] == '-')
		return (1);
	return (0);
}
