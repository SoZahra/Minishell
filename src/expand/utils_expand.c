/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:47:01 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/22 15:57:26 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// char	*handle_dollar(const char *str, t_ctx *ctx, const char **str_ptr,
// 		char *expanded)
// {
// 	char	*temp;

// 	(void)str;
// 	(*str_ptr)++;
// 	if (**str_ptr && (ft_isalpha(**str_ptr) || **str_ptr == '_'
// 			|| **str_ptr == '?'))
// 	{
// 		if (**str_ptr == '?')
// 		{
// 			temp = expand_exit_code(ctx, expanded);
// 			(*str_ptr)++;
// 		}
// 		else
// 			temp = expand_env_variable(str_ptr, ctx, expanded);
// 	}
// 	else
// 		temp = append_literal_dollar(expanded);
// 	return (temp);
// }

// char	*expand_exit_code(t_ctx *ctx, char *expanded)
// {
// 	char	*exit_code;
// 	char	*temp;

// 	exit_code = ft_itoa(ctx->exit_status);
// 	temp = ft_strjoin(expanded, exit_code);
// 	free(expanded);
// 	free(exit_code);
// 	return (temp);
// }

// char	*append_literal_dollar(char *expanded)
// {
// 	char	*temp;

// 	temp = ft_strjoin(expanded, "$");
// 	free(expanded);
// 	return (temp);
// }

// char	*handle_non_dollar(const char *str, const char **str_ptr,
// 		char *expanded)
// {
// 	char	curr[2];
// 	char	*temp;

// 	curr[0] = *str;
// 	curr[1] = '\0';
// 	temp = ft_strjoin(expanded, curr);
// 	free(expanded);
// 	(*str_ptr)++;
// 	return (temp);
// }
