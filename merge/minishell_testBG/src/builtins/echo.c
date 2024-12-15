/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:48:48 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/15 17:59:05 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


//fonctionne
int is_valid_n(t_token *current)
{
    int i;

    i = 0;
    while (current->value[i])
    {
        if (current->value[0] != '-')
            return (0);
        if (i != 0 && current->value[i] != 'n')
            return (0);
        i++;
    }
    if (current->value[0] == '-')
        return (1);
    return 0;
}

// int is_valid_n(t_token *current)
// {
//     // Le token doit commencer par '-'
//     if (current->value[0] != '-')
//         return 0;

//     // Vérifier que tous les caractères sont 'n'
//     for (int i = 1; current->value[i]; i++)
//     {
//         if (current->value[i] != 'n')
//             return 0;
//     }
    
//     return 1;
// }



// void    write_echo_content(t_token *token_list, int n_option)
// {
//     t_token *current;
//     t_token *next;
//     char    *cleaned;

//     current = token_list;
//     while (current)
//     {
//         next = current->next;
//         if (current->value)
//         {
//             cleaned = clean_dollar_quotes(current->value);
//             write(STDOUT_FILENO, cleaned, ft_strlen(cleaned));
//             free(cleaned);
//         }
// 		if (next && current->had_space)
//             write(STDOUT_FILENO, " ", 1);
//         current = current->next;
//     }
//     if (!n_option)
//         write(STDOUT_FILENO, "\n", 1);
// }

char *ft_strstr(const char *str, const char *to_find)
{
    size_t i;
    size_t j;

    if (!*to_find)
        return ((char *)str);
    i = 0;
    while (str[i])
    {
        j = 0;
        while (to_find[j] && str[i + j] && str[i + j] == to_find[j])
            j++;
        if (!to_find[j])
            return ((char *)&str[i]);
        i++;
    }
    return (NULL);
}

// void write_echo_content(t_token *tokens, int n_option)
// {
//     t_token *current = tokens;

//     // Boucle sur les tokens en ignorant le premier
//     while (current)
//     {
//         if (current->value)
//         {
//             // Écrire la valeur du token
//             write(STDOUT_FILENO, current->value, ft_strlen(current->value));

//             // Ajouter un espace après chaque token, sauf le dernier
//             if (current->next)
//                 write(STDOUT_FILENO, " ", 1);
//         }
//         current = current->next;
//     }
//     // Ajouter une nouvelle ligne si l'option `-n` n'est pas présente
//     if (!n_option)
//         write(STDOUT_FILENO, "\n", 1);
// }

// void write_echo_content(t_token *token_list, int n_option)
// {
//     t_token *current;
//     t_token *next;
//     char *cleaned;

//     current = token_list;
//     while (current)
//     {
//         next = current->next;
//         if (current->value)
//         {
//             cleaned = clean_dollar_quotes(current->value);
//             write(STDOUT_FILENO, cleaned, ft_strlen(cleaned));
//             free(cleaned);
//         }

//         // Mettre un espace seulement si
//         // - Il y a un token suivant
//         // - ET had_space est à 1 (indiquant qu'il y avait un espace dans la commande originale)
//         if (next && current->had_space)
//             write(STDOUT_FILENO, " ", 1);

//         current = current->next;
//     }
//     if (!n_option)
//         write(STDOUT_FILENO, "\n", 1);
// }
