/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:29:12 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 14:51:54 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_output_redirection(
	t_token *redir_token,
	int *redirect,
	int *redirect_output)
{
	int	output_fd;

	if (ft_strcmp(redir_token->value, ">>") == 0)
		output_fd = open(redir_token->next->value,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		output_fd = open(redir_token->next->value,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd == -1)
	{
		perror("Redirection error");
		exit(EXIT_FAILURE);
	}
	dup2(output_fd, STDOUT_FILENO);
	close(output_fd);
	*redirect = 1;
	*redirect_output = 1;
}

void	cleanup_parent_resources(
	int *prev_fd,
	int *pipe_fd,
	t_token **cmd_start,
	t_token *cmd_end)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd_end != NULL)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
		*cmd_start = cmd_end->next;
	}
	else
		*cmd_start = NULL;
}

int	wait_for_all_children(int status, t_token *tokens, t_ctx *ctx)
{
	int		exit_status;
	t_token	*next;
	t_token	*next_next;

	exit_status = (status >> 8) & 0xFF;
	if ((status & 0x7F) != 0)
		return (ctx->exit_status);
	if (tokens && ft_strcmp(tokens->value, "cd") == 0)
	{
		next = tokens->next;
		next_next = NULL;

		if (next)
			next_next = next->next;
		if (next && next_next)
			ctx->exit_status = 1;
		else
			ctx->exit_status = exit_status;
	}
	else if (tokens && ft_strcmp(tokens->value, "exit") == 0)
		ctx->exit_status = exit_status;
	return (ctx->exit_status);
}
