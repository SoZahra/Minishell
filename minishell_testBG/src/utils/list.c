/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:55:03 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 14:55:26 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token *create_token_list(char **args)
{
    t_token *head = NULL;
    t_token *current = NULL;

    for (int i = 0; args[i]; i++)
    {
        t_token *new_token = malloc(sizeof(t_token));
        if (!new_token)
        {
            perror("malloc failed");
            free_tokens(head);
            return NULL;
        }
        new_token->value = ft_strdup(args[i]);
        new_token->type = STRING;  // Type par défaut
        if (!new_token->value)
        {
            free(new_token);
            free_tokens(head);
            return NULL;
        }
        new_token->next = NULL;
        if (!head)
            head = new_token;
        else
            current->next = new_token;
        current = new_token;
    }
    return head;
}