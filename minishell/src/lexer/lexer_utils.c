/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 17:41:00 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/11 11:33:40 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// t_token_type lex_get_type(char *str)
// {
//     if (*str == '|')
//         return T_PIPE;
//     if (*str == '<')
//         return (*(str + 1) == '<') ? T_HEREDOC : T_REDIRECT_IN;
//     if (*str == '>')
//         return (*(str + 1) == '>') ? T_APPEND_OUT : T_REDIRECT_OUT;
//     if (*str == '\'')
//         return T_SINGLE_QUOTE;
//     if (*str == '"')
//         return T_DOUBLE_QUOTE;
//     if (*str == '$')
//         return T_ENV_VAR;
//     return T_WORD;
// }

// int lex_get_len(char *str, t_token_type type)
// {
//     int len = 0;
//     char quote_char;

//     switch (type) {
//         case T_PIPE:
//         case T_REDIRECT_IN:
//         case T_REDIRECT_OUT:
//         case T_SINGLE_QUOTE:
//         case T_DOUBLE_QUOTE:
//             return 1;
//         case T_HEREDOC:
//         case T_APPEND_OUT:
//             return 2;
//         case T_WORD:
//         case T_ENV_VAR:
//             while (str[len] && !strchr(" \t|<>'\"\0", str[len]))
//                 len++;
//             return len;
//         // case T_SINGLE_QUOTE:
//         // case T_DOUBLE_QUOTE:
//             quote_char = str[0];
//             len = 1;
//             while (str[len] && str[len] != quote_char)
//                 len++;
//             return (str[len] == quote_char) ? len + 1 : len;
//         default:
//             return 0;
//     }
// }

// t_token *tok_create(char *value, int n, t_token_type type)
// {
//     t_token *token = malloc(sizeof(t_token));
//     if (!token)
//         return NULL;

//     token->value = strndup(value, n);
//     if (!token->value) {
//         free(token);
//         return NULL;
//     }

//     token->type = type;
//     token->next = NULL;
//     return token;
// }

// void tok_add_back(t_token **head, t_token *new)
// {
//     if (!*head)
//         *head = new;
//     else {
//         t_token *last = *head;
//         while (last->next)
//             last = last->next;
//         last->next = new;
//     }
// }
