/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:07:10 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/26 12:35:19 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int handle_exit_with_argument(char **args, t_ctx *ctx)
{
    char *cleaned_arg;
    long exit_code;

    cleaned_arg = ft_strdup(args[0]);
    if (!is_numeric_argument(cleaned_arg))
    {
        ft_fprintf(2, "minishell: exit: %s: numeric argument required\n", cleaned_arg);
        free(cleaned_arg);
        ctx->exit_status = 2;
        return 1;
    }
    exit_code = ft_atoi(cleaned_arg);
    free(cleaned_arg);
    if (args[1])
    {
        ft_fprintf(2, "minishell: exit: too many arguments\n");
        ctx->exit_status = 1;
        return 0;
    }
    if (exit_code < 0)
        ctx->exit_status = 256 + (exit_code % 256);
    else
        ctx->exit_status = exit_code % 256;
    return 1;
}

int	handle_exit_without_argument(t_ctx *ctx)
{
	ctx->exit_status = 0;
	return (1);
}

int process_exit_arg(char **args, t_ctx *ctx)
{
    if (!args[0] || !*args[0])
        return handle_exit_without_argument(ctx);
    return handle_exit_with_argument(args, ctx);
}
