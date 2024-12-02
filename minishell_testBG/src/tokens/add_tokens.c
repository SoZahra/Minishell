/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:00:31 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 15:00:55 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token *add_token(t_token **token_list, t_token_type type, const char *value)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        perror("malloc failed");
        return NULL;
    }
    new_token->value = ft_strdup(value); // Duplicate the string
    new_token->content = ft_strdup(value); // Duplicate aussi pour content
    if (!new_token->value || !new_token->content)
    {
        if (new_token->value)
            free(new_token->value);
        if (new_token->content)
            free(new_token->content);
        free(new_token);
        perror("ft_strdup failed");
        return NULL;
    }
    new_token->type = type;
    new_token->next = NULL;
    if (!*token_list)
        *token_list = new_token;
    else
    {
        t_token *current = *token_list;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
    return new_token;
}
