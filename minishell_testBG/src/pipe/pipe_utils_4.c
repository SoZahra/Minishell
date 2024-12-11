/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:08:54 by llarrey           #+#    #+#             */
/*   Updated: 2024/12/11 17:24:16 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h" 

void	handle_output_redirection(t_token *redir_token, t_redir *redir)
{
	int	output_fd;

	if (ft_strcmp(redir_token->value, ">>") == 0)
		output_fd = open(redir_token->next->value,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		output_fd = open(redir_token->next->value, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	if (output_fd == -1)
		exit(EXIT_FAILURE);
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);
	redir->output = 1;
}

void	setup_pipe(t_pipeline *pl)
{
	if (pl->cmd_end && ft_strcmp(pl->cmd_end->value, "|") == 0)
	{
		if (pipe(pl->pipe_fd) == -1)
			exit(EXIT_FAILURE);
	}
}

void	setup_redirects(int prev_fd, int *pipe_fd, t_token *cmd_end,
t_redir *redir)
{
	t_token	*cmd_start;

	cmd_start = cmd_end;
	if (prev_fd != -1 && redir->input != 1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	cmd_end = cmd_start;
	if (cmd_end && (ft_strcmp(cmd_end->value, "|") == 0) && redir->output != 1)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

void	close_pipe(t_pipeline *pl)
{
	if (pl->prev_fd != -1)
		close(pl->prev_fd);
	if (pl->cmd_end)
	{
		close(pl->pipe_fd[1]);
		pl->prev_fd = pl->pipe_fd[0];
	}
}

void	wait_for_children(void)
{
	int	status;

	while (wait(&status) > 0)
		;
}
