/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:40:47 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/27 09:20:53 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	char	*dest;
	int		i;

	dest = str;
	i = 0;
	while (n)
	{
		dest[i] = c;
		n--;
		i++;
	}
	return (dest);
}
/*
#include <stdio.h>
#include <string.h>
int	main(void)
{
	char str[] = "test";
	char str2[] = "test";
	char *dest = ft_memset(str, 'd', 2);
	printf("%s\n", (char *)memset(str2, 'd', 2));
	printf("%s\n", dest);
	return (0);
}
*/