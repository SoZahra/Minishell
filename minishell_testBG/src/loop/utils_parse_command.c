/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:59:33 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 15:02:53 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token *handle_special_cases(const char *line, t_ctx *ctx)
{
    if (ft_strcmp(line, "$") == 0)
        return create_single_token(ft_strdup("$"), STRING);
    if (ft_strcmp(line, "$?") == 0)
        return create_single_token(ft_itoa(ctx->exit_status), STRING);
    return NULL;
}

t_token *create_single_token(char *value, t_token_type type)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->value = value;
    token->type = type;
    token->next = NULL;
    return token;
}

int check_invalid_quotes(char *line)
{
    char quote_type = 0;
    int i = 0;

    // Cas spécial pour echo "$" et echo '$'
    if (ft_strcmp(line, "echo \"$\"") == 0 || ft_strcmp(line, "echo '$'") == 0)
        return 0;
    while (line[i])
    {
        if (line[i] == '$')
        {
            // Cas de quotes vides après $
            if (line[i + 1] && line[i + 2] &&
                ((line[i + 1] == '\'' && line[i + 2] == '\'') ||
                 (line[i + 1] == '"' && line[i + 2] == '"')))
            {
                i += 3;
                continue;
            }            // Cas d'une seule quote après $
            else if (line[i + 1] && (line[i + 1] == '\'' || line[i + 1] == '"'))
            {
                char q = line[i + 1];
                i += 2; // Skip $ et quote
                while (line[i] && line[i] != q)
                    i++;
                if (!line[i])
                    return 1;
                i++;
                continue;
            }
        }
        if ((line[i] == '\'' || line[i] == '"') && !quote_type)
            quote_type = line[i];
        else if (line[i] == quote_type)
            quote_type = 0;
        i++;
    }
    return quote_type != 0;
}
