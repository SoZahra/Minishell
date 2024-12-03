/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:40:11 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 19:00:22 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_exit_builtin(char **args, t_ctx *ctx)
{
	if (process_exit_arg(args, ctx))
		return (1);
	return (1);
}

int	handle_echo_builtin(char **args, t_ctx *ctx)
{
	t_token	*token_list;

	token_list = create_token_list(args + 1);
	if (token_list)
		handle_echo(token_list, ctx);
	ctx->exit_status = 0;
	return (1);
}

int	handle_export_builtin(char **args, t_ctx *ctx)
{
	if (args[1] && (ft_strcmp(args[1], "\"\"") == 0 || ft_strlen(args[1]) == 0))
	{
		fprintf(stderr, "bash: export: `': not a valid identifier\n");
		ctx->exit_status = 1;
		return (1);
	}
	if (args[1] && ft_strcmp(args[1], "$") == 0)
	{
		fprintf(stderr, "bash: export: `$': not a valid identifier\n");
		ctx->exit_status = 1;
		return (1);
	}
	if (!args[1])
	{
		print_export(ctx);
		ctx->exit_status = 0;
		return (1);
	}
	return (handle_export_loop(args, ctx));
}

int	handle_export_loop(char **args, t_ctx *ctx)
{
	int	i;
	int	has_error;

	i = 1;
	has_error = 0;
	while (args[i])
	{
		if (handle_special_case(args[i], ctx))
		{
			i++;
			continue ;
		}
		has_error |= process_var_assignment(args[i], ctx);
		i++;
	}
	ctx->exit_status = has_error;
	return (1);
}
