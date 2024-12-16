/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:06:37 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/27 10:31:49 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t elementCount, size_t elementSize)
{
	void	*ptr;

	ptr = (void *)malloc(elementCount * elementSize);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, elementCount * elementSize);
	return (ptr);
}
/*
int main(void)
{
    int *arr = (int *)ft_calloc(5, sizeof(int));
    if (arr) {
        for (int i = 0; i < 5; i++)
            printf("%d ", arr[i]);
        free(arr);
    }
    return (0);
}*/