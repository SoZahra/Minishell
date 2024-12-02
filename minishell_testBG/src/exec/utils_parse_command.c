/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:10:05 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 19:04:45 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_args(char **args)
{
	int	count;

	count = 0;
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

// int	is_valid_id(const char *var)
// {
// 	char	*equal_sign;
// 	size_t	len;

// 	equal_sign = ft_strchr(var, '=');
// 	len = equal_sign ? (size_t)(equal_sign - var) : ft_strlen(var);
// 	if (!var || !*var)
// 		return (0);
// 	if (!ft_isalpha(var[0]) && var[0] != '_')
// 		return (0);
// 	for (size_t i = 1; i < len; i++)
// 	{
// 		if (!ft_isalnum(var[i]) && var[i] != '_')
// 			return (0);
// 	}
// 	return (1);
// }

int	is_valid_id(const char *var)
{
	char	*equal_sign;
	size_t	len;
	size_t	i;

	if (!var || !*var)
		return (0);
	equal_sign = ft_strchr(var, '=');
	if (equal_sign)
		len = (size_t)(equal_sign - var);
	else
		len = ft_strlen(var);
	if (!ft_isalpha(var[0]) && var[0] != '_')
		return (0);
	i = 1;
	while (i < len)
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	split_env_v(char *arg, char **var, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		*var = ft_strdup(arg);
		*value = NULL;
		return (*var != NULL);
	}
	*var = ft_strndup(arg, equal_sign - arg);
	*value = ft_strdup(equal_sign + 1);
	if (!*var || (equal_sign && !*value))
	{
		free(*var);
		free(*value);
		return (0);
	}
	return (1);
}
