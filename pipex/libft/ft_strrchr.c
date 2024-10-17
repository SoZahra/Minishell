/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 10:15:36 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/23 13:47:56 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *string, int c)
{
	int	i;

	i = ft_strlen(string);
	while (i >= 0)
	{
		if (string[i] == (char)c)
			return ((char *)string + i);
		i--;
	}
	return (NULL);
}

/*int	main(void)
{
	char buffer1[40] = "computer program";
	char *ptr;
	int ch = 'p';

	ptr = ft_strrchr(buffer1, '\0');
	printf("The last occurrence of %c in '%s' is '%s'\n", ch, buffer1, ptr);
}*/