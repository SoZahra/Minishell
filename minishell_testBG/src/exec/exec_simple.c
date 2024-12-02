/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:04:03 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 15:04:19 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int exec_simple_cmd(t_token *tokens, char **env, t_ctx *ctx)
{
    char **args;
	(void)env;

    args = prepare_args(tokens, ctx);
    if (!args || !args[0])
    {
        perror("Erreur d'allocation de mémoire pour les arguments");
        free_tab(args);
        ctx->exit_status = 1;
        return 0;
    }
    if (is_builtin(args[0]))
    {
        // Convertir env_vars en tableau pour les builtins
        char **env_array = convert_env_to_array(ctx);
        if (!env_array)
        {
            free_tab(args);
            ctx->exit_status = 1;
            return 0;
        }
        int ret = exec_builtin_cmd(args, env_array, ctx);
        free_tab(args);
        free_tab(env_array);
        return ret;
    }
    else
    {
        fprintf(stderr, "miniBG: %s: command not found\n", args[0]);
        ctx->exit_status = 127;
        free_tab(args);
        return 0;
    }
    // return 0;
}