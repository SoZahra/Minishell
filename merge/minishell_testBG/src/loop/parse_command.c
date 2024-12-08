/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:58:29 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/08 19:04:33 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int validate_export_token(char *value)
{
    // Ne pas valider si c'est juste "export" sans arguments
    if (ft_strcmp(value, "export") == 0)
        return 1;

    // Vérifier les cas d'erreur
    if (ft_strcmp(value, "=") == 0)
        return 0;
    
    // Vérifier si commence par un chiffre
    if (ft_isdigit(value[0]))
        return 0;

    // Vérifier le tiret dans le nom (avant le =)
    char *equal_pos = ft_strchr(value, '=');
    char *dash_pos = ft_strchr(value, '-');
    if (dash_pos && (!equal_pos || dash_pos < equal_pos))
        return 0;

    return 1;
}

t_token *parse_command_line(char *line, t_ctx *ctx)
{
   t_token *special_case = handle_special_cases(line, ctx);
   if (special_case)
       return (special_case);
   if (check_invalid_quotes(line))
   {
       fprintf(stderr, "miniBG: syntax error near unexpected token `newline'\n");
       ctx->exit_status = 2;
       return NULL;
   }

   t_token *token_list = NULL;
   char buffer[1024] = {0};
   int i = 0;
   int j = 0;
   char current_quote_type = 0;
   char *temp = NULL;
   t_token_type temp_type = STRING;

   while (line[i])
   {
       if (i == 5 && ft_strncmp(line, "echo ", 5) == 0 &&
           ((line[i] == '"' && line[i + 1] == '$' && line[i + 2] == '"') ||
            (line[i] == '\'' && line[i + 1] == '$' && line[i + 2] == '\'')))
       {
           add_token(&token_list, STRING, "$");
           break;
       }

       if ((line[i] == '\'' || line[i] == '"') && !current_quote_type)
        {
            int had_previous_content = (j > 0);
            current_quote_type = line[i];
            i++;
            if (line[i] == current_quote_type)
            {
                if (ft_strncmp(line, "export ", 7) == 0)
                {
                    add_token(&token_list, STRING, "");
                    i++;
                    current_quote_type = 0;
                    continue;
                }
                else
                {
                    i++;
                    current_quote_type = 0;
                    continue;
                }
            }
            temp_type = (current_quote_type == '\'') ? SINGLE_QUOTE : DOUBLEQUOTE;
            if (j > 0)
            {
                buffer[j] = '\0';
                if (!temp)
                    temp = ft_strdup(buffer);
                else
                {
                    char *new_temp = ft_strjoin(temp, buffer);
                    free(temp);
                    temp = new_temp;
                }
                j = 0;
            }
            while (line[i] && line[i] != current_quote_type)
                buffer[j++] = line[i++];
            if (!line[i])
            {
                fprintf(stderr, "Error: unclosed quote\n");
                free(temp);
                free_tokens(token_list);
                return NULL;
            }
            buffer[j] = '\0';
            if (temp)
            {
                char *combined = ft_strjoin(temp, buffer);
                t_token *new_token = add_token(&token_list, temp_type, combined);
                if (!new_token)
                {
                    free(combined);
                    free(temp);
                    free_tokens(token_list);
                    ctx->exit_status = 1;
                    return (NULL);
                }
                if (new_token)
                {
                    new_token->had_space = !had_previous_content;
                    new_token->quoted = 1;
                }
                free(combined);
                free(temp);
                temp = NULL;
            }
            else
            {
                t_token *new_token = add_token(&token_list, temp_type, buffer);
                if (new_token)
                {
                    new_token->had_space = !had_previous_content;
                    new_token->quoted = 1;
                }
            }
            j = 0;
            i++;
            current_quote_type = 0;
        }
        else if (is_whitespace(line[i]))
        {
            if (j > 0)
            {
                buffer[j] = '\0';
                t_token *new_token = add_token(&token_list, STRING, buffer);
                new_token->had_space = 1;
                new_token->quoted = 0;
                j = 0;
            }
            while (is_whitespace(line[i]))
                i++;
        }
        else
            buffer[j++] = line[i++];
    }
    if (j > 0)
    {
        buffer[j] = '\0';
        t_token *new_token = add_token(&token_list, STRING, buffer);
        new_token->had_space = 0;
        new_token->quoted = 0;
    }

   t_token *current = token_list;
   while (current)
   {
       if (current->type == DOUBLEQUOTE || current->type == STRING)
       {
           char *expanded = expand_variables(current->value, ctx, current->type);
           if (expanded)
           {
               free(current->value);
               current->value = expanded;
           }
       }
       current = current->next;
   }

   // Validation des arguments export
   if (token_list && ft_strcmp(token_list->value, "export") == 0)
   {
       current = token_list->next;
       while (current)
       {
           // Vérifie le cas "export ="
           if (ft_strcmp(current->value, "=") == 0)
           {
               fprintf(stderr, "MiniBG: export: `=': not a valid identifier\n");
               ctx->exit_status = 1;
               free_tokens(token_list);
               return NULL;
           }

           // Vérifie si commence par un chiffre
           if (current->value[0] && ft_isdigit(current->value[0]))
           {
               fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n", 
                       current->value);
               ctx->exit_status = 1;
               free_tokens(token_list);
               return NULL;
           }

           // Vérifie la présence de tirets dans le nom avant le '='
           char *equal_pos = ft_strchr(current->value, '=');
           char *dash_pos = ft_strchr(current->value, '-');
           if (dash_pos && (!equal_pos || dash_pos < equal_pos))
           {
               fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n", 
                       current->value);
               ctx->exit_status = 1;
               free_tokens(token_list);
               return NULL;
           }

           current = current->next;
       }
   }

   return token_list;
}

// t_token_type get_quote_type(char quote)
// {
//     if (quote == '\'')
//         return SINGLE_QUOTE;
//     return DOUBLEQUOTE;
// }

// void handle_empty_quotes(char **line, int *i, t_token **token_list, char *current_quote_type)
// {
//     if (ft_strncmp(*line, "export ", 7) == 0)
//     {
//         add_token(token_list, STRING, "");
//     }
//     (*i)++; // Skip closing quote
//     *current_quote_type = 0;
// }

// int handle_quotes_content_(char **line, int *i, int *j, char *buffer, char quote_type)
// {
//     while ((*line)[*i] && (*line)[*i] != quote_type)
//         buffer[(*j)++] = (*line)[(*i)++];
//     if (!(*line)[*i])
//     {
//         fprintf(stderr, "Error: unclosed quote\n");
//         return 1;
//     }
//     return 0;
// }

// t_token *parse_command_line(char *line, t_ctx *ctx)
// {
//     t_token *special_case = handle_special_cases(line, ctx);
//     if (special_case)
//         return (special_case);
//     if (check_invalid_quotes(line))
//     {
//         fprintf(stderr, "miniBG: syntax error near unexpected token `newline'\n");
//         ctx->exit_status = 2;
//         return NULL;
//     }

//     return parse_tokens(line, ctx);
// }

// void handle_token_expansion(t_token *token_list, t_ctx *ctx)
// {
//     t_token *current = token_list;
//     while (current)
//     {
//         if (current->type == DOUBLEQUOTE || current->type == STRING)
//         {
//             char *expanded = expand_variables(current->value, ctx, current->type);
//             if (expanded)
//             {
//                 free(current->value);
//                 current->value = expanded;
//             }
//         }
//         current = current->next;
//     }
// }

// void handle_quoted_content(char *buffer, int *j, char *temp, t_token **list, t_token_type type)
// {
//     buffer[*j] = '\0';
//     if (temp)
//     {
//         char *combined = ft_strjoin(temp, buffer);
//         add_token(list, type, combined);
//         free(combined);
//         free(temp);
//     }
//     else
//         add_token(list, type, buffer);
//     *j = 0;
// }

// int handle_echo_dollar(char *line, int i, t_token **list)
// {
//     if (i == 5 && ft_strncmp(line, "echo ", 5) == 0 &&
//         ((line[i] == '"' && line[i + 1] == '$' && line[i + 2] == '"') ||
//          (line[i] == '\'' && line[i + 1] == '$' && line[i + 2] == '\'')))
//     {
//         add_token(list, STRING, "$");
//         return 1;
//     }
//     return 0;
// }

// t_token *parse_tokens(char *line, t_ctx *ctx)
// {
//     t_token *token_list = NULL;
//     // char buffer[1024] = {0};
//     int i = 0;
//     int j = 0;
//     char current_quote_type = 0;
//     char *temp = NULL;

//     if (parse_line(&i, &j, line, &token_list, &temp, &current_quote_type) != 0)
//         return NULL;
//     handle_token_expansion(token_list, ctx);
//     return token_list;
// }

// int parse_line(int *i, int *j, char *line, t_token **list, char **temp, char *quote_type)
// {
//     // t_token_type temp_type;

//     while (line[*i])
//     {
//         if (handle_echo_dollar(line, *i, list))
//             return 0;

//         if ((line[*i] == '\'' || line[*i] == '"') && !*quote_type)
//         {
//             if (!handle_quotes_(&line[*i], i, j, list, temp, quote_type))
//                 return 1;
//         }
//         else if (is_whitespace(line[*i]))
//         {
//             if (*j > 0)
//             {
//                 add_remaining_buffer(list, *j, line);
//                 *j = 0;
//             }
//             (*i)++;
//         }
//         else
//             line[(*j)++] = line[(*i)++];
//     }
//     if (*j > 0)
//         add_token(list, STRING, line);
//     return 0;
// }

// int handle_quotes_(char *line, int *i, int *j, t_token **list,
//                  char **temp, char *quote_type)
// {
//     *quote_type = line[0];
//     (*i)++;

//     if (line[1] == *quote_type)
//     {
//         handle_empty_quotes(&line, i, list, quote_type);
//         return 1;
//     }

//     t_token_type temp_type = get_quote_type(*quote_type);
//     if (handle_temp_buffer(j, *temp, line))
//         return 0;

//     if (handle_quotes_content_(&line, i, j, line, *quote_type))
//         return 0;

//     handle_quoted_content(line, j, *temp, list, temp_type);
//     handle_quote_end(i, quote_type, temp);
//     return 1;
// }

// void add_remaining_buffer(t_token **list, int j, char *buffer)
// {
//     buffer[j] = '\0';
//     add_token(list, STRING, buffer);
// }

// void handle_quote_end(int *i, char *quote_type, char **temp)
// {
//     (*i)++;
//     *quote_type = 0;
//     *temp = NULL;
// }

// int handle_temp_buffer(int *j, char *temp, char *buffer)
// {
//     if (*j > 0)
//     {
//         buffer[*j] = '\0';
//         if (!temp)
//         {
//             temp = ft_strdup(buffer);
//             return 0;
//         }
//         char *new_temp = ft_strjoin(temp, buffer);
//         free(temp);
//         temp = new_temp;
//         *j = 0;
//     }
//     return 1;
// }