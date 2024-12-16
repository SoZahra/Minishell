/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:26:57 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/27 13:38:33 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*concat;
	size_t	len_s1;
	size_t	len_s2;
	int		i;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	i = 0;
	concat = (char *)malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (!concat)
		return (NULL);
	ft_memcpy(concat, s1, len_s1);
	while (s2[i] != '\0')
	{
		concat[len_s1 + i] = s2[i];
		i++;
	}
	concat[len_s1 + i] = '\0';
	return (concat);
}
/*
int main(void)
{
	printf("%s", ft_strjoin("Hello", "World"));
	return(0);
}*/
