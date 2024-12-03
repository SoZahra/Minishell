/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:50:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/03 17:06:33 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_line_for_loop(char *line, t_ctx *ctx)
{
	t_token	*tokens;

	if (*line)
	{
		add_history(line);
		tokens = parse_command_line(line, ctx);
		if (tokens)
			process_pline(tokens, ctx);
	}
}

int	loop_with_pipes(t_ctx *ctx)
{
	char	*line;

	while (1)
	{
		line = readline(PROMPT);
		if (line == NULL)
		{
			write(1, "exit\n", 5);
			exit(ctx->exit_status);
		}
		handle_line_for_loop(line, ctx);
		free(line);
	}
	return (ctx->exit_status);
}
