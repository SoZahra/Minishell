/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:14:16 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 15:14:34 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *expand_variables(const char *str, t_ctx *ctx, t_token_type token_type)
{
    if (token_type == SINGLE_QUOTE)
        return ft_strdup(str);

    char *expanded = ft_strdup("");
    char *temp;

    while (*str)
    {
        if (*str == '$' && token_type != SINGLE_QUOTE)
        {
            str++; // Skip the $
            // Vérifier si le caractère suivant est valide pour une variable
            if (*str && (ft_isalpha(*str) || *str == '_' || *str == '?'))
            {
                if (*str == '?') // Cas spécial pour $?
                {
                    char *exit_code = ft_itoa(ctx->exit_status);
                    temp = ft_strjoin(expanded, exit_code);
                    free(expanded);
                    free(exit_code);
                    expanded = temp;
                    str++;
                }
                else // Variable normale
                {
                    char buffer[1024];
                    int i = 0;
                    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                    {
                        buffer[i] = str[i];
                        i++;
                    }
                    buffer[i] = '\0';
                    str += i;

                    char *value = find_env_value(buffer, ctx->env_vars);
                    temp = ft_strjoin(expanded, value ? value : "");
                    free(expanded);
                    expanded = temp;
                }
            }
            else // Si le caractère suivant n'est pas valide, on garde le $
            {
                temp = ft_strjoin(expanded, "$");
                free(expanded);
                expanded = temp;
            }
        }
        else
        {
            char curr[2] = {*str, '\0'};
            temp = ft_strjoin(expanded, curr);
            free(expanded);
            expanded = temp;
            str++;
        }
    }
    return expanded;
}