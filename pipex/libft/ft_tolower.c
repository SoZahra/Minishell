/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 09:55:22 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/22 09:58:28 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c < 65 || c > 90)
	{
		return (c);
	}
	else
	{
		c += 32;
	}
	return (c);
}
/*
int	main(void)
{
	printf("%d", ft_tolower('3'));
	return (0);
}*/