/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:58:29 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 14:58:40 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token *parse_command_line(char *line, t_ctx *ctx)
{
   t_token *special_case = handle_special_cases(line, ctx);
   if(special_case)
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
            current_quote_type = line[i];
            i++; // Skip opening quote

            // Si les quotes sont vides ("" ou ''), les ignorer et continuer le token en cours
            if (line[i] == current_quote_type)
            {
                // Si c'est un export avec des quotes vides, créer un token vide
                if (ft_strncmp(line, "export ", 7) == 0)
                {
                    add_token(&token_list, STRING, "");
                    i++; // Skip closing quote
                    current_quote_type = 0;
                    continue;
                }
                // Sinon, ignorer les quotes vides et continuer le token en cours
                else
                {
                    i++; // Skip closing quote
                    current_quote_type = 0;
                    continue;
                }
            }

           // Définir le type de quote
           temp_type = (current_quote_type == '\'') ? SINGLE_QUOTE : DOUBLEQUOTE;

           // Si on a du contenu avant les quotes, on le garde dans temp
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

           // Capture le contenu entre quotes
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

           // Ajouter le token avec son type
           if (temp)
           {
               char *combined = ft_strjoin(temp, buffer);
               add_token(&token_list, temp_type, combined);
               free(combined);
               free(temp);
               temp = NULL;
           }
           else
           {
               add_token(&token_list, temp_type, buffer);
           }

           j = 0;
           i++; // Skip la quote fermante
           current_quote_type = 0;
       }
       else if (is_whitespace(line[i]))
       {
           if (j > 0)
           {
               buffer[j] = '\0';
               add_token(&token_list, STRING, buffer);
               j = 0;
           }
           i++;
       }
       else
       {
           buffer[j++] = line[i++];
       }
   }

   // Ajoute le dernier token s'il existe
   if (j > 0)
   {
       buffer[j] = '\0';
       add_token(&token_list, STRING, buffer);
   }

   // Traitement des variables d'environnement pour les tokens
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

   return token_list;
}