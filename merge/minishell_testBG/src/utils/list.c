/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:55:03 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/08 18:54:18 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// t_token	*create_token_node(char *arg)
// {
// 	t_token	*new_token;

// 	new_token = malloc(sizeof(t_token));
// 	if (!new_token)
// 	{
// 		perror("malloc failed");
// 		return (NULL);
// 	}
// 	new_token->value = ft_strdup(arg);
// 	if (!new_token->value)
// 	{
// 		free(new_token);
// 		perror("ft_strdup failed");
// 		return (NULL);
// 	}
// 	new_token->type = STRING;
// 	new_token->next = NULL;
// 	return (new_token);
// }

t_token *create_token_node(char *arg)
{
    t_token *new_token;
    
    new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        perror("malloc failed");
        return (NULL);
    }

    new_token->value = ft_strdup(arg);
    if (!new_token->value)
    {
        free(new_token);
        perror("ft_strdup failed");
        return (NULL);
    }

    new_token->type = STRING;
    new_token->next = NULL;
    new_token->quoted = 0;
    new_token->had_space = 0;  // Initialisé à 0, le parsing définira si c'est 1
    
    return new_token;
}

t_token	*create_token_list(char **args)
{
	t_token	*head;
	t_token	*current;
	t_token	*new_token;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
	while (args[i])
	{
		new_token = create_token_node(args[i]);
		if (!new_token)
		{
			free_tokens(head);
			return (NULL);
		}
		if (!head)
			head = new_token;
		else
			current->next = new_token;
		current = new_token;
		i++;
	}
	return (head);
}
