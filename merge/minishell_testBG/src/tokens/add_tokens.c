/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:00:31 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/08 16:42:58 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	free_token_resources(t_token *token)
{
	if (token->value)
		free(token->value);
	if (token->content)
		free(token->content);
	free(token);
}

t_token	*create_new_token(t_token_type type, const char *value)
{
    t_token	*new_token;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        perror("malloc failed");
        return (NULL);
    }
    new_token->value = ft_strdup(value);
    new_token->content = ft_strdup(value);    // Un seul strdup pour content
    if (!new_token->value || !new_token->content)
    {
        perror("ft_strdup failed");
        free_token_resources(new_token);
        return (NULL);
    }
    new_token->type = type;
    new_token->next = NULL;
    new_token->quoted = 0;
    new_token->had_space = 0;
    return (new_token);
}

t_token	*add_token(t_token **token_list, t_token_type type, const char *value)
{
	t_token	*new_token;
	t_token	*current;

	new_token = create_new_token(type, value);
	if (!new_token)
		return (NULL);
	if (!*token_list)
		*token_list = new_token;
	else
	{
		current = *token_list;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
	return (new_token);
}