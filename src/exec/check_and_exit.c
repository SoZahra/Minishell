/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_and_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:34:56 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 14:39:19 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_clean_and_exit(t_ctx *ctx, t_command *cmd, char **env_v,
		int exit_code)
{
	if (ctx->save_stdin > 2)
		close(ctx->save_stdin);
	if (ctx->save_stdout > 2)
		close(ctx->save_stdout);
	free_args(env_v);
	while (cmd && cmd->prev)
		cmd = cmd->prev;
	free_command_list(cmd);
	cleanup_shell(ctx);
	free_env(ctx->env_vars);
	exit(exit_code);
}

int	check_direct_path(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
	{
		ft_fprintf(2, "MiniBG: %s: No such file or directory\n", path);
		return (127);
	}
	if (S_ISDIR(statbuf.st_mode))
	{
		ft_fprintf(2, "MiniBG: %s: Is a directory\n", path);
		return (126);
	}
	if (access(path, X_OK) == -1)
	{
		ft_fprintf(2, "MiniBG: %s: Permission denied\n", path);
		return (126);
	}
	return (0);
}

int	check_errors(t_command *cmd)
{
	if (!cmd->args[0] || !*(cmd->args[0]))
		return (0);
	if (cmd->args[0][0] == '/'
		|| (cmd->args[0][0] == '.' && cmd->args[0][1] == '/'))
		return (check_direct_path(cmd->args[0]));
	return (0);
}

void	handle_command_not_found(t_ctx *ctx, t_command *cmd, char **env_v)
{
	ft_fprintf(2, "MiniBG: %s: command not found\n", cmd->args[0]);
	cmd_clean_and_exit(ctx, cmd, env_v, 127);
}

int	clear_exit(t_ctx *ctx)
{
	write(1, "exit\n", 5);
	cleanup_shell(ctx);
	rl_clear_history();
	free_command_list(get_ctx()->current_command);
	return (0);
}
