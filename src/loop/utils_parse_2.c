/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:13:19 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/27 16:43:33 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*find_env_value(const char *name, t_env_var *env_vars)
{
	while (env_vars)
	{
		if (ft_strcmp(env_vars->name, name) == 0)
			return (env_vars->value);
		env_vars = env_vars->next;
	}
	return (NULL);
}
