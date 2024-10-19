/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:11:48 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/28 14:31:54 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	i;
	unsigned int	j;
	char			*cs1;

	j = ft_strlen(s1);
	i = 0;
	if (j == 0)
	{
		cs1 = (char *)malloc(sizeof(char));
		if (cs1)
			cs1[0] = '\0';
		return (cs1);
	}
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	while (s1[j - 1] && ft_strchr(set, s1[j - 1]) && j > i)
		j--;
	cs1 = (char *)malloc(sizeof(char) * (j - i + 1));
	if (cs1)
		ft_strlcpy(cs1, &s1[i], j - i + 1);
	return (cs1);
}
/*
int	main(void)
{
	printf("test : %s\n", ft_strtrim(" lorem ipsum dolor sit amet", "l "));
	return (0);
}*/
