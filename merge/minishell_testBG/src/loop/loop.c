/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:50:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/10 15:48:25 by fzayani          ###   ########.fr       */
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

t_token *tokenize_input(char *line)
{
    t_token *tokens = NULL;

    if (tokenizer(&tokens, line) < 0)
    {
        free_tokens(tokens);
        return (NULL);
    }
    return (tokens);
}

void handle_line_for_loop(char *line, t_ctx *ctx)
{
    t_token *tokens;
    char *output;

    if (*line)
    {
        add_history(line);
        tokens = tokenize_input(line);
        if (tokens)
        {
            output = prepare_command(tokens, ctx);
            if (output)
            {
				char **cmd_array = create_command_array(output);
                if (cmd_array)
                {
                    int builtin_result = is_builtin(output);
                    if (builtin_result)
                        execute_builtin(output, ctx); // ou passer cmd_array à la place de output
                    else
                        execute_external_command(output, ctx);
                    free_array(cmd_array);
                }
                free(output);
            }
            free_tokens(tokens);
        }
        else
            fprintf(stderr, "Error: tokenization failed\n");
    }
}

int	loop_with_pipes(t_ctx *ctx)
{
	char	*line;

	while (1)
	{
		// line = readline(PROMPT);
		line = readline("MiniBG> ");
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
