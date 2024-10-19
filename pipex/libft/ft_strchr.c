/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 10:00:15 by llarrey           #+#    #+#             */
/*   Updated: 2024/09/18 14:24:03 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *string, int c)
{
	int		i;

	i = 0;
	while (string[i])
	{
		if (string[i] == (char)c)
			return ((char *)(string + i));
		i++;
	}
	if (string[i] == (char)c)
		return ((char *)(string + i));
	return (0);
}
/*d
int main(void)
{
  char buffer1[40] = "computer program";
  char * ptr;
  int    ch = 'p';
 
  ptr = ft_strchr( buffer1, ch );
  printf( "The first occurrence of %c in '%s' is '%s'\n",
            ch, buffer1, ptr );
 
}*/