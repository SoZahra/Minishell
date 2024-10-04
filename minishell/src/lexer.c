/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:39:48 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/04 17:58:03 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// t_token *lexer(const char *input)
// {
// 	int bufsize;
// 	t_token *tokens;
// 	int token_i;
// 	const char *start;

// 	bufsize = count_tokens(input);
// 	tokens = malloc((bufsize + 1) * sizeof(t_token));
// 	token_i = 0;
// 	start = input;

// 	while(*input)
// 	{
// 		t_token_type token_type = identify_token(input);
// 		if(token_type == T_WHITESPACE)
// 		{
// 			while(ft_isspace(*input))
// 				input++;
// 			start = input;
// 			continue;
// 		}
// 		else if(token_type != T_WORD) //pour les autres types de tokens comme pipe, redirection,...
// 		{
// 			tokens[token_i].type = token_type;
// 			tokens[token_i].value = copy_token(input, input + 1);
// 			input++;
// 			token_i++;
// 		}
// 		else //pour les mots ou les sequences
// 		{
// 			while(*input && !ft_isspace(*input) && !identify_token(input))
// 				input++;
// 			tokens[token_i].type = T_WORD;
// 			tokens[token_i].value = copy_token(start, input);
// 			token_i++;
// 		}
// 	}
// 	tokens[token_i].type = -1; //fin des tokens
// 	tokens[token_i].value = NULL;
// 	return (tokens);
// }

t_token *lexer(const char *input)
{
    int bufsize;
    t_token *tokens;
    int token_i;
    const char *start;
    int in_single_quote = 0, in_double_quote = 0;

    bufsize = count_tokens(input);
    tokens = malloc((bufsize + 1) * sizeof(t_token));  // Correct allocation
    if (!tokens)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    token_i = 0;
    start = input;
    while (*input)
    {
        // Gérer les guillemets
        if (*input == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (*input == '"' && !in_single_quote)
            in_double_quote = !in_double_quote;

        // Si on est en dehors des guillemets et on rencontre un espace ou un token spécial
        if (ft_isspace(*input) && !in_single_quote && !in_double_quote)
        {
            if (input > start)
            {
                tokens[token_i].type = T_WORD;
                tokens[token_i].value = copy_token(start, input);
                token_i++;
            }
            while (ft_isspace(*input))  // Sauter les espaces
                input++;
            start = input;
        }
        else if (*input == '|')
        {
            tokens[token_i].type = T_PIPE;
            tokens[token_i].value = copy_token(input, input + 1);
            token_i++;
            input++;
            start = input;
        }
        // Ajouter la gestion d'autres types de tokens (redirections, etc.)
        else
        {
            input++;
        }
    }

    // Ajouter le dernier token si nécessaire
    if (start < input)
    {
        tokens[token_i].type = T_WORD;
        tokens[token_i].value = copy_token(start, input);
        token_i++;
    }

    // Terminer avec T_END
    tokens[token_i].type = T_END;
    tokens[token_i].value = NULL;

    return tokens;
}
