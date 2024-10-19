/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:26:32 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/27 16:16:54 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	unsigned int	i;

	if (!s)
		return (NULL);
	str = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	if (s && f)
	{
		while (s[i] != '\0')
		{
			str[i] = f(i, s[i]);
			i++;
		}
	}
	str[i] = '\0';
	return (str);
}
/*
char test_function(unsigned int index, char c)
{
    return c + 1;
	index = 1;
}

int main()
{
    char *input_string = "Test"; 
    char *result = ft_strmapi(input_string, &test_function); 

    printf("Chaîne initiale : %s\n", input_string);
    printf("Chaîne transformée : %s\n", result);

    free(result);

    return 0;
}*/
