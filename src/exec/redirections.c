/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:30:51 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 14:31:14 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_files_first(t_command *cmd)
{
	t_redirection	*redir;
	t_redirection	*last_out;
	int				tmp_fd;

	redir = cmd->redirs;
	last_out = NULL;
	while (redir)
	{
		if (redir->type == '>' || redir->type == 'A')
			last_out = redir;
		redir = redir->next;
	}
	redir = cmd->redirs;
	while (redir)
	{
		if ((redir->type == '>' || redir->type == 'A') && redir != last_out)
		{
			tmp_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (tmp_fd == -1)
				return (1);
			close(tmp_fd);
		}
		redir = redir->next;
	}
	return (0);
}

int	create_output_file(t_redirection *redir)
{
	int	tmp_fd;

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
	tmp_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd == -1)
		return (1);
	close(tmp_fd);
	return (0);
}

int	handle_redirection(t_command *cmd, t_redirection *redir)
{
	if (redir->type == '>')
		return (open_outt(cmd, redir));
	if (redir->type == 'A')
		return (open_outa(cmd, redir));
	if (redir->type == '<' || redir->type == 'H')
		return (open_in(cmd, redir));
	return (0);
}

int	set_redirs(t_command *cmd)
{
	if (has_heredoc(cmd))
	{
		if (open_outfiles(cmd))
			return (1);
		if (set_fds(cmd))
			return (1);
		if (set_pfd(cmd))
			return (1);
	}
	else
	{
		if (set_pfd(cmd))
			return (1);
		if (open_outfiles(cmd))
			return (1);
		if (set_fds(cmd))
			return (1);
	}
	return (0);
}
