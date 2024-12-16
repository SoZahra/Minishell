/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 10:46:53 by llarrey           #+#    #+#             */
/*   Updated: 2024/05/28 11:10:20 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *memoryBlock, int searchedChar, size_t size )
{
	char	*string;

	string = (char *)memoryBlock;
	while (size > 0)
	{
		if (*string == (char)searchedChar)
		{
			return ((char *)string);
		}
		string++;
		size--;
	}
	return (NULL);
}
/*
int main() {
    // Exemple de bloc mémoire à tester
    char memoryBlock[] = "Hello, world!";
    int searchedChar = 'o';
    size_t size = sizeof(memoryBlock);
    char *result = (char *)ft_memchr(memoryBlock, searchedChar, size);

    if (result != NULL) {
        printf("Caractère '%c' trouvé à l'adresse : %p\n", searchedChar, result);
        printf("La chaîne restante à partir de ce point est : \"%s\"\n", result);
    } else {
        printf("Caractère '%c' non trouvé .\n", searchedChar);
    }

    return 0;
}
*/