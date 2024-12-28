/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:27:26 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 13:27:27 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_single_char(char c)
{
	if (is_token(c, REDIRS))
		return (printf("bash: syntax error near unexpected\
 token `newline'\n"), 1);
	if (c == '|')
		return (printf("bash: syntax error near unexpected token `|'\n"), 1);
	return (0);
}

static int	check_double_char(char c)
{
	if (c == '<' || c == '>')
		return (printf("bash: syntax error near unexpected\
 token `newline'\n"), 1);
	if (c == '|')
		return (printf("bash: syntax error near unexpected token `||'\n"), 1);
	return (0);
}

int	check_syntax_errors(const char *line)
{
	int	i;
	int	len;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (!line[i])
		return (0);
	len = ft_strlen(line) - 1;
	while (len > i && ft_isspace(line[len]))
		len--;
	if (i == len)
		return (check_single_char(line[i]));
	if (i == len - 1 && line[i] == line[i + 1])
		return (check_double_char(line[i]));
	return (0);
}

int	is_argument(t_token_type type)
{
	return (type == 'S' || type == '"' || type == '\'');
}

int	add_argument(t_command *cmd, t_token *token, int *arg_idx)
{
	if (!cmd || !cmd->args)
		return (1);
	cmd->args[*arg_idx] = ft_strdup(token->value);
	if (!cmd->args[*arg_idx])
		return (1);
	cmd->had_spaces[*arg_idx] = token->had_space;
	(*arg_idx)++;
	return (0);
}
