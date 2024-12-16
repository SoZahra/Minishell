/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:40:47 by llarrey           #+#    #+#             */
/*   Updated: 2024/12/15 17:53:59 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	save_and_restore_fds(int *saved_stdin, int *saved_stdout, int restore)
{
	if (restore)
	{
		if (dup2(*saved_stdin, STDIN_FILENO) == -1 || dup2(*saved_stdout,
				STDOUT_FILENO) == -1)
			return (0);
		close(*saved_stdin);
		close(*saved_stdout);
		return (1);
	}
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	return (*saved_stdin != -1 && *saved_stdout != -1);
}

void	handle_redirection(t_token *current)
{
	int	fd;

	fd = -1;
	if (ft_strcmp(current->value, ">") == 0 && current->next)
		fd = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strcmp(current->value, "<") == 0 && current->next)
		fd = open(current->next->value, O_RDONLY);
	else if (ft_strcmp(current->value, ">>") == 0 && current->next)
		fd = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (ft_strcmp(current->value, "<<") == 0 && current->next)
		exit(EXIT_FAILURE);
	if (fd == -1)
		exit(EXIT_FAILURE);
	if (ft_strcmp(current->value, "<") == 0)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	process_redirections(t_token *current)
{
	while (current)
	{
		if (ft_strcmp(current->value, ">") == 0 || ft_strcmp(current->value,
				"<") == 0 || ft_strcmp(current->value, ">>") == 0
			|| ft_strcmp(current->value, "<<") == 0)
			handle_redirection(current);
		current = current->next;
	}
}

void	apply_redirect_and_execute(t_pipeline *pl, t_ctx *ctx, int saved_stdin,
		int saved_stdout)
{
	process_redirections(pl->cmd_start);
	execute_builtin(pl->cmd_line, ctx);
	if (!save_and_restore_fds(&saved_stdin, &saved_stdout, 1))
		exit(EXIT_FAILURE);
}

void	setup_redirects_single_builtin(t_pipeline *pl, t_ctx *ctx)
{
	int	saved_stdin;
	int	saved_stdout;

	if (!save_and_restore_fds(&saved_stdin, &saved_stdout, 0))
		exit(EXIT_FAILURE);
	apply_redirect_and_execute(pl, ctx, saved_stdin, saved_stdout);
}

/* void	setup_redirects_single_builtin(t_pipeline *pl, t_ctx *ctx)
{
	t_token *current = pl->cmd_start;
	int fd;
	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDOUT_FILENO);

	if (saved_stdin == -1 || saved_stdout == -1)
	{
		exit(EXIT_FAILURE);
	}
	while (current)
	{
		if (ft_strcmp(current->value, ">") == 0 && current->next)
		{
			fd = open(current->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (ft_strcmp(current->value, "<") == 0 && current->next)
		{
			fd = open(current->next->value, O_RDONLY);
			if (fd == -1)
			{
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (ft_strcmp(current->value, ">>") == 0 && current->next)
		{
			fd = open(current->next->value, O_WRONLY | O_CREAT | O_APPEND,
					0644);
			if (fd == -1)
			{
				exit(EXIT_FAILURE);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (ft_strcmp(current->value, "<<") == 0 && current->next)
		{
			exit(EXIT_FAILURE);
		}
		current = current->next;
	}

	execute_builtin(pl->cmd_line, ctx);

	if (dup2(saved_stdin, STDIN_FILENO) == -1 || dup2(saved_stdout,
			STDOUT_FILENO) == -1)
	{
		exit(EXIT_FAILURE);
	}

	close(saved_stdin);
	close(saved_stdout);
} */