/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:40:43 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/27 13:36:39 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*dest;
	size_t	len_src;

	len_src = ft_strlen(src);
	dest = (char *)malloc((len_src + 1) * sizeof(char));
	if (!dest)
	{
		return (NULL);
	}
	ft_memcpy(dest, src, len_src);
	dest[len_src] = '\0';
	return (dest);
}
/*
int	main(void)
{
	//char *test;
	//test = ft_strdup("hm");
	printf("%s", ft_strdup("lorem ipsum dolor sit amet"));
	return(0);
}*/