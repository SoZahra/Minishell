/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:50:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/03 10:03:29 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_line_for_loop(char *line, t_ctx *ctx)
{
	t_token	*tokens;
	char	**cmd_env;

	if (*line)
	{
		add_history(line);
		tokens = parse_command_line(line, ctx);
		if (tokens)
		{
			cmd_env = convert_env_to_array(ctx);
			if (cmd_env)
			{
				exec_simple_cmd(tokens, cmd_env, ctx);
				free_tab(cmd_env);
			}
		}
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
