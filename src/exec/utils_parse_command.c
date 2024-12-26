/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:13:51 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/26 16:16:00 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_args(char **args)
{
	int	count;

	count = 0;
	if (args)
		while (args[count])
			count++;
	return (count);
}

t_env_var	*get_last_env_node(t_env_var **env)
{
	t_env_var	*tmp;

	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

int	is_valid_id(const char *var)
{
	size_t	i;

	if (!var || !*var)
		return (0);
	if (!ft_isalpha(var[0]) && var[0] != '_')
		return (0);
	i = 1;
	while (var[i] && var[i] != '=')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	if (i == 0 || (var[0] == '=' && i == 1))
		return (0);
	return (1);
}

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
