/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:38:08 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/26 16:38:09 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_putstr_fd_(char *str, int fd, int *len)
{
	if (!str)
		str = "(null)";
	while (*str)
		*len += write(fd, str++, 1);
}

void	ft_is_digit_fd(long long nbr, int base, int fd, int *len)
{
	if (nbr < 0)
	{
		nbr *= -1;
		*len += write(fd, "-", 1);
	}
	if (nbr >= base)
		ft_is_digit_fd((nbr / base), base, fd, len);
	*len += write(fd, &"0123456789abcdef"[nbr % base], 1);
}

int	ft_fprintf(int fd, const char *format, ...)
{
	int		len;
	va_list	ptr;

	len = 0;
	va_start(ptr, format);
	while (*format)
	{
		if (*format != '%')
			len += write(fd, format, 1);
		else if ((*format == '%') && *(format + 1))
		{
			format++;
			if (*format == 's')
				ft_putstr_fd_(va_arg(ptr, char *), fd, &len);
			else if (*format == 'x')
				ft_is_digit_fd((long long)va_arg(ptr, unsigned int),
					16, fd, &len);
			else if (*format == 'd')
				ft_is_digit_fd((long long)va_arg(ptr, int), 10, fd, &len);
		}
		format++;
	}
	return (va_end(ptr), len);
}
