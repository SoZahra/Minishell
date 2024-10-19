/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 17:02:27 by llarrey           #+#    #+#             */
/*   Updated: 2024/09/17 14:15:32 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *first, const char *second)
{
	int	i;

	i = 0;
	while ((first[i] || second[i] != '\0') && first[i] == second[i])
	{
		i++;
	}
	return (first[i] - second[i]);
}
