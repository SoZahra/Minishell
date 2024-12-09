/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:48:48 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/09 15:29:45 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int is_valid_n(t_token *current)
{
    int i = 0;
	t_token *tmp = current;
	while (tmp)
	{
		while (tmp->value[i])
		{
			if (tmp->value[0] != '-')
				break;
			if (i != 0 && tmp->value[i] != 'n')
				break;
			if (tmp->value[i + 1] == '\0')
				return (1);
			i++;
		}
		tmp = tmp->next;
	}
	return (0);
}

void handle_echo(t_token *token_list, t_ctx *ctx)
{
    t_token *current;
    t_token *start;
    int n_option = 0;

    current = token_list;
	(void)ctx;
    while (current && is_valid_n(current))
    {
        n_option = 1;
        t_token *to_free = current;
        current = current->next;
        free(to_free->value);
        free(to_free->content);
        free(to_free);
    }
    start = current;
    write_echo_content(start, n_option);
    // ctx->exit_status = 0;
}

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

void write_echo_content(t_token *token_list, int n_option)
{
    t_token *current;
    t_token *next;
    char *cleaned;

    current = token_list;
    while (current)
    {
        next = current->next;
        if (current->value)
        {
            // Si le token suivant existe et si l'un des tokens contient une quote
            if (next && (strchr(current->value, '\'') || strchr(current->value, '"') ||
                        strchr(next->value, '\'') || strchr(next->value, '"')))
            {
                // Joindre les tokens sans espace
                cleaned = clean_dollar_quotes(current->value);
                write(STDOUT_FILENO, cleaned, ft_strlen(cleaned));
                free(cleaned);
            }
            else
            {
                // Sinon, écrire normalement avec un espace
                cleaned = clean_dollar_quotes(current->value);
                write(STDOUT_FILENO, cleaned, ft_strlen(cleaned));
                free(cleaned);
                if (next)
                    write(STDOUT_FILENO, " ", 1);
            }
        }
        current = current->next;
    }
    if (!n_option)
        write(STDOUT_FILENO, "\n", 1);
}

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
