/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:50:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/10 00:00:47 by fatimazahra      ###   ########.fr       */
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
        return (NULL); // Retourne NULL en cas d'erreur de tokenisation
    }
    return (tokens);
}

void	handle_line_for_loop(char *line, t_ctx *ctx)
{
	t_token	*tokens;
	char *output;

	// tokens = NULL;
	if (*line)
	{
		add_history(line);

		// tokens = parse_command_line(line, ctx);
		tokens = tokenize_input(line);
		// return ;
		if (tokens)
		{
			output = process_tokens(tokens, ctx); // Process les tokens
            // printf("%s\n", output); // Affiche la sortie finale
            free(output);
            free_tokens(tokens);
		}
		else
        {
            fprintf(stderr, "Error: tokenization failed\n");
        }
			// process_pline(tokens, ctx);
		// free_tokens(tokens);
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
