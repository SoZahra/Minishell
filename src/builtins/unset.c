/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:08:18 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/26 15:18:35 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_unset_builtin(const char *input, t_ctx *ctx)
{
	char	**split_args;
	int		i;

	while (*input == ' ')
		input++;
	if (!*input)
		return (0);
	split_args = ft_split(input, ' ');
	if (!split_args)
		return (perror("ft_split"), 1);
	i = 0;
	while (split_args[i])
	{
		if (!is_valid_var_name(split_args[i]))
		{
			ft_fprintf(2, "MiniBG: unset: `%s': not a valid identifier\n",
				split_args[i]);
			ctx->exit_status = 1;
			continue ;
		}
		if (unset_v(&ctx->env_vars, split_args[i]) != 0)
			ctx->exit_status = 0;
		i++;
	}
	return (free_array(split_args), 0);
}

int	unset_v(t_env_var **env_vars, const char *var)
{
	t_env_var	*current;
	t_env_var	*prev;

	current = *env_vars;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, var) == 0)
		{
			if (prev == NULL)
				*env_vars = current->next;
			else
				prev->next = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}
