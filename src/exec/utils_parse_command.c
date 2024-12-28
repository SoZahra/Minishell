/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:13:51 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:12:16 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_valid_var_name(const char *name)
{
	int	i;

	i = 1;
	if (!name || !*name || ft_isdigit(*name))
		return (0);
	if (!ft_isalpha(*name) && *name != '_')
		return (0);
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
