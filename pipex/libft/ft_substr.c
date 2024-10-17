/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:09:40 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/28 14:18:36 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (start >= ft_strlen(s) || len == 0)
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (i >= start && j < len)
		{
			str[j] = s[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
	return (str);
}
/*
int main(void)
{
    const char *s = "Hello, World!";
    char *sub = ft_substr(s, 7, 5); // Obtient "World"
    
    if (sub)
    {
        printf("Substring: %s\n", sub);
        free(sub); // N'oubliez pas de libérer la mémoire allouée
    }
    else
    {
        printf("Erreur lors de la création du sous-chaine.\n");
    }

    return (0);
}*/