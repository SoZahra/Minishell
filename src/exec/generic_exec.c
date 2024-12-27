/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:00:43 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 14:37:33 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	set_and_exec_builtin(t_ctx *ctx, t_command *cmd)
{
	char	*cmd_line;

	cmd_line = tokens_to_string_from_command(cmd);
	if (!cmd_line)
		return (1);
	ctx->exit_status = execute_builtin(cmd_line, ctx);
	free(cmd_line);
	cmd_clean_and_exit(ctx, cmd, NULL, ctx->exit_status);
	return (0);
}

void	exec_parent(t_command *cmd)
{
	if (cmd->prev)
	{
		close(cmd->prev->pfd[0]);
		close(cmd->prev->pfd[1]);
	}
}

void	execute_command(t_ctx *ctx, t_command *cmd, char **env_v)
{
	int	error_code;

	error_code = check_errors(cmd);
	if (error_code)
		cmd_clean_and_exit(ctx, cmd, env_v, error_code);
	if (!cmd->args[0] || !*(cmd->args[0]))
		cmd_clean_and_exit(ctx, cmd, env_v, 0);
	cmd->path = find_command_path(cmd->args[0], ctx);
	if (!cmd->path)
		handle_command_not_found(ctx, cmd, env_v);
	env_v = create_env_array(ctx->env_vars);
	setsig(&ctx->s_sigquit, SIGQUIT, handle_sigquit, 0);
	execve(cmd->path, cmd->args, env_v);
	handle_command_not_found(ctx, cmd, env_v);
}

int	child_process(t_ctx *ctx, t_command *cmd)
{
	char	**env_v;

	env_v = NULL;
	if (set_redirs(cmd))
		cmd_clean_and_exit(ctx, cmd, env_v, 1);
	if (is_builtin(cmd->args[0]))
		set_and_exec_builtin(ctx, cmd);
	execute_command(ctx, cmd, env_v);
	return (0);
}

int	exec_child(t_ctx *ctx, t_command *cmd)
{
	if (!cmd)
		return (1);
	if (cmd->next && pipe(cmd->pfd) == -1)
		return (1);
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (1);
	if (!cmd->pid)
		child_process(ctx, cmd);
	if (cmd->pid)
		exec_parent(cmd);
	return (0);
}
