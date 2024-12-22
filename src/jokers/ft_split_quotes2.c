/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 20:10:32 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/22 12:14:55 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_squote(const char *str, size_t *i, t_ctx *data);
int	parse_dquote(const char *str, size_t *i, t_ctx *data);

int	check_quotes(char c, int *quote_type, t_ctx *data)
{
	(void)data;
	if (c == '\'')
		*quote_type = 1;
	else if (c == '"')
		*quote_type = 2;
	return (*quote_type);
}

int	parse_quotes(const char *str, int quote_type, size_t *i,
	t_ctx *data)
{
	if (quote_type == 1)
		return (parse_squote(str, i, data));
	else if (quote_type == 2)
		return (parse_dquote(str, i, data));
	return (0);
}

//else if (str[*i] == '\'' && !passed)
//	passed = 1;
int	parse_squote(const char *str, size_t *i, t_ctx *data)
{
	(void)data;
	while (str[*i])
	{
		if (str[*i] == '\'' && (!str[*i + 1] || str[*i + 1] == ' '))
			return (1);
		else if (str[*i] == '\'')
		{
			while (str[*i] && !is_sep_joker(str[*i]))
				(*i)++;
			return (1);
		}
		(*i)++;
	}
	return (-2);
}

/*
		Bon"jour mec MOUAH"AHAHH "why are you raging ??" "" A"urevoir "
*/

//else if (str[*i] == '"' && !passed)
//	passed = 1;
int	parse_dquote(const char *str, size_t *i, t_ctx *data)
{
	(void)data;
	while (str[*i])
	{
		if (str[*i] == '"' && (!str[*i + 1] || str[*i + 1] == ' '))
			return (1);
		else if (str[*i] == '"')
		{
			while (str[*i] && !is_sep_joker(str[*i]))
				(*i)++;
			return (1);
		}
		(*i)++;
	}
	return (-1);
}

size_t	ft_count_words_quotes(const char *s, t_ctx *data)
{
	size_t	words;
	size_t	i;
	char	quote_status;

	(void)data;
	quote_status = 0;
	words = 0;
	i = 0;
	while (s[i])
	{
		check_quote_status(s[i], &quote_status);
		if (!is_sep_joker(s[i]) && (!s[i + 1] || is_sep_joker(s[i + 1])) && !quote_status)
			words++;
		i++;
	}
	return (words);
}
