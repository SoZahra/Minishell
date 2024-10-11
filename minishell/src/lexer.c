/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:39:48 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/11 15:40:38 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		perror("malloc failed");
		return (NULL);
	}
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	return (new_token);
}

t_token *lexer(const char *input) {
    t_token *head = NULL;
    t_token *tail = NULL;
    char *input_copy = strdup(input);
    char *token = strtok(input_copy, " ");

    while (token) {
        t_token *new_token = NULL;

        // Identifier le type de token
        if (strcmp(token, ">") == 0) {
            new_token = create_token(TOKEN_REDIRECT_OUTPUT, token);
        } else if (strcmp(token, "<") == 0) {
            new_token = create_token(TOKEN_REDIRECT_INPUT, token);
        } else if (strcmp(token, ">>") == 0) {
            new_token = create_token(TOKEN_REDIRECT_APPEND, token);
        } else if (strcmp(token, "<<") == 0) {
            new_token = create_token(TOKEN_HEREDOC, token);
        } else {
            new_token = create_token(TOKEN_COMMAND, token);
        }
        // Gestion d'erreur pour la création de tokens
        if (!new_token) {
            while (head) {
                t_token *tmp = head;
                head = head->next;
                free(tmp->value);
                free(tmp);
            }
            free(input_copy);
            return NULL;
        }
        // Ajout du token à la liste
        if (!head) {
            head = new_token;
            tail = new_token;
        } else {
            tail->next = new_token;
            tail = new_token;
        }
        token = strtok(NULL, " ");
    }
    free(input_copy);
    return head;
}

// t_token *lexer(const char *input) {
//     t_token *head = NULL;
//     t_token *tail = NULL;
//     char *input_copy = ft_strdup(input); // Utiliser une copie pour la tokenisation
//     char *token = strtok(input_copy, " ");
//     int token_count = 0;

//     while (token) {
//         printf("Token found: '%s'\n", token); // Debug print

//         t_token *new_token = create_token(token);
//         if (!new_token) {
//             // Gestion des erreurs (libération des tokens précédents)
//             while (head)
// 			{
//                 t_token *tmp = head;
//                 head = head->next;
//                 free(tmp->value);
//                 free(tmp);
//             }
//             free(input_copy);
//             return NULL;
//         }
//         if (!head)// Ajout du token à la liste
// 		{
//             head = new_token;
//             tail = new_token;
//         }
// 		else
// 		{
//             tail->next = new_token;
//             tail = new_token;
//         }
//         token = strtok(NULL, " ");
//         token_count++;
//     }
//     printf("Total tokens: %d\n", token_count); // Debug print
//     free(input_copy); // Libérer la copie d'entrée
//     return head;
// }
