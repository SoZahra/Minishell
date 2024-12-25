/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skibidi_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bama <bama@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 22:58:48 by bama              #+#    #+#             */
/*   Updated: 2024/12/25 23:39:36 by bama             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define BOOL char
#define TRUE 1
#define FALSE 0

#include <stdlib.h>
#include <stdio.h>

BOOL	is_a_delim(const char c, const char *delim)
{
	unsigned int	i;

	i = 0U;
	while (delim[i])
		if (c == delim[i++])
			return (TRUE);
	return (FALSE);
}

inline static void	skip_delim(const char *str, const char *delim, unsigned int *i, BOOL *flag)
{
	while (is_a_delim(str[*i], delim))
	{
		if (flag)
			*flag = FALSE;
		++(*i);
	}
}

unsigned int	ft_count_word(const char *str, const char *delim)
{
	BOOL			flag;
	unsigned int	i;
	unsigned int	word;

	flag = FALSE;
	word = 0U;
	i = 0U;
	while (str[i])
	{
		skip_delim(str, delim, &i, &flag);
		if (!str[i])
			break ;
		if (!flag)
		{
			flag = TRUE;
			++word;
		}
		++i;
	}
	return (word);
}

char	*gyatt_strdup(const char* str, const char *delim, unsigned int *index)
{
	char			*ret;
	unsigned int	size;
	unsigned int	j;
	unsigned int	i;

	j = 0U;
	size = 0U;
	skip_delim(str, delim, &j, NULL);
	str = &str[j];
	if (!str[j])
		return (malloc(0));
	while (!is_a_delim(str[size], delim))
		++size;
	ret = (char *)malloc((size + 1U) * sizeof(char));
	i = 0U;
	while (i < size && str[i])
	{
		ret[i] = str[i];
		++i;
	}
	ret[i] = '\0';
	*index += i + j;
	return (ret);
}

char	**skibidi_split(const char *str, const char *delim)
{
	char			**ret;
	unsigned int	size;
	unsigned int	i;
	unsigned int	j;

	i = 0U;
	j = 0U;
	size = ft_count_word(str, delim);
	ret = (char **)malloc((size + 1) * sizeof(char *));
	if (!ret)
		return (NULL);
	printf("size %d\n", size);
	ret[size] = NULL;
	while (j < size)
	{
		ret[j++] = gyatt_strdup(&str[i], delim, &i);
		++i;
	}
	return (ret);
}
