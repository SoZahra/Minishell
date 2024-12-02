/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:01:19 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 15:01:27 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **prepare_args(t_token *tokens, t_ctx *ctx)
{
   (void)ctx;
   int count = 0;
   t_token *current = tokens;
   char **args;
   int i = 0;

   while (current)
   {
       count++;
       current = current->next;
   }
   args = malloc((count + 1) * sizeof(char *));
   if (!args)
   {
       perror("malloc failed");
       return NULL;
   }
   current = tokens;
   i = 0;
   while (current)
   {
    //    printf("Processing token %d: type = '%c'\n", i, (char)current->type);

       if (current->type == '\'') // Vérifie explicitement la quote simple
       {
        //    printf("Found single quote: using value '%s' without expansion\n", current->value);
           args[i] = ft_strdup(current->value);
       }
       else if (current->type == '"') // Vérifie explicitement la quote double
       {
        //    printf("Found double quote: expanding value '%s'\n", current->value);
           args[i] = expand_variables(current->value, ctx, current->type);
       }
       else // Cas normal (STRING ou autres)
       {
        //    printf("Found normal token: expanding value '%s'\n", current->value);
           args[i] = expand_variables(current->value, ctx, current->type);
       }

       if (!args[i])
       {
           // Gestion d'erreur...
           while (i > 0)
               free(args[--i]);
           free(args);
           return NULL;
       }
       i++;
       current = current->next;
   }
   args[i] = NULL;
   return args;
}