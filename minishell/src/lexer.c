/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:39:48 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/21 14:55:54 by fzayani          ###   ########.fr       */
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

void add_token(t_token **head, t_token_type type, const char *value)
{
    t_token *new_token = create_token(type, value);
    if (!new_token)
        return;
    if (!*head)
        *head = new_token;
    else
    {
        t_token *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
}

t_token_type get_token_type(const char *str)
{
    if (strcmp(str, "|") == 0)
        return TOKEN_PIPE;
    if (strcmp(str, "<") == 0)
        return TOKEN_REDIRECT_INPUT;
    if (strcmp(str, ">") == 0)
        return TOKEN_REDIRECT_OUTPUT;
    if (strcmp(str, ">>") == 0)
        return TOKEN_REDIRECT_APPEND;
    if (strcmp(str, "<<") == 0)
        return TOKEN_HEREDOC;

    return TOKEN_ARGUMENT; // Par défaut, on considère que c'est un argument
}

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

t_token *parse_command_line(char *line)
{
    t_token *token_list = NULL;
    char buffer[1024];
    int i = 0;
    int in_quotes = 0;  // Variable pour savoir si on est à l'intérieur de guillemets

    while (*line)
    {
        if (*line == '"' || *line == '\'')  // Détecter un guillemet ouvrant ou fermant
        {//quote pas ferme erreur
            if (in_quotes)
            {
                in_quotes = 0;// Si on était déjà dans les guillemets, on les ferme
                buffer[i] = '\0';
                add_token(&token_list, TOKEN_ARGUMENT, buffer);  // Ajouter le token complet
                i = 0;  // Réinitialiser le buffer
            }
            else
                in_quotes = 1;// Si on n'était pas dans des guillemets, on les ouvre
            line++;  // Passer le guillemet
            continue;
        }
        if (is_whitespace(*line) && !in_quotes)
        {
            if (i > 0)  // Si le buffer contient déjà des caractères
            {
                buffer[i] = '\0';  // Terminer la chaîne
                add_token(&token_list, TOKEN_ARGUMENT, buffer);  // Ajouter le token
                i = 0;  // Réinitialiser le buffer pour le prochain token
            }
            line++;  // Passer l'espace
            continue;
        }
        if ((*line == '|' || *line == '<' || *line == '>') && !in_quotes)
        {
            if (i > 0)
            {
                buffer[i] = '\0';
                add_token(&token_list, TOKEN_ARGUMENT, buffer);
                i = 0;
            }
            char special[3] = {0};  // pour gérer les redirections ">>" et "<<"
            special[0] = *line;
            line++;

            if ((*special == '>' && *line == '>') || (*special == '<' && *line == '<'))
            {
                special[1] = *line;
                line++;
            }
            add_token(&token_list, get_token_type(special), special);
        }
        else
            buffer[i++] = *line++;  // Ajouter le caractère dans le buffer
    }
    if (i > 0)// Ajouter le dernier token s'il y en a un
    {
        buffer[i] = '\0';
        add_token(&token_list, TOKEN_ARGUMENT, buffer);
    }
    return token_list;
}

t_token *lexer(const char *input)
{
    t_token *head = NULL;
    t_token *tail = NULL;
    char *input_copy = strdup(input);
    char *ptr = input_copy;
    int first_token = 1;  // Pour identifier la première commande

    while (*ptr)
    {
        if (*ptr == ' ')// Ignorer les espaces
        {
            ptr++;
            continue;
        }
        if (*ptr == '|')
        {
            t_token *new_token = create_token(TOKEN_PIPE, "|");
            if (!new_token) {
                free(input_copy);
                return NULL; // Gestion d'erreur
            }
            first_token = 1; // Le prochain mot après le pipe est une nouvelle commande
            if (!head)
            {
                head = new_token;
                tail = new_token;
            }
            else
            {
                tail->next = new_token;
                tail = new_token;
            }
            ptr++; // Passer au prochain caractère
            continue;
        }
        char *start = ptr;// Traiter les autres tokens (commandes et arguments)
        while (*ptr && *ptr != ' ' && *ptr != '|') { // Arrêter aux espaces et pipes
            ptr++;
        }
        char *token_str = strndup(start, ptr - start); // Extraire le token
        t_token *new_token = NULL;
        if (first_token)
        {
            new_token = create_token(TOKEN_COMMAND, token_str);// Si c'est le premier token après un pipe ou au début
            first_token = 0;  // Après la commande, les suivants sont des arguments
        }
        else
            new_token = create_token(TOKEN_ARGUMENT, token_str);
        free(token_str); // Libérer la chaîne du token
        if (!new_token) // Gestion d'erreur pour la création de tokens
        {
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
        } else {
            tail->next = new_token;
            tail = new_token;
        }
    }
    free(input_copy);
    return (head);
}


// t_token *lexer(const char *input) {
//     t_token *head = NULL;
//     t_token *tail = NULL;
//     char *input_copy = strdup(input);
//     char *ptr = input_copy;

//     while (*ptr) {
//         // Ignorer les espaces
//         if (*ptr == ' ') {
//             ptr++;
//             continue;
//         }
//         // Identifier le pipe
//         if (*ptr == '|') {
//             t_token *new_token = create_token(TOKEN_PIPE, "|");
//             if (!new_token) {
//                 free(input_copy);
//                 return NULL; // Gestion d'erreur
//             }
//             if (!head) {
//                 head = new_token;
//                 tail = new_token;
//             } else {
//                 tail->next = new_token;
//                 tail = new_token;
//             }
//             ptr++; // Passer au prochain caractère
//             continue;

//         // Traiter les autres tokens (commandes et redirections)
//         char *start = ptr;
//         while (*ptr && *ptr != ' ' && *ptr != '|') { // Arreter aux espaces et pipes
//             ptr++;
//         }
//         char *token_str = strndup(start, ptr - start); // Extraire le token
//         t_token *new_token = NULL;
//         // Identifier le type de token
//         if (strcmp(token_str, ">") == 0) {
//             new_token = create_token(TOKEN_REDIRECT_OUTPUT, token_str);
//         } else if (strcmp(token_str, "<") == 0) {
//             new_token = create_token(TOKEN_REDIRECT_INPUT, token_str);
//         } else if (strcmp(token_str, ">>") == 0) {
//             new_token = create_token(TOKEN_REDIRECT_APPEND, token_str);
//         } else if (strcmp(token_str, "<<") == 0) {
//             new_token = create_token(TOKEN_HEREDOC, token_str);
//         } else {
//             new_token = create_token(TOKEN_COMMAND, token_str);
//         }
//         free(token_str); // Libérer la chaîne du token
//         // Gestion d'erreur pour la création de tokens
//         if (!new_token) {
//             while (head) {
//                 t_token *tmp = head;
//                 head = head->next;
//                 free(tmp->value);
//                 free(tmp);
//             }
//             free(input_copy);
//             return NULL;
//         }
//         // Ajout du token à la liste
//         if (!head) {
//             head = new_token;
//             tail = new_token;
//         } else {
//             tail->next = new_token;
//             tail = new_token;
//         }
//     }

//     free(input_copy);
//     return head;
// }


// t_token *lexer(const char *input) {
//     t_token *head = NULL;
//     t_token *tail = NULL;
//     char *input_copy = strdup(input);
//     char *token = strtok(input_copy, " ");

//     while (token) {
//         t_token *new_token = NULL;

//         // Identifier le type de token
//         if (strcmp(token, ">") == 0) {
//             new_token = create_token(TOKEN_REDIRECT_OUTPUT, token);
//         } else if (strcmp(token, "<") == 0) {
//             new_token = create_token(TOKEN_REDIRECT_INPUT, token);
//         } else if (strcmp(token, ">>") == 0) {
//             new_token = create_token(TOKEN_REDIRECT_APPEND, token);
//         } else if (strcmp(token, "<<") == 0) {
//             new_token = create_token(TOKEN_HEREDOC, token);
//         } else {
//             new_token = create_token(TOKEN_COMMAND, token);
//         }
//         // Gestion d'erreur pour la création de tokens
//         if (!new_token) {
//             while (head) {
//                 t_token *tmp = head;
//                 head = head->next;
//                 free(tmp->value);
//                 free(tmp);
//             }
//             free(input_copy);
//             return NULL;
//         }
//         // Ajout du token à la liste
//         if (!head) {
//             head = new_token;
//             tail = new_token;
//         } else {
//             tail->next = new_token;
//             tail = new_token;
//         }
//         token = strtok(NULL, " ");
//     }
//     free(input_copy);
//     return head;
// }

