/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_struct_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:54:41 by llarrey           #+#    #+#             */
/*   Updated: 2024/12/22 12:05:45 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int	count_env_vars(t_env_var *env_vars)
// {
// 	int	count;

// 	count = 0;
// 	while (env_vars)
// 	{
// 		count++;
// 		env_vars = env_vars->next;
// 	}
// 	return (count);
// }

// char	*join_name_value(const char *name, const char *value)
// {
// 	char	*temp;
// 	char	*result;

// 	if (!name || !value)
// 		return (NULL);
// 	temp = ft_strjoin(name, "=");
// 	if (!temp)
// 		return (NULL);
// 	result = ft_strjoin(temp, value);
// 	free(temp);
// 	return (result);
// }

// char	**create_env_array(t_env_var *env_vars, int count)
// {
// 	char		**env_array;
// 	t_env_var	*current;
// 	int			i;

// 	env_array = malloc((count + 1) * sizeof(char *));
// 	current = env_vars;
// 	i = 0;
// 	if (!env_array)
// 		return (NULL);
// 	while (current)
// 	{
// 		env_array[i] = join_name_value(current->name, current->value);
// 		if (!env_array[i])
// 		{
// 			while (--i >= 0)
// 				free(env_array[i]);
// 			free(env_array);
// 			return (NULL);
// 		}
// 		i++;
// 		current = current->next;
// 	}
// 	env_array[i] = NULL;
// 	return (env_array);
// }

// char	**ctx_to_env_array(t_ctx *ctx)
// {
// 	int	count;

// 	if (!ctx || !ctx->env_vars)
// 		return (NULL);
// 	count = count_env_vars(ctx->env_vars);
// 	return (create_env_array(ctx->env_vars, count));
// }

// int	is_pipe_after_builtin(t_pipeline *pl)
// {
// 	t_token	*current;
// 	int		found_builtin;

// 	current = pl->cmd_start;
// 	found_builtin = 0;
// 	while (current)
// 	{
// 		if (is_builtin(current->value))
// 			found_builtin = 1;
// 		else if (found_builtin && ft_strcmp(current->value, "|") == 0)
// 			return (1);
// 		current = current->next;
// 	}
// 	return (0);
// }
