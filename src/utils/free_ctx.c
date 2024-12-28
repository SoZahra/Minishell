/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ctx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:58:02 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 13:23:17 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_ctx_close_fd(t_ctx *ctx)
{
	if (ctx->save_stdin != STDIN_FILENO)
		close(ctx->save_stdin);
	if (ctx->save_stdout != STDOUT_FILENO)
		close(ctx->save_stdout);
}

void	free_ctx(t_ctx *ctx)
{
	if (!ctx)
		return ;
	if (ctx->env_vars)
	{
		free_env(ctx->env_vars);
		ctx->env_vars = NULL;
	}
	if (ctx->tokens)
		free_tokens(ctx->tokens);
	if (ctx->oldpwd)
	{
		free(ctx->oldpwd);
		ctx->oldpwd = NULL;
	}
	if (ctx->pwd)
	{
		free(ctx->pwd);
		ctx->pwd = NULL;
	}
	free_ctx_close_fd(ctx);
}
