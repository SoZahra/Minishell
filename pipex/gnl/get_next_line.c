/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:33:22 by llarrey           #+#    #+#             */
/*   Updated: 2024/07/02 14:55:20 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_read_to_stash(int fd, char *stash)
{
	char	*buff;
	int		bytes;

	buff = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buff)
	{
		free(buff);
		return (NULL);
	}
	bytes = 1;
	while (!ft_strchr(stash, '\n') && bytes != 0)
	{
		bytes = read(fd, buff, BUFFER_SIZE);
		if (bytes == -1)
		{
			free(buff);
			free(stash);
			return (NULL);
		}
		buff[bytes] = '\0';
		stash = ft_strjoin(stash, buff);
	}
	free(buff);
	return (stash);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = ft_read_to_stash(fd, stash);
	if (!stash)
	{
		return (NULL);
	}
	line = ft_get_line(stash);
	stash = ft_new_stash(stash);
	if (!line && !stash)
	{
		free(line);
		free(stash);
		return (NULL);
	}
	return (line);
}

/* int	main(void)
{
	char	*line;
	int		i;
	int		fd1;
	// int		fd2;
	// int		fd3;

	fd1 = open("./test.txt", O_RDONLY);
	//fd2 = open("tests/test2.txt", O_RDONLY);
	//fd3 = open("tests/test3.txt", O_RDONLY);
	i = 1;
	while (i < 100)
	{
		line = get_next_line(fd1);
		printf("line [%02d]: %s", i, line);
		if (line){
			free(line);
		}
		if (i == 5){
			close(fd1);
			exit(0);
		}
		if (i != 5 && !line) {
			close(fd1);
			exit (0);
		}
		//line = get_next_line(fd2);
		//printf("line [%02d]: %s", i, line);
		//free(line);
		//line = get_next_line(fd3);
		//printf("line [%02d]: %s", i, line);
		//free(line);
		i++;
	}
	close(fd1);
	//close(fd2);
	//close(fd3);
	return (0);
} */