/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:52:13 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 16:41:58 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*join_path(const char *dir, const char *file)
{
	size_t	len_dir;
	size_t	len_file;
	char	*full_path;

	len_file = ft_strlen(file);
	len_dir = ft_strlen(dir);
	full_path = malloc(len_dir + len_file + 2);
	if (!full_path)
	{
		perror("malloc");
		return (NULL);
	}
	strcpy(full_path, dir);
	if (full_path[len_dir - 1] != '/')
		strcat(full_path, "/");
	strcat(full_path, file);
	return (full_path);
}

int	is_executable(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (1);
	return (0);
}

char	*find_command_path(const char *command, t_ctx *ctx)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	if (strchr(command, '/'))
	{
		if (is_executable(command))
			return (ft_strdup(command));
		return (NULL);
	}
	path_env = ps_get_env_var("PATH", ctx);
	if (!path_env)
		return (NULL);

	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);

	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], command);
		if (!full_path)
		{
			free_array(paths);
			return (NULL);
		}
		if (is_executable(full_path))
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}
