/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:28:23 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:28:55 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	join_with_space(char **result)
{
	char	*temp;

	temp = ft_strjoin(*result, " ");
	if (!temp)
		return (1);
	free(*result);
	*result = temp;
	return (0);
}

static int	join_with_arg(char **result, char *arg)
{
	char	*temp;

	temp = ft_strjoin(*result, arg);
	if (!temp)
		return (1);
	free(*result);
	*result = temp;
	return (0);
}

static char	*process_args(t_command *cmd, char *result)
{
	int	i;

	i = 1;
	while (i < cmd->arg_count)
	{
		if (!cmd->had_spaces[i])
		{
			if (join_with_space(&result))
				return (NULL);
		}
		if (join_with_arg(&result, cmd->args[i]))
			return (NULL);
		i++;
	}
	return (result);
}

char	*tokens_to_string_from_command(t_command *cmd)
{
	char	*result;

	if (!cmd->args || cmd->arg_count == 0)
		return (ft_strdup("\0"));
	result = ft_strdup(cmd->args[0]);
	if (!result)
		return (NULL);
	return (process_args(cmd, result));
}
