/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:38:54 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/26 16:41:28 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*strip_quotes(char *arg)
{
	if ((*arg == '"' || *arg == '\'') && arg[strlen(arg) - 1] == *arg)
	{
		return (ft_strndup(arg + 1, strlen(arg) - 2));
	}
	return (ft_strdup(arg));
}

int	is_numeric_argument(const char *arg)
{
	if (!arg || (*arg != '+' && *arg != '-' && !ft_isdigit(*arg)))
		return (0);
	if (*arg == '+' || *arg == '-')
		arg++;
	while (*arg)
	{
		if (!ft_isdigit(*arg))
			return (0);
		arg++;
	}
	return (1);
}

size_t	ft_strspn(const char *str, const char *accept)
{
	size_t	count;
	size_t	i;
	size_t	j;
	int		found;

	count = ((i = 0));
	while (str[i])
	{
		found = ((j = 0));
		while (accept[j])
		{
			if (str[i] == accept[j])
			{
				found = 1;
				break ;
			}
			j++;
		}
		if (!found)
			return (count);
		count++;
		i++;
	}
	return (count);
}
