/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:06:46 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/10 11:19:07 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	print_env(t_ctx *ctx)
// {
// 	t_env_var	*current;

// 	current = ctx->env_vars;
// 	while (current)
// 	{
// 		if (current->value)
// 			printf("%s=%s\n", current->name, current->value);
// 		current = current->next;
// 	}
// }

void	print_export(t_ctx *ctx)
{
	t_env_var	*current;

	current = ctx->env_vars;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->name, current->value);
		else
			printf("%s\n", current->name);
		current = current->next;
	}
}
