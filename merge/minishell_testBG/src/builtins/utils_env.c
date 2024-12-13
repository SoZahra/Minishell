/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:06:46 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/13 18:16:23 by fzayani          ###   ########.fr       */
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

void print_export(t_ctx *ctx)
{
    t_env_var *current = ctx->env_vars;
    if (!current)
        return;

    while (current)
    {
        // Force l'affichage de toutes les variables avec un retour à la ligne
        if (current->name)
        {
            if (current->value)
                printf("declare -x %s=\"%s\"\n", current->name, current->value);
            else
                printf("declare -x %s\n", current->name);
        }
        current = current->next;
    }
}
