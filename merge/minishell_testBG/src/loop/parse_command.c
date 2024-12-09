/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:58:29 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/09 15:35:03 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int validate_export_token(char *value)
// {
//     // Ne pas valider si c'est juste "export" sans arguments
//     if (ft_strcmp(value, "export") == 0)
//         return 1;

//     // Vérifier les cas d'erreur
//     if (ft_strcmp(value, "=") == 0)
//         return 0;

//     // Vérifier si commence par un chiffre
//     if (ft_isdigit(value[0]))
//         return 0;

//     // Vérifier le tiret dans le nom (avant le =)
//     char *equal_pos = ft_strchr(value, '=');
//     char *dash_pos = ft_strchr(value, '-');
//     if (dash_pos && (!equal_pos || dash_pos < equal_pos))
//         return 0;

//     return 1;
// }

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

void remove_last_token(t_token **token_list)
{
    t_token *current;
    t_token *prev;

    if (!token_list || !*token_list)
        return;

    if (!(*token_list)->next)
    {
        free_tokens(*token_list);
        *token_list = NULL;
        return;
    }

    current = *token_list;
    prev = NULL;
    while (current->next)
    {
        prev = current;
        current = current->next;
    }
    prev->next = NULL;
    free(current->value);
    free(current);
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
            // int had_previous_content = (j > 0);
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

                if (new_token)
                {
                    // Fusionner avec le précédent token si applicable
                    t_token *prev = get_last_token(token_list);
                    if (prev && prev != new_token && prev->quoted && !new_token->had_space)
                    {
                        char *joined = ft_strjoin(prev->value, new_token->value);
                        free(prev->value);
                        prev->value = joined;
                        prev->type = temp_type;
                        remove_last_token(&token_list); // Supprime le nouveau token
                        free(new_token);
                    }
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
                    // Fusionner avec le précédent token si applicable
                    t_token *prev = get_last_token(token_list);
                    if (prev && prev != new_token && !new_token->had_space)
                    {
                        char *joined = ft_strjoin(prev->value, new_token->value);
                        free(prev->value);
                        prev->value = joined;
                        prev->type = temp_type;
                        remove_last_token(&token_list); // Supprime le nouveau token
                        free(new_token);
                    }
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

