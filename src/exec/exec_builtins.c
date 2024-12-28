/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:00:59 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:00:46 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_and_add_var(t_ctx *ctx, char *var, char *value)
{
	t_env_var	*new_var;

	new_var = malloc(sizeof(t_env_var));
	if (!new_var)
		return (free(var), free(value), 1);
	new_var->name = var;
	new_var->value = value;
	new_var->next = NULL;
	add_env_var_to_list(&ctx->env_vars, new_var);
	return (0);
}

void	print_env(t_ctx *ctx)
{
	t_env_var	*current;

	current = ctx->env_vars;
	if (!current)
		return ;
	while (current)
	{
		if (current->name)
		{
			if (current->value)
				printf("%s=%s\n", current->name, current->value);
			else
				printf("%s=\n", current->name);
		}
		current = current->next;
	}
}

int	handle_env_builtin(const char *input, t_ctx *ctx)
{
	while (*input == ' ')
		input++;
	if (*input != '\0')
	{
		ft_fprintf(2, "env: too many arguments\n");
		ctx->exit_status = 1;
		return (1);
	}
	print_env(get_ctx());
	ctx->exit_status = 0;
	return (0);
}
