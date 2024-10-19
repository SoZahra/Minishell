/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:52:59 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/27 09:48:47 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *s1, const void *s2, size_t len)
{
	unsigned char	*dst;
	unsigned char	*src;
	unsigned int	i;

	dst = (unsigned char *)s1;
	src = (unsigned char *)s2;
	i = 0;
	if (dst == NULL && src == NULL)
		return (NULL);
	if (dst < src)
	{
		while (i < len)
		{
			dst[i] = src[i];
			i++;
		}
	}
	else
	{
		while (0 < len--)
			dst[len] = src[len];
	}
	return (dst);
}
/*
int main(void)
{
   char dest[] = "oldstring";
   const char src[]  = "newstring";

   printf("Before memmove dest = %s, src = %s\n", dest, src);
   ft_memmove(dest, src, 2);
   printf("After ft_memmove dest = %s, src = %s\n", dest, src);
   printf("Before memmove dest = %s, src = %s\n", dest, src);
   memmove(dest, src, 2);
   printf("After memmove dest = %s, src = %s\n", dest, src);

   return(0);
}*/