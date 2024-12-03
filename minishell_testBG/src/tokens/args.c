/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:01:19 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/03 13:36:56 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

static char	*process_token_value(t_token *token, t_ctx *ctx)
{
	if (token->type == '\'')
		return (ft_strdup(token->value));
	return (expand_variables(token->value, ctx, token->type));
}

static void	free_args_on_failure(char **args, int i)
{
	while (i > 0)
		free(args[--i]);
	free(args);
}

char	**prepare_args(t_token *tokens, t_ctx *ctx)
{
	char	**args;
	int		count;
	int		i;
	t_token	*current;

	count = count_tokens(tokens);
	args = malloc((count + 1) * sizeof(char *));
	if (!args)
		return (perror("malloc failed"), NULL);
	current = tokens;
	i = 0;
	while (current)
	{
		args[i] = process_token_value(current, ctx);
		if (!args[i])
			return (free_args_on_failure(args, i), NULL);
		i++;
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}
