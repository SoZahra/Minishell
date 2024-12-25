/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:00:31 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/26 00:11:10 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// t_token *create_new_token(t_token_type type, char *value)
// {
//     struct s_token *new_token;

//     new_token = malloc(sizeof(struct s_token));
//     if (!new_token)
//         return (perror("malloc failed"), NULL);
//     new_token->value = ft_strdup(value);
//     if (!new_token->value)
//         return (free(new_token), NULL);
//     new_token->content = new_token->value;
//     new_token->type = type;
//     new_token->quoted = 0;
//     new_token->had_space = 0;
//     new_token->next = NULL;
//     new_token->prev = NULL;
//     return new_token;
// }

t_token *create_new_token(t_token_type type, char *value)
{
    t_token *new_token;
    
    if (!value)
        return NULL;
    new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return NULL;
    ft_memset(new_token, 0, sizeof(t_token));
    new_token->value = ft_strdup(value);
    if (!new_token->value)
    {
        free(new_token);
        return NULL;
    }
    // new_token->content = NULL;
    // new_token->content = new_token->value;  // Même adresse
    new_token->type = type;
    return (new_token);
}


int add_token(t_token **tokens, t_token_type type, char *value)
{
    t_token *new_token;
    t_token *last;

    if (!tokens || !value)
        return -1;

    new_token = create_new_token(type, value);
    if (!new_token)
        return -1;
    if (!*tokens)
    {
        *tokens = new_token;
        return 0;
    }
    last = *tokens;
    while (last->next)
        last = last->next;

    last->next = new_token;
    new_token->prev = last;

    return 0;
}