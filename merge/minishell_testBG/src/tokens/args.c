/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:01:19 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/09 16:20:13 by fzayani          ###   ########.fr       */
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

// static char	*process_token_value(t_token *token, t_ctx *ctx)
// {
// 	if (token->type == '\'')
// 		return (ft_strdup(token->value));
// 	return (expand_variables(token->value, ctx, token->type));
// }

static char *process_token_value(t_token *token, t_ctx *ctx)
{
    char *current_value;
    char *next_value;
    char *final_value;

	final_value = NULL;

    // Récupérer la valeur du token actuel
    if (token->type == '\'')
        current_value = ft_strdup(token->value);
    else
        current_value = expand_variables(token->value, ctx, token->type);

    // Si pas de token suivant, retourner la valeur actuelle
    if (!token->next)
        return current_value;

    // Si un espace sépare les tokens, retourner le token actuel
    if (token->had_space)
        return current_value;

    // Si les tokens doivent être concaténés (pas d'espace entre eux)
    next_value = (token->next->type == '\'') ?
                 ft_strdup(token->next->value) :
                 expand_variables(token->next->value, ctx, token->next->type);

    // Joindre les valeurs sans espace
    // final_value = ft_strjoin(current_value, next_value);
    free(current_value);
    free(next_value);

    // Skip le token suivant car il a été utilisé
    token->next = token->next->next;

    return final_value;
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
