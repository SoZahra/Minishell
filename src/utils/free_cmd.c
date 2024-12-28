/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:18:49 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 13:20:54 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_cmd_close_fd(t_command *cmd)
{
	if (cmd->in_fd > 2)
		close(cmd->in_fd);
	if (cmd->out_fd > 2)
		close(cmd->out_fd);
	if (cmd->pfd[0] > 2)
		close(cmd->pfd[0]);
	if (cmd->pfd[1] > 2)
		close(cmd->pfd[1]);
}

static void	free_command2(t_command *cmd)
{
	int	i;

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
		i = 0;
		while (cmd->redirs[i].type != 0)
		{
			if (cmd->redirs[i].file)
				free(cmd->redirs[i].file);
			cmd->redirs[i++].file = NULL;
		}
		free(cmd->redirs);
		cmd->redirs = NULL;
	}
}

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
	free_command2(cmd);
	free_cmd_close_fd(cmd);
	free(cmd);
}
