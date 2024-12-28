/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:00:25 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:00:48 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_exit(const char *args, char *cmd, char *cmd_line, t_ctx *ctx)
{
	char	*args_dup;
	int		result;

	args_dup = ft_strdup(args);
	if (!args_dup)
		return (free(cmd), 1);
	free(cmd);
	free(cmd_line);
	result = handle_exit_builtin(args_dup, ctx);
	free(args_dup);
	return (result);
}

static int	execute_cmd(const char *cmd, const char *args, t_ctx *ctx)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (handle_echo_builtin(args, ctx));
	if (ft_strcmp(cmd, "cd") == 0)
		return (handle_cd_builtin(args, ctx));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (handle_pwd_builtin(args, ctx));
	if (ft_strcmp(cmd, "export") == 0)
		return (handle_export_builtin(args, ctx));
	if (ft_strcmp(cmd, "env") == 0)
		return (handle_env_builtin(args, ctx));
	if (ft_strcmp(cmd, "unset") == 0)
		return (handle_unset_builtin(args, ctx));
	return (0);
}

int	execute_builtin(char *cmd_line, t_ctx *ctx)
{
	char		*cmd;
	char		*space;
	const char	*args;
	int			result;

	cmd = ft_strdup(cmd_line);
	if (!cmd)
		return (1);
	space = ft_strchr(cmd, ' ');
	if (space)
		*space = '\0';
	args = cmd_line + ft_strlen(cmd);
	while (*args == ' ')
		args++;
	if (ft_strcmp(cmd, "exit") == 0)
		return (handle_exit(args, cmd, cmd_line, ctx));
	else
	{
		result = execute_cmd(cmd, args, ctx);
		free(cmd);
		return (result);
	}
}
