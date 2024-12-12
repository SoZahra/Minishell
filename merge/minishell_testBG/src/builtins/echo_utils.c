/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:53:13 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/08 19:30:54 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// char	*clean_dollar_quotes(const char *str)
// {
// 	char		quote_type;
// 	const char	*content_start;
// 	const char	*content_end;

// 	quote_type = str[1];
// 	content_start = str + 2;
// 	content_end = ft_strchr(content_start, quote_type);
// 	if (!str || ft_strlen(str) < 4)
// 		return (ft_strdup(str));
// 	if (str[0] == '$' && (str[1] == '"' || str[1] == '\''))
// 	{
// 		if (content_end)
// 			return (ft_strndup(content_start, content_end - content_start));
// 	}
// 	return (ft_strdup(str));
// }

char *clean_dollar_quotes(const char *str)
{
    if (!str)
        return NULL;

    // Si la chaîne est entre simples quotes
    if (str[0] == '\'' && str[strlen(str) - 1] == '\'')
        return ft_strdup(str);  // Garder tel quel pour les simples quotes

    // Si c'est un cas de $' ou $"
    if (str[0] == '$' && (str[1] == '"' || str[1] == '\''))
    {
        char quote_type = str[1];
        const char *content_start = str + 2;
        const char *content_end = ft_strchr(content_start, quote_type);
        
        if (content_end && ft_strlen(str) >= 4)
            return ft_strndup(content_start, content_end - content_start);
    }

    return ft_strdup(str);
}
