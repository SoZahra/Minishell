/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:39:48 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/11 15:28:30 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_token(const char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		perror("malloc failed");
		return (NULL);
	}
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	return (new_token);
}

t_token *lexer(const char *input) {
    t_token *head = NULL;
    t_token *tail = NULL;
    char *input_copy = ft_strdup(input); // Utiliser une copie pour la tokenisation
    char *token = strtok(input_copy, " ");
    int token_count = 0;

    while (token) {
        printf("Token found: '%s'\n", token); // Debug print

        t_token *new_token = create_token(token);
        if (!new_token) {
            // Gestion des erreurs (libération des tokens précédents)
            while (head)
			{
                t_token *tmp = head;
                head = head->next;
                free(tmp->value);
                free(tmp);
            }
            free(input_copy);
            return NULL;
        }
        if (!head)// Ajout du token à la liste
		{
            head = new_token;
            tail = new_token;
        }
		else
		{
            tail->next = new_token;
            tail = new_token;
        }
        token = strtok(NULL, " ");
        token_count++;
    }
    printf("Total tokens: %d\n", token_count); // Debug print
    free(input_copy); // Libérer la copie d'entrée
    return head;
}


// t_token	*lexer(const char *input)
// {
// 	t_token		*head;
// 	t_token		*tail;
// 	const char	*start = input;
// 		const char *end = start;
// 	char		*token_value;
// 	t_token		*new_token;

// 	head = NULL;
// 	tail = NULL;
// 	while (*start)
// 	{
// 		while (ft_isspace(*start))// Ignorer les espaces
// 			start++;
// 		if (*start == '\0')// Vérifier si on a atteint la fin de l'entrée
// 			break ;
// 		while (*end && !isspace(*end))// Début d'un mot (token)
// 			end++;
// 		token_value = strndup(start, end - start);// Créer un token à partir du mot
// 		if (!token_value)
// 		{
// 			perror("strndup failed");
// 			free_tokens(head);
// 			return (NULL);
// 		}
// 		new_token = create_token(token_value);
// 		free(token_value); // Libérer la chaîne temporaire
// 		if (!new_token)
// 		{
// 			free_tokens(head);
// 			return (NULL); // Échec de la création du token
// 		}
// 		if (head == NULL)// Ajouter le token à la liste
// 		{
// 			head = new_token; // Premier token
// 			tail = new_token;
// 		}
// 		else
// 		{
// 			tail->next = new_token; // Lier le nouveau token
// 			tail = new_token;       // Mettre à jour le pointeur de fin
// 		}
// 		start = end; // Avancer au prochain mot
// 	}
// 	return (head); // Retourner la liste de tokens
// }
