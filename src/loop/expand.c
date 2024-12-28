/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 16:31:47 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 16:32:13 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_dollar(char *result, char quote_type)
{
	char	*temp;

	if (quote_type != '\'')
	{
		temp = ft_strjoin(result, "$");
		free(result);
		return (temp);
	}
	return (result);
}

char	*expand_full_string(const char *str, char quote_type, t_ctx *ctx)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
				result = handle_exit_status(result, ctx, &i);
			else if (str[i + 1] && (ft_isalnum(str[i + 1])
					|| str[i + 1] == '_'))
				result = handle_env_var(result, str, &i, ctx);
			else
			{
				result = handle_dollar(result, quote_type);
				i++;
			}
		}
		else
			result = handle_regular_char(result, str[i++]);
	}
	return (result);
}

int	expand_str(t_token *token, t_ctx *ctx)
{
	char	*expanded;

	expanded = NULL;
	if (!token || !token->value)
		return (1);
	expanded = expand_full_string(token->value, token->type, ctx);
	if (!expanded)
		return (-1);
	free(token->value);
	token->value = expanded;
	return (0);
}
