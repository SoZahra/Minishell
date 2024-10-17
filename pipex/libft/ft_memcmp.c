/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:14:56 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/28 11:10:03 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *p1, const void *p2, size_t size)
{
	unsigned char	*s1;
	unsigned char	*s2;

	s1 = (unsigned char *)p1;
	s2 = (unsigned char *)p2;
	if (size == 0)
	{
		return (0);
	}
	while ((*s1 == *s2) && (size - 1 > 0))
	{
		size--;
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
/*
int main(void)
{
    const char s1[] = "Hello";
    const char s2[] = "Hello";

    printf("Comparaison de s1 et s2 : %d\n", ft_memcmp(s1, s2, 5));
    printf("Comparaison de s1 et s3 : %d\n", memcmp(s1, s2, 5));

    return 0;
}*/