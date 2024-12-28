/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:40:47 by llarrey           #+#    #+#             */
/*   Updated: 2024/12/28 11:56:35 by fzayani          ###   ########.fr       */
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
	t_token *tmp;

	tmp = current;
	while (tmp)
	{
		if (ft_strcmp(tmp->value, ">") == 0 || ft_strcmp(tmp->value,
				"<") == 0 || ft_strcmp(tmp->value, ">>") == 0
			|| ft_strcmp(tmp->value, "<<") == 0)
			handle_redirection(tmp);
		tmp = tmp->next;
	}
}

// void	apply_redirect_and_execute(t_pipeline *pl, t_ctx *ctx, int saved_stdin,
// 		int saved_stdout)
// {
// 	process_redirections(pl->cmd_start);
// 	execute_builtin(pl->cmd_line, ctx);
// 	if (!save_and_restore_fds(&saved_stdin, &saved_stdout, 1))
// 		exit(EXIT_FAILURE);
// }

// void	setup_redirects_single_builtin(t_pipeline *pl, t_ctx *ctx)
// {
// 	int	saved_stdin;
// 	int	saved_stdout;

// 	if (!save_and_restore_fds(&saved_stdin, &saved_stdout, 0))
// 		exit(EXIT_FAILURE);
// 	apply_redirect_and_execute(pl, ctx, saved_stdin, saved_stdout);
// }
