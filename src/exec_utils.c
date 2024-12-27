/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:59:51 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 15:00:17 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_env_array(t_env_var *env_vars)
{
	int			count;
	t_env_var	*current;
	char		**env;
	char		*temp;

	count = 0;
	current = env_vars;
	while (current)
	{
		count++;
		current = current->next;
	}
	env = malloc(sizeof(char *) * (count + 1));
	if (!env)
		return (NULL);
	current = env_vars;
	count = 0;
	while (current)
	{
		temp = ft_strjoin(current->name, "=");
		env[count] = ft_strjoin(temp, current->value);
		free(temp);
		if (!env[count])
		{
			free_array(env);
			return (NULL);
		}
		current = current->next;
		count++;
	}
	env[count] = NULL;
	return (env);
}

char	*tokens_to_string_from_command(t_command *cmd)
{
	char	*temp;
	char	*result;
	int		i;

	if (!cmd->args || cmd->arg_count == 0)
		return (ft_strdup("\0"));
	result = ft_strdup(cmd->args[0]);
	if (!result)
		return (NULL);
	i = 1;
	while (i < cmd->arg_count)
	{
		if (!cmd->had_spaces[i])
		{
			temp = ft_strjoin(result, " ");
			if (!temp)
			{
				free_command_list(cmd);
				return (NULL);
			}
			free(result);
			result = temp;
		}
		temp = ft_strjoin(result, cmd->args[i]);
		if (!temp)
		{
			free_command_list(cmd);
			return (NULL);
		}
		free(result);
		result = temp;
		++i;
	}
	return (result);
}
