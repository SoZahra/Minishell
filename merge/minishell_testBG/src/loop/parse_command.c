/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:58:29 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 18:42:33 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *expand_full_string(const char *str, char quote_type, t_ctx *ctx)
{
    char *result;
    char *temp;
    char *var_name;
    char *var_value;
    int i;

    i = 0;
    result = ft_strdup("");
    while (str[i])
    {
        if (str[i] == '$')
        {
            if (str[i + 1] == '?')
            {
                char *exit_str = ft_itoa(ctx->exit_status);
                temp = ft_strjoin(result, exit_str);
                free(exit_str);
                free(result);
                result = temp;
                i += 2;
            }
            else if (str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
            {
                int j = i + 1;
                while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
                    j++;
                var_name = ft_substr(str, i + 1, j - (i + 1));
                var_value = getenv(var_name);
                free(var_name);
                if (var_value)
                {
                    temp = ft_strjoin(result, var_value);
                    free(result);
                    result = temp;
                }
                i = j;
            }
            else
            {
                if (quote_type != '\'')
                {
                    temp = ft_strjoin(result, "$");
                    free(result);
                    result = temp;
                }
                i++;
            }
        }
        else
        {
            char c[2] = {str[i], '\0'};
            temp = ft_strjoin(result, c);
            free(result);
            result = temp;
            i++;
        }
    }
    return result;
}

char *expand_variable_(const char *str, char quote_type, t_ctx *ctx)
{
    if (!str)
        return NULL;
    if (quote_type == '\'')
        return ft_strdup(str);
    if (quote_type == '"' || ft_strchr(str, '$'))
        return (expand_full_string(str, quote_type, ctx));
    return ft_strdup(str);
}

int expand_str(t_token *token, t_ctx *ctx)
{
    char *expanded;

    expanded = NULL;
    if (!token || !token->value)
        return -1;
    expanded = expand_full_string(token->value, token->type, ctx);
    if (!expanded)
        return -1;
    free(token->value);
    token->value = expanded;
    return 0;
}

int expand_proc(t_token **tokens, t_ctx *ctx)
{
    t_token *token;

    if (!tokens || !*tokens)
        return 0;
    token = *tokens;
    while (token)
    {
        if ((token->type == 'S' || token->type == '"') && ft_strchr(token->value, '$'))
        {
            if (expand_str(token, ctx) == -1)
                return -1;
        }
        token = token->next;
    }
    return 0;
}

int join_str(t_token *token, bool limiter)
{
    t_token *prev;

    if (!token || !token->prev)
        return 0;
    prev = token->prev;
    if (limiter)
    {
        if (!prev->prev || prev->prev->type != 'H')
            return 0;
        prev = prev->prev;
    }
    if (join_tokens(prev, token) == -1)
        return -1;
    token_del(token);
    return 0;
}

void token_del(t_token *token)
{
    if (!token)
        return;
    if (token->prev)
        token->prev->next = token->next;
    if (token->next)
        token->next->prev = token->prev;
    free(token->value);
    free(token->content);
    free(token);
}

int join_tokens(t_token *prev, t_token *current)
{
    char *tmp;
    char *joined;

    if (!prev || !current || !prev->value || !current->value)
        return -1;
    tmp = prev->value;
    joined = ft_strjoin(tmp, current->value);
    if (!joined)
        return -1;
    prev->value = joined;
    free(tmp);
    if (current->type == '"' || current->type == '\'')
        prev->type = current->type;
    return 0;
}


int join_proc(t_token **tokens, bool limiter)
{
    t_token *token;
    t_token *next;

    if (!tokens || !*tokens)
        return 0;
    token = *tokens;
    while (token)
    {
        next = token->next;
        if (token->had_space)
        {
            if (join_str(token, limiter) == -1)
                return -1;
        }
        token = next;
    }
    return 0;
}


char *tokens_to_string(t_token *tokens)
{
    if(!tokens)
        return (NULL);
    t_token *current;
    char *result;
    char *temp;

    current = tokens;
    result = ft_strdup("");
    if(!result)
        return (NULL);
    while (current)
    {
        if (*result && !current->had_space)
        {
            temp = ft_strjoin(result, " ");
            if (!temp)
                return (free(result),NULL);
            free(result);
            result = temp;
        }
        temp = ft_strjoin(result, current->value);
        if (!temp)
            return (free(result), NULL);
        free(result);
        result = temp;
        current = current->next;
    }
    return result;
}

bool is_token(char c, char *str)
{
    int i;

    i = -1;
    while (str[++i])
    {
        if (str[i] == c)
            return true;
    }
    return false;
}

t_token *get_last_token(t_token *token_list)
{
    t_token *current;

    if (!token_list)
        return (NULL);
    current = token_list;
    while (current->next)
        current = current->next;
    return (current);
}

int quotes_proc(t_token **tokens, char *input, int *i)
{
    char *value;
    char type;
    int j;

    type = input[(*i)++];
    j = (*i);
    while (input[(*i)] && input[(*i)] != type)
        (*i)++;
    if (input[(*i)++] != type)
        return (printf("error: syntax\n"), -1);
    value = ft_substr(input, j, *i - j - 1);
    if (!value)
        return -1;
    add_token(tokens, type, value);
    if (j > 1 && tokens && !ft_isspace(input[j - 2]) && !is_token(input[j - 2], UNJOIN))
        get_last_token(*tokens)->had_space = 1;
    return 0;
}
// > < |
int operators_proc(t_token **tokens, char *input, int *i, int n)
{
    char type;
    int x;
    int start;
    char *value;

    if (*tokens && is_token(get_last_token(*tokens)->type, OPERATORS))
        return (printf("error: syntax\n"), -1);
    start = *i;
    x = 1;
    type = input[(*i)++];
    while (input[*i] && input[*i] == type && x++ < n)
        (*i)++;
    value = ft_substr(input, start, *i - start);
    if (!value)
        return -1;
    if (type == '<' && x > 1)
        type = 'H';
    else if (type == '>' && x > 1)
        type = 'A';
    if(add_token(tokens, type, value) != 0)
        return (free(value), -1);
    return 0;
}

// x"y"'z'"a"'b'c>|>><"x"<<

int token_proc(t_token **tokens, char *input, int *i)
{
    if (input[*i] == '<' || input[*i] == '>' )
    {
        if (operators_proc(tokens, input, i, 2))
            return -1;
    }
    else if (input[*i] == '|')
    {
        if (operators_proc(tokens, input, i, 1))
            return -1;
    }
    else if (input[*i] == '"' || input[*i] == '\'')
    {
        if (quotes_proc(tokens, input, i))
            return -1;
    }
    return 0;
}

int word_proc(t_token **tokens, char *input, int *i)
{
    char *value;
    int j;
    int result;

    j = *i;
    while (input[(*i)] && !ft_isspace(input[(*i)]) && !is_token(input[*i], TOKENS))
        (*i)++;
    if (j == *i)
        return 0;
    value = ft_substr(input, j, *i - j);
    if (!value)
        return -1;
    result = add_token(tokens, 'S', value);
    if (result != 0)
        return (free(value), -1);
    if (j > 0 && input[j - 1] && !ft_isspace(input[j - 1]) && !is_token(input[j - 1], UNJOIN))
        get_last_token(*tokens)->had_space = 1;
    return 0;
}

int tokenizer(t_token **tokens, char *input)
{
    int i;
    int result;

    i = 0;
    *tokens = NULL;
    while (input[i])
    {
        if (input[i] != ' ' && input[i] != '\t')
        {
            if (is_token(input[i], TOKENS))
                result = token_proc(tokens, input, &i);
            else
                result = word_proc(tokens, input, &i);
            if (result != 0)
            {
                free_tokens(*tokens);
                *tokens = NULL;
                return -1;
            }
        }
        else if (input[i])
            i++;
    }
    return 0;
}

// int tokenizer(t_token **tokens, char *input)
// {
//     int i;

//     i = 0;
//     *tokens = NULL;
//     while (input[i])
//     {
//         if (input[i] != ' ' && input[i] != '\t')
//         {
//             if (is_token(input[i], TOKENS))
//             {
//                 if (token_proc(tokens, input, &i))
//                     return -1;
//             }
//             else
//             {
//                 if (word_proc(tokens, input, &i))
//                     return -1;
//             }
//         }
//         else if (input[i])
//             i++;
//     }
//     // print_tokens(*tokens);
//     return 0;
// }

//----------------------------------

// void remove_last_token(t_token **token_list)
// {
//     t_token *current;
//     t_token *prev;

//     if (!token_list || !*token_list)
//         return;

//     if (!(*token_list)->next)
//     {
//         free_tokens(*token_list);
//         *token_list = NULL;
//         return;
//     }

//     current = *token_list;
//     prev = NULL;
//     while (current->next)
//     {
//         prev = current;
//         current = current->next;
//     }
//     prev->next = NULL;
//     free(current->value);
//     free(current);
// }

// t_token *parse_command_line(char *line, t_ctx *ctx)
// {
//    t_token *special_case = handle_special_cases(line, ctx);
//    if (special_case)
//        return (special_case);
//    if (check_invalid_quotes(line))
//    {
//     printf("===============\n");
//        fprintf(stderr, "miniBG: syntax error near unexpected token `newline'\n");
//        ctx->exit_status = 2;
//        return NULL;
//    }

//    t_token *token_list = NULL;
//    char buffer[1024] = {0};
//    int i = 0;
//    int j = 0;
//    char current_quote_type = 0;
//    char *temp = NULL;
//    t_token_type temp_type = STRING;

//    while (line[i])
//    {
//        if (i == 5 && ft_strncmp(line, "echo ", 5) == 0 &&
//            ((line[i] == '"' && line[i + 1] == '$' && line[i + 2] == '"') ||
//             (line[i] == '\'' && line[i + 1] == '$' && line[i + 2] == '\'')))
//        {
//            add_token(&token_list, STRING, "$");
//            break;
//        }

//        if ((line[i] == '\'' || line[i] == '"') && !current_quote_type)
//         {
//             // int had_previous_content = (j > 0);
//             current_quote_type = line[i];
//             i++;
//             if (line[i] == current_quote_type)
//             {
//                 if (ft_strncmp(line, "export ", 7) == 0)
//                 {
//                     add_token(&token_list, STRING, "");
//                     i++;
//                     current_quote_type = 0;
//                     continue;
//                 }
//                 else
//                 {
//                     i++;
//                     current_quote_type = 0;
//                     continue;
//                 }
//             }
//             temp_type = (current_quote_type == '\'') ? SINGLE_QUOTE : DOUBLEQUOTE;
//             if (j > 0)
//             {
//                 buffer[j] = '\0';
//                 if (!temp)
//                     temp = ft_strdup(buffer);
//                 else
//                 {
//                     // char *new_temp = ft_strjoin(temp, buffer);
//                     // free(temp);
//                     // temp = new_temp;
//                 }
//                 j = 0;
//             }
//             while (line[i] && line[i] != current_quote_type)
//                 buffer[j++] = line[i++];
//             if (!line[i])
//             {
//                 fprintf(stderr, "Error: unclosed quote\n");
//                 free(temp);
//                 free_tokens(token_list);
//                 return NULL;
//             }
//             buffer[j] = '\0';
//             if (temp)
//             {
//                 // char *combined = ft_strjoin(temp, buffer);
//                 // char *combined = ft_strdup();
//                 t_token *new_token1 = add_token(&token_list, temp_type, temp);
//                 t_token *new_token2 = add_token(&token_list, temp_type, buffer);
//                 // t_token *new_token = add_token(&token_list, temp_type, combined);

//                 (void)new_token1;
//                 (void)new_token2;
//                 // (void)new_token;
//                 // if (new_token)
//                 // {
//                 //     // Fusionner avec le précédent token si applicable
//                 //     // t_token *prev = get_last_token(token_list);
//                 // //     if (prev && prev != new_token && prev->quoted && !new_token->had_space)
//                 // //     {
//                 // //         char *joined = ft_strjoin(prev->value, new_token->value);
//                 // //         free(prev->value);
//                 // //         prev->value = joined;
//                 // //         prev->type = temp_type;
//                 // //         remove_last_token(&token_list); // Supprime le nouveau token
//                 // //         free(new_token);
//                 // //     }
//                 // }
//                 // free(combined);
//                 free(temp);
//                 temp = NULL;
//             }
//             else
//             {
//                 t_token *new_token = add_token(&token_list, temp_type, buffer);
//                 if (new_token)
//                 {
//                     // Fusionner avec le précédent token si applicable
//                     // t_token *prev = get_last_token(token_list);
//                     // if (prev && prev != new_token && !new_token->had_space)
//                     // {
//                     //     char *joined = ft_strjoin(prev->value, new_token->value);
//                     //     free(prev->value);
//                     //     prev->value = joined;
//                     //     prev->type = temp_type;
//                     //     remove_last_token(&token_list); // Supprime le nouveau token
//                     //     free(new_token);
//                     // }
//                 }
//             }
//             j = 0;
//             i++;
//             current_quote_type = 0;
//         }
//         else if (is_whitespace(line[i]))
//         {
//             if (j > 0)
//             {
//                 buffer[j] = '\0';
//                 t_token *new_token = add_token(&token_list, STRING, buffer);
//                 new_token->had_space = 1;
//                 new_token->quoted = 0;
//                 j = 0;
//             }
//             while (is_whitespace(line[i]))
//                 i++;
//         }
//         else
//             buffer[j++] = line[i++];
//     }
//     if (j > 0)
//     {
//         buffer[j] = '\0';
//         t_token *new_token = add_token(&token_list, STRING, buffer);
//         new_token->had_space = 0;
//         new_token->quoted = 0;
//     }


//    t_token *current = token_list;

//    while (current)
//    {
//        if (current->type == DOUBLEQUOTE || current->type == STRING)
//        {
//            char *expanded = expand_variables(current->value, ctx, current->type);
//            if (expanded)
//            {
//                free(current->value);
//                current->value = expanded;
//            }
//        }
//        current = current->next;
//    }

//    // Validation des arguments export
//    if (token_list && ft_strcmp(token_list->value, "export") == 0)
//    {
//        current = token_list->next;
//        while (current)
//        {
//            // Vérifie le cas "export ="
//            if (ft_strcmp(current->value, "=") == 0)
//            {
//                fprintf(stderr, "MiniBG: export: `=': not a valid identifier\n");
//                ctx->exit_status = 1;
//                free_tokens(token_list);
//                return NULL;
//            }

//            // Vérifie si commence par un chiffre
//            if (current->value[0] && ft_isdigit(current->value[0]))
//            {
//                fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n",
//                        current->value);
//                ctx->exit_status = 1;
//                free_tokens(token_list);
//                return NULL;
//            }

//            // Vérifie la présence de tirets dans le nom avant le '='
//            char *equal_pos = ft_strchr(current->value, '=');
//            char *dash_pos = ft_strchr(current->value, '-');
//            if (dash_pos && (!equal_pos || dash_pos < equal_pos))
//            {
//                fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n",
//                        current->value);
//                ctx->exit_status = 1;
//                free_tokens(token_list);
//                return NULL;
//            }

//            current = current->next;
//        }
//    }

//    return token_list;
// }
