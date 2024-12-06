/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:04:03 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/06 15:29:27 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_simple_cmd(t_token *tokens, t_ctx *ctx)
{
	char	**args;
	int		result;

	args = prepare_args(tokens, ctx);
	if (!validate_args(args, ctx))
		return (0);
	if (is_builtin(args[0]))
	{
		printf("test simple cmd : %s\n", args[0]);
		result = execute_builtin(ctx, args);
		free_tab(args);
		return (result);
	}
	print_command_not_found(args[0], ctx);
	free_tab(args);
	return (0);
}

int	validate_args(char **args, t_ctx *ctx)
{
	if (!args || !args[0])
	{
		perror("Erreur d'allocation de mémoire pour les arguments");
		free_tab(args);
		ctx->exit_status = 1;
		return (0);
	}
	return (1);
}

int	execute_builtin(t_ctx *ctx, char **args)
{
	char	**env_array;
	int		ret;

	env_array = convert_env_to_array(ctx);
	if (!env_array)
	{
		free_tab(args);
		ctx->exit_status = 1;
		return (0);
	}
	printf("test builtin : %s\n", args[0]);
	ret = exec_builtin_cmd(args, env_array, ctx);
	free_tab(env_array);
	return (ret);
}

void	print_command_not_found(const char *cmd, t_ctx *ctx)
{
	fprintf(stderr, "miniBG: %s: command not found\n", cmd);
	ctx->exit_status = 127;
}
