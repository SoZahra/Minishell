/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:00:31 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/13 14:17:30 by fzayani          ###   ########.fr       */
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
    // Initialiser chaque champ directement
    // new_token->value = ft_strdup(value);
    // new_token->content = ft_strdup(value);
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
    // printf("DEBUG: Created token at %p with value %s\n", (void*)new_token, value);
    free(value);
    return new_token;
}

int add_token(t_token **token_list, t_token_type type, char *value)
{
    if (!token_list || !value)
        return -1;

    t_token *new_token = create_new_token(type, value);
    if (!new_token)
        return -1;

    // Si la liste est vide
    if (!*token_list)
    {
        *token_list = new_token;
        return 0;
    }

    // Ajouter à la fin
    t_token *current = *token_list;
    while (current->next)
        current = current->next;

    current->next = new_token;
    new_token->prev = current;  // N'oublions pas le prev

    return 0;
}
// t_token *add_token(t_token **token_list, t_token_type type, const char *value)
// {
//     struct s_token *new_token;

//     // printf("DEBUG 1: Entering add_token\n");
//     // printf("DEBUG 2: token_list = %p\n", (void*)token_list);
//     if (!token_list)
//     {
//         // printf("DEBUG 3: token_list is NULL\n");
//         return NULL;
//     }

//     // printf("DEBUG 4: *token_list = %p\n", (void*)*token_list);
//     new_token = create_new_token(type, value);
//     // printf("DEBUG 5: Created token at %p with value %s\n", (void*)new_token, value);
//     if (!new_token)
//     {
//         // printf("DEBUG 6: Failed to create new token\n");
//         return NULL;
//     }

//     // Si la liste est vide
//     if (!*token_list)
//     {
//         // printf("DEBUG 7: Setting first token\n");
//         *token_list = new_token;
//     }
//     else
//     {
//         // printf("DEBUG 8: List not empty, adding to end\n");
//         new_token->next = NULL;

//         struct s_token *current = *token_list;
//         // printf("DEBUG 9: current = %p\n", (void*)current);

//         while (current && current->next)
//         {
//             // printf("DEBUG 10: Moving to next token\n");
//             current = current->next;
//         }

//         if (current)
//         {
//             // printf("DEBUG 11: Adding new token to end\n");
//             current->next = new_token;
//         }
//     }
//     // printf("DEBUG 12: Returning new token\n");
//     return new_token;
// }
