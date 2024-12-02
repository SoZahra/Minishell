/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:07:10 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 15:08:25 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int process_exit_arg(char **args, t_ctx *ctx)
{
    char *cleaned_arg;
    long exit_code;

    if (args[1])
	{
        cleaned_arg = strip_quotes(args[1]); // Supprimer les quotes
        if (!is_numeric_argument(cleaned_arg))
		{
            fprintf(stderr, "minishell: exit: %s: numeric argument required\n", cleaned_arg);
            free(cleaned_arg);
            ctx->exit_status = 2;
            return 1; // Indique une sortie immédiate avec code 2
        }
        exit_code = ft_atoi(cleaned_arg);
        free(cleaned_arg);
        if (args[2])
		{
            fprintf(stderr, "minishell: exit: too many arguments\n");
            ctx->exit_status = 1;
            return 0; // Ne pas quitter immédiatement
        }
        ctx->exit_status = (exit_code % 256 + 256) % 256; // Calculer modulo 256
    }
	else
		ctx->exit_status = 0;
    write(1, "exit\n", 5);
    exit(ctx->exit_status);
    return 1; // Pour respecter les conventions de retour (même si unreachable ici)
}

