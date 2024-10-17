/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:43:33 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/27 13:57:52 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*destalt;

	destalt = dest;
	i = 0;
	if (!dest && !src)
		return (dest);
	while (i < n)
	{
		*(char *)destalt = *(char *)src;
		destalt++;
		src++;
		i++;
	}
	return (dest);
}
/*
int main(void)
{
   const char src[50] = "https://www.tutorialspoint.com";
   char dest[50];
   strcpy(dest,"Heloooo!!");
   printf("Before memcpy dest = %s\n", dest);
   ft_memcpy(dest, src, strlen(src)+1);
   printf("After memcpy dest = %s\n", dest);
   
   return(0);
}*/