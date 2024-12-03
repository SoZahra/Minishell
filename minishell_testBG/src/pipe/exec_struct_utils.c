/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_struct_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:54:41 by llarrey           #+#    #+#             */
/*   Updated: 2024/12/03 17:23:44 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int count_env_vars(t_env_var *env_vars)
{
	int count;

	count = 0;
	while (env_vars)
	{
		count++;
		env_vars = env_vars->next;
	}
	return count;
}

char **create_env_array(t_env_var *env_vars, int count)
{
	char **env_array;
	t_env_var *current;
	int i;
	size_t len;
	
	env_array = malloc((count + 1) * sizeof(char *));
	current = env_vars;
	i = 0;
	if (!env_array)
		return NULL;
	while (current)
	{
		len = strlen(current->name) + strlen(current->value) + 2;
		env_array[i] = malloc(len);
		if (!env_array[i])
		{
			while (--i >= 0)
				free(env_array[i]);
			free(env_array);
			return NULL;
		}
		sprintf(env_array[i], "%s=%s", current->name, current->value);
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return env_array;
}

char **ctx_to_env_array(t_ctx *ctx)
{
	int	count;
	
	if (!ctx || !ctx->env_vars)
		return NULL;
	count = count_env_vars(ctx->env_vars);
	return create_env_array(ctx->env_vars, count);
}
