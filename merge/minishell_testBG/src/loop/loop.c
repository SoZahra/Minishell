/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:50:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/09 18:57:16 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void print_tokens(t_token *tokens)
{
	t_token *tmp;

	if (!tokens)
		return ;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->value)
			printf("value: %s \t type: %c  \t flag: %d\n", tmp->value, tmp->type, tmp->had_space);
		tmp = tmp->next;
	}
}

void	handle_line_for_loop(char *line, t_ctx *ctx)
{
	t_token	*tokens;

	// tokens = NULL;
	if (*line)
	{
		add_history(line);

		// tokens = parse_command_line(line, ctx);
		tokens = tokenizer(&tokens, line);
		// return ;
		if (tokens)
			process_pline(tokens, ctx);
		free_tokens(tokens);
	}
}

int	loop_with_pipes(t_ctx *ctx)
{
	char	*line;

	while (1)
	{
		// line = readline(PROMPT);
		line = readline("MiniBG$ ");
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
