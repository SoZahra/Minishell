/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:37:16 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/09 17:14:26 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void loop(void)
{
    char *input;
    t_token *tokens;

    while (1)
    {
        input = readline(PROMPT);
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (*input)
            add_history(input);
        tokens = lexer(input);
        if (!tokens)  // Si une erreur est survenue pendant l'analyse (comme l'erreur de syntaxe)
        {
            free(input);  // Libérer l'entrée avant de recommencer
            continue;  // Retourne au début de la boucle, afficher le prompt à nouveau
        }
        process_tokens(tokens);
        free(input);
        free(tokens);  // N'oublie pas de libérer les tokens aussi
    }
}

