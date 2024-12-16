/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 13:22:05 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/20 14:49:51 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 00 && c <= 127)
	{
		return (1);
	}
	else
		return (0);
}
/*
#include <stdio.h>
int	main(void)
{
	printf("%d", ft_isascii('ç'));
}
*/