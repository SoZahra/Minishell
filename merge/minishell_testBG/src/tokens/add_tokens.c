/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:00:31 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 17:32:28 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token *create_new_token(t_token_type type, char *value)
{
    struct s_token *new_token;

    new_token = malloc(sizeof(struct s_token));
    if (!new_token)
        return (perror("malloc failed"), NULL);
    new_token->value = NULL;
    new_token->content = NULL;
    new_token->next = NULL;
    new_token->prev = NULL;
    new_token->value = ft_strdup(value);
    if (!new_token->value)
        return (free(new_token), free(value), NULL);
    new_token->content = ft_strdup(value);
    if (!new_token->content)
        return (free(new_token->value), free(new_token), free(value), NULL);
    new_token->type = type;
    new_token->quoted = 0;
    new_token->had_space = 0;
    free(value);
    return new_token;
}

int add_token(t_token **token_list, t_token_type type, char *value)
{
    t_token *new_token;

    if (!token_list || !value)
        return -1;
    new_token = create_new_token(type, value);
    if (!new_token)
        return -1;
    if (!*token_list)
    {
        *token_list = new_token;
        return 0;
    }
    t_token *current = *token_list;
    while (current->next)
        current = current->next;
    current->next = new_token;
    new_token->prev = current;
    return 0;
}
