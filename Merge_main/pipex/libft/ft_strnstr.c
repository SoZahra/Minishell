/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:36:04 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/22 11:53:06 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *bag, const char *cat, size_t len)
{
	size_t	i;
	size_t	c;
	size_t	c_len;
	char	*kitten;

	i = 0;
	kitten = (char *)bag;
	c_len = ft_strlen(cat);
	if (c_len == 0 || bag == cat)
		return (kitten);
	while (kitten[i] != '\0' && i < len)
	{
		c = 0;
		while (kitten[i + c] != '\0' && cat[c] != '\0'
			&& kitten[i + c] == cat[c] && i + c < len)
			c++;
		if (c == c_len)
			return (kitten + i);
		i++;
	}
	return (0);
}
/*
int main(void) {
    const char *bag = "Hello, world!";
    const char *cat = "world";
    size_t len = 15;
    char *result = ft_strnstr(bag, cat, len);

    if (result)
        printf("Found: %s\n", result);
    else
        printf("Not found\n");

    return 0;
}*/