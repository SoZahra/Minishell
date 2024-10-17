/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:43:01 by llarrey           #+#    #+#             */
/*   Updated: 2024/09/23 17:45:59 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	*free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	return (NULL);
}

char	*find_in_env(char *name, char **env)
{
	int	i;
	int	len;

	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*join_path_cmd(char *path, char *cmd)
{
	char	*full_path;
	char	*path_with_slash;

	path_with_slash = ft_strjoin(path, "/");
	full_path = ft_strjoin(path_with_slash, cmd);
	free(path_with_slash);
	return (full_path);
}

char	*get_path(char *cmd, char **env)
{
	char	**paths;
	char	**s_cmd;
	char	*full_path;
	int		i;

	i = 0;
	s_cmd = ft_split(cmd, ' ');
	paths = ft_split(find_in_env("PATH", env), ':');
	while (paths[i])
	{
		full_path = join_path_cmd(paths[i], s_cmd[0]);
		if (access(full_path, F_OK | X_OK) == 0)
			return (free_tab(paths), free_tab(s_cmd), full_path);
		free(full_path);
		i++;
	}
	free_tab(paths);
	free_tab(s_cmd);
	return (cmd);
}

void	close_fd(int *pipe_fd, int fd)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(fd);
}
