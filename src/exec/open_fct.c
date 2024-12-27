/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fct.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 11:58:25 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 14:01:17 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection(t_command *cmd, t_redirection *redir);

static int	open_output_file(t_command *cmd, t_redirection *redir)
{
	cmd->out_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->out_fd == -1)
	{
		ft_fprintf(2, "MiniBG: %s: Cannot create file\n", redir->file);
		return (1);
	}
	return (0);
}

int	open_outt(t_command *cmd, t_redirection *redir)
{
	if (cmd->out_fd > 0)
		close(cmd->out_fd);
	if (access(redir->file, F_OK) == 0)
	{
		if (check_existing_file(redir->file))
			return (1);
	}
	else if (check_parent_dir(redir->file))
		return (1);
	return (open_output_file(cmd, redir));
}

int	open_outa(t_command *cmd, t_redirection *redir)
{
	if (access(redir->file, F_OK) == 0)
	{
		if (access(redir->file, W_OK) == -1)
		{
			ft_fprintf(2, "MiniBG: %s: Permission denied\n", redir->file);
			return (1);
		}
	}
	else if (check_parent_dir(redir->file))
		return (1);
	if (cmd->out_fd > 0)
		close(cmd->out_fd);
	cmd->out_fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->out_fd == -1)
		return (perror("MiniBG"), 1);
	return (0);
}

int	open_in(t_command *cmd, t_redirection *redir)
{
	if (redir->type == 'H')
		return (handle_heredoc(cmd, redir));
	if (access(redir->file, F_OK))
	{
		get_ctx()->exit_status = 1;
		return (ft_fprintf(2, "MiniBG: %s: No such file or directory\n",
				redir->file),
			1);
	}
	if (access(redir->file, R_OK))
	{
		get_ctx()->exit_status = 1;
		return (ft_fprintf(2, "MiniBG %s: Permission denied\n", redir->file), 1);
	}
	if (cmd->in_fd)
		close(cmd->in_fd);
	cmd->in_fd = open(redir->file, O_RDONLY);
	if (cmd->in_fd == -1)
		return (1);
	return (0);
}

int	open_outfiles(t_command *cmd)
{
	t_redirection	*curr;
	int				i;

	if (!cmd || !cmd->redirs)
		return (0);
	curr = cmd->redirs;
	i = 0;
	while (curr[i].type)
	{
		if (handle_redirection(cmd, &curr[i]))
			return (1);
		i++;
	}
	return (0);
}
