/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:51:13 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/26 16:51:42 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	wait_for_children(t_command *cmd, t_ctx *ctx)
{
	t_command	*current;
	int			status;

	current = cmd;
	while (current->prev)
		current = current->prev;

	while (current)
	{
		if (current->pid > 0)
		{
			waitpid(current->pid, &status, 0);
			if (WIFEXITED(status))
				ctx->exit_status = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
				ctx->exit_status = WTERMSIG(status) + 128;
		}
		current = current->next;
	}
}

void	clear_and_exit(pid_t *pids, t_command *cmds, int exit_code)
{
	free_ctx(get_ctx());
	free_env(get_ctx()->env_vars);
	(void)pids;
	// free pids
	free_tokens(get_ctx()->tokens);
	free_args(cmds->args);
	free_command(cmds);
	exit(exit_code);
}
