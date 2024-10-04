/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:39:03 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/04 15:39:26 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token_type identify_token(const char *str)
{
    if(*str == '|')
        return(T_PIPE);
    else if(*str == '<' && *(str + 1) == '<')
        return(T_HEREDOC);
    else if(*str == '>' && *(str + 1) == '>')
        return(T_APPEND_OUT);
    else if(*str == '<')
        return(T_REDIRECT_IN);
    else if(*str == '>')
        return(T_REDIRECT_OUT);
    else if(*str == '$')
        return(T_ENV_VAR);
    else if(*str == '\'')
        return(T_SINGLE_QUOTE);
    else if(*str == '"')
        return(T_DOUBLE_QUOTE);
    else if(ft_isspace(*str))
        return(T_WHITESPACE);
    return (T_WORD);
}