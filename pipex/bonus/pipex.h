/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:31:57 by llarrey           #+#    #+#             */
/*   Updated: 2024/09/28 11:24:08 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include "../gnl/get_next_line_bonus.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdlib.h>

//void	pipex(int f1, int f2, char *cmd1, char *cmd2);
void	exec(char *cmd, char **env);
/* void	child(char **av, int *p_fd, char **env);
void	parent(char **av, int *p_fd, char **env); */
size_t	ft_strlen(const char *c);
void	*free_tab(char **tab);
char	*my_getenv(char *name, char **env);
char	*get_path(char *cmd, char **env);
char	*join_path_cmd(char *path, char *cmd);
char	*find_in_env(char *name, char **env);
void	middle_command(char *cmd, int pipe_fd[1024][2], int i, char **env);
void	last_command(char **av, int pipe_fd[1024][2], int i, char **env);
void	first_command(char **av, int *pipe_fd, char **env);
void	ft_close_pipe(int mypipe[1024][2], int i);
void	close_fd(int *pipe_fd, int fd);
int		ft_opener(char **av, int ac, int *pipe_fd);

#endif