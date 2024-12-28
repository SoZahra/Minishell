/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:26:10 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:30:04 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*check_direct_command(const char *command)
{
	if (!strchr(command, '/'))
		return (NULL);
	if (is_executable(command))
		return (ft_strdup(command));
	return (NULL);
}

static char	**get_path_dirs(t_ctx *ctx)
{
	char	*path_env;

	path_env = ps_get_env_var("PATH", ctx);
	if (!path_env)
		return (NULL);
	return (ft_split(path_env, ':'));
}

static char	*search_in_paths(char **paths, const char *command)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], command);
		if (!full_path)
			return (free_array(paths), NULL);
		if (is_executable(full_path))
			return (free_array(paths), full_path);
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

char	*find_command_path(const char *command, t_ctx *ctx)
{
	char	**paths;
	char	*direct_path;

	direct_path = check_direct_command(command);
	if (direct_path)
		return (direct_path);
	paths = get_path_dirs(ctx);
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, command));
}
