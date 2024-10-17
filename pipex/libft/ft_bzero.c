/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:16:32 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/20 14:49:41 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_bzero(void *str, size_t n)
{
	char	*dest;
	int		i;

	dest = str;
	i = 0;
	while (n)
	{
		dest[i] = '\0';
		n--;
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>
int	main(void)
{
	char str[] = "test";
	char str2[] = "test";
	bzero(str2, 2);
	ft_bzero(str, 2);
	printf("%s\n", (char *)str2);
	printf("%s\n", (char *)str);
	return (0);
}*/