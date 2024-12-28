/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:28:05 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:13:29 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_pfd(t_command *cmd)
{
	if (cmd->prev)
	{
		close(cmd->prev->pfd[1]);
		if (dup2(cmd->prev->pfd[0], STDIN_FILENO) == -1)
			return (1);
		close(cmd->prev->pfd[0]);
	}
	if (cmd->next)
	{
		close(cmd->pfd[0]);
		if (dup2(cmd->pfd[1], STDOUT_FILENO) == -1)
			return (1);
		close(cmd->pfd[1]);
	}
	return (0);
}

int	set_fds(t_command *cmd)
{
	if (cmd->in_fd)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			return (1);
		close(cmd->in_fd);
	}
	if (cmd->out_fd)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			return (1);
		close(cmd->out_fd);
	}
	return (0);
}

int	save_std(t_ctx *ctx)
{
	ctx->save_stdin = dup(STDIN_FILENO);
	if (ctx->save_stdin == -1)
		return (1);
	ctx->save_stdout = dup(STDOUT_FILENO);
	if (ctx->save_stdout == -1)
	{
		close(ctx->save_stdin);
		return (1);
	}
	return (0);
}

int	restore_std(t_ctx *ctx)
{
	int	ret;

	ret = 0;
	if (dup2(ctx->save_stdin, STDIN_FILENO) == -1)
		ret = 1;
	close(ctx->save_stdin);
	if (dup2(ctx->save_stdout, STDOUT_FILENO) == -1)
		ret = 1;
	close(ctx->save_stdout);
	return (ret);
}
