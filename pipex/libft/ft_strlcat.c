/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:47:40 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/22 16:10:37 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	x;
	size_t	lend;
	size_t	lens;

	x = 0;
	lens = ft_strlen(src);
	while (dest[x] != '\0' && x < size)
	{
		x++;
	}
	lend = x;
	if (size == x)
	{
		return (lens + x);
	}
	if (lens < size - lend)
		ft_memcpy(dest + lend, src, lens + 1);
	else
	{
		ft_memcpy(dest + lend, src, size - lend - 1);
		dest[size - 1] = '\0';
	}
	return (lend + lens);
}
/*
int	main(void)
{
	char *src;
	src = "Hello, World!";
	char dst[50] = "not Hello";
	size_t size = 23;
	size_t result;

	// Utilisation de ft_strlcat
	result = ft_strlcat(dst, src, size);

	// Affichage des résultats
	printf("Source: %s\n", src);
	printf("Destination: %s\n", dst);
	printf("Taille de la source: %zu\n", result);

	return (0);
}*/