/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:39:48 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/09 11:35:39 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// t_token *l_double_q(const char *input)
// {

// }

int handle_output_redic(const char **input, t_token *tokens, int *token_i)
{
    const char *current = *input;

    if (*current == '>')
    {
        if (*(current + 1) == '>') // double >>
        {
            tokens[*token_i].type = T_APPEND_OUT;
            tokens[*token_i].value = copy_token(current, current + 2);  // copie ">>"
            *input += 2;  // avance de 2 caractères
        }
        else if (*(current + 1) == '\0') // simple > sans rien derrière
            return (ft_printf_fd(2, "%ssyntax error near unexpected token `newline'\n", PROMPT), -1);
        else // simple > suivi d'un autre token (mot, autre caractère)
        {
            tokens[*token_i].type = T_REDIRECT_OUT;
            tokens[*token_i].value = copy_token(current, current + 1);  // copie ">"
            *input += 1;  // avance de 1 caractère
        }
        (*token_i)++;  // incrémente l'index des tokens
        return (0);
    }
    return (1);  // renvoie 1 si ce n'est pas une redirection
}


int handle_input_redic(const char **input, t_token *tokens, int *token_i)
{
    const char *current = *input;

    if(*current == '<')
    {
        if(*(current + 1) == '<') //double <<
        {
            tokens[*token_i].type = T_HEREDOC;
            tokens[*token_i].value = copy_token(current, current + 2);
            *input += 2;
        }
        else if (*(current + 1) == '\0')// dans le cas d'un simple < sans rien derriere
			return (ft_printf_fd(2, "%ssyntax error near unexpected token `newline'\n", PROMPT), -1);
        else //simple <
        {
            tokens[*token_i].type = T_REDIRECT_IN;
            tokens[*token_i].value = copy_token(current, current + 1);
            *input += 1;
        }
        (*token_i)++;
        return (0);
    }
    return(1);
}

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
        // perror("malloc");
		ft_printf_fd(2, "%s%s\n", PROMPT, strerror(errno));
        exit(EXIT_FAILURE);
    }
    token_i = 0;
    start = input;
    while (*input)
    {
        if (*input == '\'' && !in_double_quote)// Gérer les guillemets simples
        {
            tokens[token_i].type = T_SINGLE_QUOTE;
            tokens[token_i].value = copy_token(input, input + 1);  // Copier le guillemet simple
            token_i++;
            in_single_quote = !in_single_quote;
            input++;
            start = input;
            continue;
        }
        else if (*input == '"' && !in_single_quote)// Gérer les guillemets doubles
        {
            tokens[token_i].type = T_DOUBLE_QUOTE;
            tokens[token_i].value = copy_token(input, input + 1);  // Copier le guillemet double
            token_i++;
            in_double_quote = !in_double_quote;
            input++;
            start = input;
            continue;
        }
        if(*input == '>' || *input == '<')
        {
            if(handle_output_redic(&input, tokens, &token_i) == -1
                || handle_input_redic(&input, tokens, &token_i) == -1)
                return(free(tokens), NULL);
            start = input;
            continue;
        }
        if (in_single_quote || in_double_quote)// Si on est dans des guillemets doubles ou simples
        {
            // Chercher la fin du mot jusqu'à la fermeture des guillemets
            start = input;
            while (*input && ((*input != '\'' && in_single_quote) || (*input != '"' && in_double_quote)))
                input++;
            if (input > start)   // recup le mot entre les guillemets
            {
                tokens[token_i].type = T_WORD;
                tokens[token_i].value = copy_token(start, input);
                token_i++;
            }
            continue;
        }
        if (ft_isspace(*input) && !in_single_quote && !in_double_quote)// Si on est en dehors des guillemets et on rencontre un espace ou un token spécial
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
        // Ajouter la gestion d'autres types de tokens plus tard (redirections, etc.)
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
