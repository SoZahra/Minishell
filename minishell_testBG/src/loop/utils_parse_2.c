/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:13:19 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/03 10:08:43 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

char	*find_env_value(const char *name, t_env_var *env_vars)
{
	while (env_vars)
	{
		if (strcmp(env_vars->name, name) == 0)
			return (env_vars->value);
		env_vars = env_vars->next;
	}
	return (NULL);
}
