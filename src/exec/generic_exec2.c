/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic_exec2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:37:16 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:04:24 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin_once(t_ctx *ctx, t_command *cmd)
{
	char	*cmd_line;
	int		ret;

	if (!cmd)
		return (1);
	cmd_line = tokens_to_string_from_command(cmd);
	if (!cmd_line)
		return (1);
	if (save_std(ctx))
		return (free(cmd_line), 1);
	if (set_redirs(cmd))
	{
		restore_std(ctx);
		return (free(cmd_line), 1);
	}
	ctx->exit_status = execute_builtin(cmd_line, ctx);
	if (ft_strncmp(cmd->args[0], "exit", 5))
		free(cmd_line);
	ret = restore_std(ctx);
	return (ret);
}

void	wait_loop(t_ctx *ctx, t_command *cmd)
{
	t_command	*tmp;
	int			status;

	tmp = cmd;
	status = 0;
	while (tmp)
	{
		if (tmp->pid > 0)
		{
			waitpid(tmp->pid, &status, 0);
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				status = WTERMSIG(status) + 128;
			set_term_attr();
			ctx->exit_status = status;
		}
		tmp = tmp->next;
	}
}

static int	exec_command_type(t_ctx *ctx, t_command *cmd, int *has_child)
{
	if (!cmd->prev && !cmd->next && is_builtin(cmd->args[0]))
	{
		if (exec_builtin_once(ctx, cmd))
			return (1);
	}
	else
	{
		if (exec_child(ctx, cmd))
			return (1);
		*has_child = 1;
	}
	return (0);
}

int	exec_loop(t_ctx *ctx, t_command *cmd)
{
	t_command	*tmp;
	int			has_child;
	int			ret;

	if (setup_heredocs(cmd))
		return (1);
	tmp = cmd;
	has_child = ((ret = 0));
	ctx->current_command = cmd;
	while (tmp)
	{
		if (exec_command_type(ctx, tmp, &has_child))
			ret = 1;
		tmp = tmp->next;
	}
	if (has_child)
		wait_loop(ctx, cmd);
	return (ret);
}
