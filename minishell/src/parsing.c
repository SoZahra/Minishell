/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:34:21 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/07 16:05:57 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int c_tokens(const char *input)
{
	int count;
	int simple_quote;
	int double_quote;

	count = ((simple_quote = double_quote = 0));

	while(*input)
	{
		if(*input == '\'')
			simple_quote = !simple_quote;
		else if(*input == '"')
			double_quote = !double_quote;
		else if(ft_isspace(*input) && !simple_quote && !double_quote)
		{
			count++;
			while(ft_isspace(*input))
				input++;
			continue;
		}
		input++;
	}
	return(count + 1);
}

///  fonction qui gere la gestion des guillemets
///	et pourra être réutilisée à plusieurs endroits.
int toggle_quote(int in_quote, char quote_char, char current_char)
{
	if(current_char == quote_char)
		return !in_quote;
	return(in_quote);
}

// Cette fonction compte le nombre de tokens
// dans une chaîne, en tenant compte des guillemets.
int count_tokens(const char *input)
{
    int count = 0;
    int simple_quote = 0, double_quote = 0;

    while (*input)
    {
        simple_quote = toggle_quote(simple_quote, '\'', *input);
        double_quote = toggle_quote(double_quote, '"', *input);

        if (ft_isspace(*input) && !simple_quote && !double_quote)
        {
            count++;
            while (ft_isspace(*input))
                input++;
            continue;
        }
        input++;
    }
    return count + 1;
}

/// On a besoin d'une fonction dédiée pour copier un token extrait du texte.
char *copy_token(const char *start, const char *end)
{
    int len = end - start;
    if(len < 0)
    {
        fprintf(stderr, "Error : invalid token length\n");
        return NULL;
    }
    char *token = malloc(len + 1);
    if (!token)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strncpy(token, start, len);
    token[len] = '\0';
    return token;
}


// char **parse_command(const char *input)
// {
//     int bufsize = count_tokens(input); // nb de tokens
//     t_token *tokens = malloc((bufsize + 1) * sizeof(t_token)); // allouer selon le nb de tokens + 1 pour le NULL
//     int token_index = 0;
//     const char *start = input;
//     int simple_quote = 0, double_quote = 0;

//     while (*input)
//     {
//         // in_single_quote = toggle_quote(in_single_quote, '\'', *input);
//         // in_double_quote = toggle_quote(in_double_quote, '"', *input);

//         if (ft_isspace(*input) && !simple_quote && !double_quote)
//         {
//             if (input > start)
//                 tokens[token_index++] = copy_token(start, input);
//             while (ft_isspace(*input))
//                 input++;
//             start = input;
//             continue;
//         }
//         input++;
//     }
//     if (start < input)
//         tokens[token_index++] = copy_token(start, input);
//     tokens[token_index] = NULL;
//     return tokens;
// }

t_token *parse_command(const char *input)
{
    int bufsize = count_tokens(input);  // On calcule le nombre de tokens.
    t_token *tokens = malloc((bufsize + 1) * sizeof(t_token));  // On alloue pour tous les tokens +1 pour NULL.
    int token_index = 0;
    const char *start = input;
    int in_single_quote = 0, in_double_quote = 0;

    while (*input)
    {
        in_single_quote = toggle_quote(in_single_quote, '\'', *input);
        in_double_quote = toggle_quote(in_double_quote, '"', *input);
        if (ft_isspace(*input) && !in_single_quote && !in_double_quote)
        {
            if (input > start)
            {
                tokens[token_index].type = T_WORD;
                tokens[token_index].value = copy_token(start, input);
                token_index++;
            }
            while (ft_isspace(*input))
                input++;

            start = input;
            continue;
        }
        else if (*input == '|')
        {
            tokens[token_index].type = T_PIPE;
            tokens[token_index].value = copy_token(input, input + 1);
            token_index++;
            input++;
            start = input;
            continue;
        }// Gérer les autres types de tokens ici...
        input++;
    }
    if (start < input)
    {
        tokens[token_index].type = T_WORD;
        tokens[token_index].value = copy_token(start, input);
        token_index++;
    }// Ajouter le token TOKEN_END à la fin
    tokens[token_index].type = T_END;
    tokens[token_index].value = NULL;

    return tokens;
}

// void process_tokens(t_token *tokens)
// {
//     int i = 0;
//     while (tokens[i].type != T_END)  // Comparer avec T_END au lieu de -1
//     {
//         printf("Token type: %d, value: %s\n", tokens[i].type, tokens[i].value);
//         i++;
//     }
// }

const char *token_type_to_string(t_token_type type)
{
    switch (type)
    {
        case T_WORD:           return "WORD";
        case T_PIPE:           return "PIPE";
        case T_REDIRECT_IN:    return "REDIRECT_IN";
        case T_REDIRECT_OUT:   return "REDIRECT_OUT";
        case T_APPEND_OUT:     return "APPEND_OUT";
        case T_HEREDOC:        return "HEREDOC";
        case T_ENV_VAR:        return "ENV_VAR";
        case T_SINGLE_QUOTE:   return "SINGLE_QUOTE";
        case T_DOUBLE_QUOTE:   return "DOUBLE_QUOTE";
        case T_WHITESPACE:     return "WHITESPACE";
        case T_END:            return "END";
        default:               return "UNKNOWN";
    }
}

void process_tokens(t_token *tokens)
{
    int i = 0;
    while (tokens[i].type != T_END)
    {
        // Afficher un type de token lisible par l'humain
        printf("Token type: %s, value: %s\n", token_type_to_string(tokens[i].type), tokens[i].value);
        i++;
    }
}

// Fonction pour convertir le type de token en chaîne lisible

