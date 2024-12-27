/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:58:02 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 14:58:59 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	if (cmd->args)
	{
		while (i < cmd->arg_count)
		{
			free(cmd->args[i]);
			cmd->args[i] = NULL;
			i++;
		}
		free(cmd->args);
		cmd->args = NULL;
	}
	if (cmd->path)
	{
		free(cmd->path);
		cmd->path = NULL;
	}
	if (cmd->had_spaces)
	{
		free(cmd->had_spaces);
		cmd->had_spaces = NULL;
	}
	if (cmd->redirs)
	{
		close_heredocs(cmd);
		for (int i = 0; cmd->redirs[i].type != 0; i++)
		{
			if (cmd->redirs[i].file)
				free(cmd->redirs[i].file);
			cmd->redirs[i].file = NULL;
		}
		free(cmd->redirs);
		cmd->redirs = NULL;
	}
	if (cmd->in_fd > 2)
		close(cmd->in_fd);
	if (cmd->out_fd > 2)
		close(cmd->out_fd);
	if (cmd->pfd[0] > 2)
		close(cmd->pfd[0]);
	if (cmd->pfd[1] > 2)
		close(cmd->pfd[1]);
	free(cmd);
}

void	free_ctx(t_ctx *ctx)
{
	t_token	*current;
	t_token	*next;

	if (!ctx)
		return ;
	if (ctx->env_vars)
	{
		free_env(ctx->env_vars);
		ctx->env_vars = NULL;
	}
	if (ctx->tokens)
	{
		current = ctx->tokens;
		while (current)
		{
			next = current->next;
			if (current->value)
			{
				free(current->value);
				current->value = NULL;
			}
			current->content = NULL;
			free(current);
			current = next;
		}
		ctx->tokens = NULL;
	}
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
	if (ctx->save_stdin != STDIN_FILENO)
		close(ctx->save_stdin);
	if (ctx->save_stdout != STDOUT_FILENO)
		close(ctx->save_stdout);
}
