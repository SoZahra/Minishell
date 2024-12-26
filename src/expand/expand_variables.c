/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:14:16 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/22 15:57:14 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// char	*expand_variables(const char *str, t_ctx *ctx, t_token_type token_type)
// {
// 	char	*expanded;

// 	if (token_type == SINGLE_QUOTE)
// 		return (ft_strdup(str));
// 	expanded = ft_strdup("");
// 	while (*str)
// 	{
// 		if (*str == '$' && token_type != SINGLE_QUOTE)
// 			expanded = handle_dollar(str, ctx, &str, expanded);
// 		else
// 			expanded = handle_non_dollar(str, &str, expanded);
// 	}
// 	return (expanded);
// }

// char	*expand_env_variable(const char **str_ptr, t_ctx *ctx, char *expanded)
// {
// 	char	buffer[1024];
// 	char	*value;
// 	char	*temp;
// 	int		i;

// 	i = 0;
// 	while ((*str_ptr)[i] && (ft_isalnum((*str_ptr)[i]) || (*str_ptr)[i] == '_'))
// 	{
// 		buffer[i] = (*str_ptr)[i];
// 		i++;
// 	}
// 	buffer[i] = '\0';
// 	*str_ptr += i;
// 	value = find_env_value(buffer, ctx->env_vars);
// 	if (value)
// 		temp = ft_strjoin(expanded, value);
// 	else
// 		temp = ft_strjoin(expanded, "");
// 	free(expanded);
// 	return (temp);
// }
