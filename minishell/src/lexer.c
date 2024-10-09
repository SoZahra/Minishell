/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:39:48 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/09 17:25:14 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// t_token *l_double_q(const char *input)
// {

// }

// int handle_output_redic(const char **input, t_token *tokens, int *token_i)
// {
//     const char *current = *input;

//     if (*current == '>')
//     {
//         if (*(current + 1) == '>') // double >>
//         {
//             tokens[*token_i].type = T_APPEND_OUT;
//             tokens[*token_i].value = copy_token(current, current + 2);  // copie ">>"
//             *input += 2;  // avance de 2 caractères
//         }
//         else if (*(current + 1) == '\0') // simple > sans rien derrière
//             return (ft_printf_fd(2, "%ssyntax error near unexpected token `newline'\n", PROMPT), -1);
//         else // simple > suivi d'un autre token (mot, autre caractère)
//         {
//             tokens[*token_i].type = T_REDIRECT_OUT;
//             tokens[*token_i].value = copy_token(current, current + 1);  // copie ">"
//             *input += 1;  // avance de 1 caractère
//         }
//         (*token_i)++;  // incrémente l'index des tokens
//         return (0);
//     }
//     return (1);  // renvoie 1 si ce n'est pas une redirection
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
        else
        {
            *input += 1;  // avance de 1 caractère pour passer le '>'
            while (ft_isspace(**input)) // ignorer les espaces après le '>'
                (*input)++;

            if (**input == '\0') // si on rencontre la fin de la ligne
            {
                ft_printf_fd(2, "%ssyntax error near unexpected token `newline'\n", PROMPT);
                return -1; // erreur de syntaxe
            }
            else if (**input == '>' || **input == '<' || **input == '|') // si on rencontre un autre opérateur
            {
                ft_printf_fd(2, "%ssyntax error near unexpected token `%c'\n", PROMPT, **input);
                return -1; // erreur de syntaxe
            }
            else // simple >
            {
                tokens[*token_i].type = T_REDIRECT_OUT;
                tokens[*token_i].value = copy_token(current, current + 1);  // copie ">"
            }
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

int ft_end_of_quote(const char *input)
{
	int	i;

	i = 1;
	while(input[i] != '"')
		i++;
	return (i);
}

// t_token *lexer(const char *input)
// {
//     int bufsize;
//     t_token *tokens;
//     int token_i;
//     const char *start;
//     int in_single_quote = 0, in_double_quote = 0;

//     token_i = 0;
//     bufsize = count_tokens(input);
//     tokens = malloc((bufsize + 1) * sizeof(t_token));  // Correct allocation
//     if (!tokens)
//     {
//         ft_printf_fd(2, "%s%s\n", PROMPT, strerror(errno));
//         exit(EXIT_FAILURE);
//     }
//     start = input;
//     while (*input)
//     {
//         if (*input == '\'' && !in_double_quote) // Gérer les guillemets simples
//         {
//             tokens[token_i].type = T_SINGLE_QUOTE;
//             tokens[token_i].value = copy_token(input, input + 1);  // Copier le guillemet simple
//             token_i++;
//             in_single_quote = !in_single_quote;
//             input++;  // Avancer d'un caractère
//             continue;
//         }
//         else if (*input == '"' && !in_single_quote) // Gérer les guillemets doubles
//         {
//             tokens[token_i].type = T_DOUBLE_QUOTE;
//             tokens[token_i].value = copy_token(input, input + 1);  // Copier le guillemet double
//             token_i++;
//             in_double_quote = !in_double_quote;
//             input++;  // Avancer d'un caractère
//             continue;
//         }
//         if (*input == '>' || *input == '<')
//         {
//             if (handle_output_redic(&input, tokens, &token_i) == -1
//                 || handle_input_redic(&input, tokens, &token_i) == -1)
//                 return (free(tokens), NULL);
//             start = input;
//             continue;
//         }
//         if (in_single_quote || in_double_quote) // Si on est dans des guillemets
//         {
//             start = input;
//             while (*input && ((*input != '\'' && in_single_quote) || (*input != '"' && in_double_quote)))
//                 input++;
//             if (input > start) // Récupérer le mot entre les guillemets
//             {
//                 tokens[token_i].type = T_WORD;
//                 tokens[token_i].value = copy_token(start, input);
//                 token_i++;
//             }
//             // Ajouter un token pour le guillemet de fermeture
//             if (*input == '"' && in_double_quote)
//             {
//                 tokens[token_i].type = T_DOUBLE_QUOTE;
//                 tokens[token_i].value = copy_token(input, input + 1);
//                 token_i++;
//                 in_double_quote = 0; // Terminer le guillemet double
//             }
//             else if (*input == '\'' && in_single_quote)
//             {
//                 tokens[token_i].type = T_SINGLE_QUOTE;
//                 tokens[token_i].value = copy_token(input, input + 1);
//                 token_i++;
//                 in_single_quote = 0; // Terminer le guillemet simple
//             }
//             continue;
//         }
//         if (ft_isspace(*input) && !in_single_quote && !in_double_quote) // Si on est en dehors des guillemets et on rencontre un espace
//         {
//             if (input > start)
//             {
//                 tokens[token_i].type = T_WORD;
//                 tokens[token_i].value = copy_token(start, input);
//                 token_i++;
//             }
//             while (ft_isspace(*input))  // Sauter les espaces
//                 input++;
//             start = input;
//         }
//         else if (*input == '|') // Gérer les pipes
//         {
//             tokens[token_i].type = T_PIPE;
//             tokens[token_i].value = copy_token(input, input + 1);
//             token_i++;
//             input++;
//             start = input;
//         }
//         else
//         {
//             input++;
//         }
//     }
//     // Ajouter le dernier token si nécessaire
//     if (start < input)
//     {
//         tokens[token_i].type = T_WORD;
//         tokens[token_i].value = copy_token(start, input);
//         token_i++;
//     }

//     // Terminer avec T_END
//     tokens[token_i].type = T_END;
//     tokens[token_i].value = NULL;

//     return tokens;
// }

t_token *lexer(const char *input)
{
    int bufsize;
    t_token *tokens;
    int token_i;
    const char *start;
    int in_single_quote = 0, in_double_quote = 0;

    token_i = 0;
    bufsize = count_tokens(input);
    tokens = malloc((bufsize + 1) * sizeof(t_token));  // Allocation correcte pour les tokens
    if (!tokens)
    {
        ft_printf_fd(2, "%s%s\n", PROMPT, strerror(errno));
        exit(EXIT_FAILURE);
    }
    start = input;

    while (*input)
    {
        // Gestion des guillemets simples
        if (*input == '\'' && !in_double_quote)
        {
            tokens[token_i].type = T_SINGLE_QUOTE;
            tokens[token_i].value = copy_token(input, input + 1);  // Copier le guillemet simple
            token_i++;
            in_single_quote = !in_single_quote;  // Toggle état du guillemet simple
            input++;  // Avancer d'un caractère
            continue;
        }

        // Gestion des guillemets doubles
        else if (*input == '"' && !in_single_quote)
        {
            tokens[token_i].type = T_DOUBLE_QUOTE;
            tokens[token_i].value = copy_token(input, input + 1);  // Copier le guillemet double
            token_i++;
            in_double_quote = !in_double_quote;  // Toggle état du guillemet double
            input++;  // Avancer d'un caractère
            continue;
        }

        // Gestion des redirections
        // if (*input == '>' || *input == '<')
        // {
        //     if (handle_output_redic(&input, tokens, &token_i) == -1
        //         || handle_input_redic(&input, tokens, &token_i) == -1)
        //     {
        //         free(tokens);
        //         return NULL;
        //     }
        //     start = input;  // Mettre à jour le début après avoir traité la redirection
        //     continue;
        // }
		if(*input == '>' || *input == '<')
        {
            if(handle_output_redic(&input, tokens, &token_i) == -1
                || handle_input_redic(&input, tokens, &token_i) == -1)
                return(free(tokens), NULL);
            start = input;
            continue;
        }
        // Gestion des mots dans des guillemets
        if (in_single_quote || in_double_quote)
        {
            start = input;
            while (*input && ((*input != '\'' && in_single_quote) || (*input != '"' && in_double_quote)))
                input++;

            if (input > start)  // Si un mot est trouvé entre les guillemets
            {
                tokens[token_i].type = T_WORD;
                tokens[token_i].value = copy_token(start, input);  // Copier le mot sans les guillemets
                token_i++;
            }

            // Fermer le guillemet
            if (*input == '"' && in_double_quote)
            {
                tokens[token_i].type = T_DOUBLE_QUOTE;
                tokens[token_i].value = copy_token(input, input + 1);
                token_i++;
                in_double_quote = 0;  // Fermer le guillemet double
            }
            else if (*input == '\'' && in_single_quote)
            {
                tokens[token_i].type = T_SINGLE_QUOTE;
                tokens[token_i].value = copy_token(input, input + 1);
                token_i++;
                in_single_quote = 0;  // Fermer le guillemet simple
            }

            input++;  // Avancer après le guillemet fermant
            start = input;  // Réinitialiser le début pour le prochain token
            continue;
        }

        // Gestion des espaces hors guillemets
        if (ft_isspace(*input) && !in_single_quote && !in_double_quote)
        {
            if (input > start)  // Si un mot a été trouvé avant l'espace
            {
                tokens[token_i].type = T_WORD;
                tokens[token_i].value = copy_token(start, input);
                token_i++;
            }

            while (ft_isspace(*input))  // Ignorer les espaces successifs
                input++;
            start = input;  // Réinitialiser le début après les espaces
            continue;
        }

        // Gestion des pipes
        else if (*input == '|')
        {
            tokens[token_i].type = T_PIPE;
            tokens[token_i].value = copy_token(input, input + 1);  // Copier le pipe
            token_i++;
            input++;
            start = input;
            continue;
        }

        input++;  // Avancer d'un caractère si rien d'autre n'est traité
    }

    // Ajouter le dernier token s'il reste quelque chose
    if (start < input)
    {
        tokens[token_i].type = T_WORD;
        tokens[token_i].value = copy_token(start, input);
        token_i++;
    }

    // Ajouter le token de fin
    tokens[token_i].type = T_END;
    tokens[token_i].value = NULL;

    return tokens;
}











