/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:27:34 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:28:54 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_env_vars(t_env_var *env_vars)
{
	int			count;
	t_env_var	*current;

	count = 0;
	current = env_vars;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(char *name, char *value)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(name, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, value);
	free(temp);
	return (result);
}

static char	**fill_env_array(t_env_var *env_vars, int count)
{
	char		**env;
	t_env_var	*current;
	int			i;

	env = malloc(sizeof(char *) * (count + 1));
	if (!env)
		return (NULL);
	current = env_vars;
	i = 0;
	while (current)
	{
		env[i] = create_env_string(current->name, current->value);
		if (!env[i])
			return (free_array(env), NULL);
		current = current->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

char	**create_env_array(t_env_var *env_vars)
{
	int	count;

	count = count_env_vars(env_vars);
	return (fill_env_array(env_vars, count));
}
