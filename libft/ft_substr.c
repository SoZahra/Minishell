/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 11:19:54 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/25 18:48:19 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*ss;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	ss = malloc(sizeof(char) * len + 1);
	if (!ss)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ss[i] = s[start];
		i++;
		start++;
	}
	ss[i] = '\0';
	return (ss);
}

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	unsigned int	i;
// 	char			*str;

// 	i = 0;
// 	if (start >= ft_strlen(s) || len == 0)
// 		return (ft_strdup(""));
// 	if (len > ft_strlen(s + start))
// 		len = ft_strlen(s + start);
// 	str = (char *)malloc(sizeof(char) * len + 1);
// 	if (!str)
// 		return (NULL);
// 	while (i < len)
// 	{
// 		str[i] = s[start + i];
// 		i++;
// 	}
// 	str[i] = '\0';
// 	return (str);
// }
/*
int	main(void)
{
	char *str1 = "salut je suis fatima";
	char *dest = ft_substr(str1, 6, 7);

	printf("%s\n", dest);
}*/