/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:25:29 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 14:28:38 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_heredoc(t_command *cmd)
{
	int	i;

	if (!cmd || !cmd->redirs)
		return (0);
	i = 0;
	while (cmd->redirs[i].type)
	{
		if (cmd->redirs[i].type == 'H')
			return (1);
		i++;
	}
	return (0);
}

int	handle_heredoc(t_command *cmd, t_redirection *redir)
{
	int	fd;

	if (redir->heredoc_fd <= 0)
	{
		fd = here_doc(redir->file, get_ctx());
		if (fd == -1)
			return (1);
		redir->heredoc_fd = fd;
	}
	if (cmd->in_fd > 0)
		close(cmd->in_fd);
	cmd->in_fd = redir->heredoc_fd;
	return (0);
}

int	here_doc(char *delimiter, t_ctx *ctx)
{
	char	*line;
	pid_t	pid;
	int		status;

	if (pipe(ctx->pfd) == -1)
		return (-1);
	pid = fork();
	g_heredoc_active = TRUE;
	if (!pid)
	{
		while (g_heredoc_active)
		{
			close(ctx->pfd[0]);
			line = readline("> ");
			if (!line)
			{
				close(ctx->pfd[1]);
				ft_fprintf(2, "\nwarning: here-document \
delimited by end-of-file\n");
				break ;
			}
			if (!ft_strcmp(line, delimiter))
			{
				free(line);
				break ;
			}
			write(ctx->pfd[1], line, ft_strlen(line));
			write(ctx->pfd[1], "\n", 1);
			free(line);
		}
		close(ctx->pfd[1]);
		cmd_clean_and_exit(get_ctx(), NULL, NULL, 0);
	}
	setsig(&get_ctx()->s_sigint, SIGINT, SIG_IGN, 0);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ctx->exit_status = WEXITSTATUS(status);
	g_heredoc_active = FALSE;
	setsig(&get_ctx()->s_sigint, SIGINT, handle_sigint, 0);
	close(ctx->pfd[1]);
	return (ctx->pfd[0]);
}

int	setup_heredocs(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->redirs)
		{
			i = 0;
			while (tmp->redirs[i].type)
			{
				if (tmp->redirs[i].type == 'H'
					&& tmp->redirs[i].heredoc_fd <= 0)
				{
					tmp->redirs[i].heredoc_fd
						= here_doc(tmp->redirs[i].file, get_ctx());
					if (tmp->redirs[i].heredoc_fd == -1 || get_ctx()->exit_status == 130)
						return (1);
				}
				i++;
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
