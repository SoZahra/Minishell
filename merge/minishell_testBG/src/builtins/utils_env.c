/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:minishell/src/env/utils_env.c
/*   Created: 2024/10/28 14:43:07 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/22 14:24:46 by llarrey          ###   ########.fr       */
=======
/*   Created: 2024/12/02 15:06:46 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/10 11:19:07 by fzayani          ###   ########.fr       */
>>>>>>> Fat:merge/minishell_testBG/src/builtins/utils_env.c
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
