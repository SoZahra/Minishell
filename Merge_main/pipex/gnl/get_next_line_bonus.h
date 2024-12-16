/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:33:43 by llarrey           #+#    #+#             */
/*   Updated: 2024/09/18 14:24:31 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

# include "../libft/libft.h"
# include <unistd.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <limits.h>
# include <stdlib.h>

//size_t	ft_strlen(char *s);
char	*ft_new_stash(char *left_str);
//char	*ft_strjoin(char *left_str, char *buff);
char	*get_next_line(int fd);
char	*ft_read_to_stash(int fd, char *left_str);
//char	*ft_strchr(char *s, int c);
char	*ft_get_line(char *left_str);

#endif