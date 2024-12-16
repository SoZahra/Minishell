/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:07:23 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/11 17:43:57 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_in_child(t_pipeline *pl, t_ctx *ctx)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
		prepare_child_execution(pl, ctx);
}

void	process_single_builtin(t_pipeline *pl, t_ctx *ctx)
{
	if (is_builtin(pl->cmd_start->value))
		exec_simple_cmd(pl->cmd_start, ctx);
}

void	process_pipeline_segment(t_pipeline *pl, t_ctx *ctx)
{
	setup_pipe(pl);
	execute_in_child(pl, ctx);
	close_pipe(pl);
}

void	process_pipeline_stage(t_pipeline *pl, t_ctx *ctx)
{
	if (is_builtin(pl->cmd_start->value) && (!pl->cmd_end
			|| pl->cmd_end->value[0] != '|'))
	{
		process_single_builtin(pl, ctx);
		if (pl->cmd_end)
			pl->cmd_start = pl->cmd_end->next;
		else
			pl->cmd_start = NULL;
	}
	else
	{
		process_pipeline_segment(pl, ctx);
		if (pl->cmd_end)
			pl->cmd_start = pl->cmd_end->next;
		else
			pl->cmd_start = NULL;
	}
}

int	process_pline(t_token *tokens, t_ctx *ctx)
{
	t_pipeline	pl;

	pl.prev_fd = -1;
	pl.cmd_start = tokens;
	while (pl.cmd_start)
	{
		pl.cmd_end = pl.cmd_start;
		while (pl.cmd_end && ft_strcmp(pl.cmd_end->value, "|") != 0)
			pl.cmd_end = pl.cmd_end->next;
		process_pipeline_stage(&pl, ctx);
	}
	wait_for_children();
	return (0);
}
