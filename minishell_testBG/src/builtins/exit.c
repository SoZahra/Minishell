/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:07:10 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 17:11:06 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_exit_with_argument(char **args, t_ctx *ctx)
{
	char	*cleaned_arg;
	long	exit_code;

	cleaned_arg = strip_quotes(args[1]);
	if (!is_numeric_argument(cleaned_arg))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			cleaned_arg);
		free(cleaned_arg);
		ctx->exit_status = 2;
		return (1);
	}
	exit_code = ft_atoi(cleaned_arg);
	free(cleaned_arg);
	if (args[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		ctx->exit_status = 1;
		return (0);
	}
	ctx->exit_status = (exit_code % 256 + 256) % 256;
	return (1);
}

int	handle_exit_without_argument(t_ctx *ctx)
{
	ctx->exit_status = 0;
	return (1);
}

int	process_exit_arg(char **args, t_ctx *ctx)
{
	if (args[1])
		return (handle_exit_with_argument(args, ctx));
	else
		return (handle_exit_without_argument(ctx));
	write(1, "exit\n", 5);
	exit(ctx->exit_status);
	return (1);
}