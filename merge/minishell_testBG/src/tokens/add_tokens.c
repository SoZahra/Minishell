/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:00:31 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/17 15:58:11 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// static void	free_token_resources(t_token *token)
// {
// 	if (token->value)
// 		free(token->value);
// 	if (token->content)
// 		free(token->content);
// 	free(token);
// }

// t_token	*create_new_token(t_token_type type, const char *value)
// {
//     t_token	*new_token;

//     new_token = malloc(sizeof(t_token));
// 	// *new_token = (t_token){0};
//     if (!new_token)
//     {
//         perror("malloc failed");
//         return (NULL);
//     }
//     new_token->value = ft_strdup(value);
//     new_token->content = ft_strdup(value);    // Un seul strdup pour content
//     if (!new_token->value || !new_token->content)
//     {
//         perror("ft_strdup failed");
//         free_token_resources(new_token);
//         return (NULL);
//     }
//     new_token->type = type;
//     new_token->next = NULL;
//     new_token->quoted = 0;
//     new_token->had_space = 0;
//     return (new_token);
// }


t_token *create_new_token(t_token_type type, char *value)
{
    struct s_token *new_token;

    new_token = malloc(sizeof(struct s_token));
    if (!new_token)
        return (perror("malloc failed"), NULL);
    new_token->value = ft_strdup(value);
    if (!new_token->value)
        return (free(new_token->value), free(new_token), free(value), NULL);

    new_token->content = new_token->value; // Réutilise la même chaîne pour content
    new_token->type = type;
    new_token->quoted = 0;
    new_token->had_space = 0;
    new_token->next = NULL;
    new_token->prev = NULL;
    return new_token;
}

// t_token *create_new_token(t_token_type type, char *value)
// {
//     struct s_token *new_token;

//     new_token = malloc(sizeof(struct s_token));
//     if (!new_token)
//         return (perror("malloc failed"), NULL);
//     new_token->value = NULL;
//     new_token->content = NULL;
//     new_token->next = NULL;
//     new_token->prev = NULL;
//     new_token->value = ft_strdup(value);
//     if (!new_token->value)
//         return (free(new_token), free(value), NULL);
//     new_token->content = ft_strdup(value);
//     if (!new_token->content)
//         return (free(new_token->value),free(value), NULL);
//     new_token->type = type;
//     new_token->quoted = 0;
//     new_token->had_space = 0;
//     return new_token;
// }

// int add_token(t_token **token_list, t_token_type type, char *value)
// {
//     t_token *new_token;
//     t_token *current;

//     if (!token_list || !value)
//         return -1;
//     new_token = create_new_token(type, value);
//     if (!new_token)
//         return -1;
//     if (!*token_list)
//     {
//         *token_list = new_token;
//         return 0;
//     }
//     current = *token_list;
//     if(!token_list)
//         return 1;
//     while (current->next)
//         current = current->next;
//     current->next = new_token;
//     new_token->prev = current;
//     return 0;
// }

int add_token(t_token **token_list, t_token_type type, char *value)
{
    t_token *new_token;
    t_token *current;

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

    current = *token_list;
    while (current->next)
        current = current->next;

    current->next = new_token;
    new_token->prev = current;
    return 0;
}

// t_token	*add_token(t_token **token_list, t_token_type type, const char *value)
// {
// 	t_token	*new_token;
// 	t_token	*current;

// 	new_token = create_new_token(type, value);
// 	if (!new_token)
// 		return (NULL);
// 	if (!*token_list)
// 		*token_list = new_token;
// 	else
// 	{
// 		current = *token_list;
// 		if (!current)
//             return NULL;
// 		while (current && current->next)
// 			current = current->next;
// 		current->next = new_token;
// 	}
// 	return (new_token);
// }
