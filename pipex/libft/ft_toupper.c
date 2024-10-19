/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 09:49:14 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/22 09:57:13 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c < 97 || c > 122)
	{
		return (c);
	}
	else
	{
		c -= 32;
	}
	return (c);
}
/*
int	main(void)
{
	printf("%d", ft_toupper('b'));
	return (0);
}*/