/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:37:12 by llarrey           #+#    #+#             */
/*   Updated: 2024/09/29 11:54:31 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_close_pipe(int mypipe[1024][2], int i)
{
	while (--i >= 0)
	{
		close(mypipe[i][0]);
		close(mypipe[i][1]);
	}
}

int	ft_opener(char **av, int ac, int pipe_fd[2])
{
	int	fd;

	fd = open(av[ac - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open output file failed");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exit(EXIT_FAILURE);
	}
	return (fd);
}
