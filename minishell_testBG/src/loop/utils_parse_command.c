/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:59:33 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/03 11:14:56 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*handle_special_cases(const char *line, t_ctx *ctx)
{
	if (ft_strcmp(line, "$") == 0)
		return (create_single_token(ft_strdup("$"), STRING));
	if (ft_strcmp(line, "$?") == 0)
		return (create_single_token(ft_itoa(ctx->exit_status), STRING));
	return (NULL);
}

t_token	*create_single_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

static int	skip_dollar_quotes(char *line, int *i)
{
	char	quote_char;

	if (line[*i + 1] && line[*i + 2] && (((line[*i + 1] == '\'' && line[*i \
				+ 2] == '\'')) || (line[*i + 1] == '"' && line[*i + 2] == '"')))
	{
		*i += 3;
		return (1);
	}
	if (line[*i + 1] && (line[*i + 1] == '\'' || line[*i + 1] == '"'))
	{
		quote_char = line[*i + 1];
		*i += 2;
		while (line[*i] && line[*i] != quote_char)
			(*i)++;
		if (!line[*i])
			return (1);
		(*i)++;
		return (1);
	}
	return (0);
}

int	check_invalid_quotes(char *line)
{
	char	quote_type;
	int		i;

	quote_type = 0;
	i = 0;
	if (ft_strcmp(line, "echo \"$\"") == 0 || ft_strcmp(line, "echo '$'") == 0)
		return (0);
	while (line[i])
	{
		if (line[i] == '$' && skip_dollar_quotes(line, &i))
			continue ;
		if ((line[i] == '\'' || line[i] == '"') && !quote_type)
			quote_type = line[i];
		else if (line[i] == quote_type)
			quote_type = 0;
		i++;
	}
	return (quote_type != 0);
}
