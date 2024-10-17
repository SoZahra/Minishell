/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:58:08 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/28 10:33:19 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	i;

	i = 0;
	if (size > 0)
	{
		while (i < size - 1 && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}
/*
int main(void)
{
    char src[] = "Hello, World!";
    char dst[20];
    size_t size = 10;
    size_t result;

    // Utilisation de ft_strlcpy
    result = ft_strlcpy(dst, src, size);

    // Affichage des résultats
    printf("Source: %s\n", src);
    printf("Destination: %s\n", dst);
    printf("Taille de la source: %zu\n", result);

    return 0;
}*/