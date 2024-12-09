/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:09:29 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/09 15:38:57 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exit_error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

char	*find_in_env(char *name, char **env)
{
	int	i;
	int	len;

    if (!name || !env)
		return (NULL);
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

/* char	*join_path_cmd(char *path, char *cmd)
{

    char *full_path;
    char *path_with_slash = malloc(strlen(path) + 2);

    if (!path_with_slash)
        return (NULL);
    sprintf(path_with_slash, "%s/", path);
    full_path = malloc(strlen(path_with_slash) + strlen(cmd) + 1);

    if (!full_path)
    {
        free(path_with_slash);
        return (NULL);
    }

    sprintf(full_path, "%s%s", path_with_slash, cmd);
    free(path_with_slash);
    return (full_path);
} */

char	*join_path_cmd(char *path, char *cmd)
{
	char	*full_path;
	char	*path_with_slash;

    if (!path || !cmd)
		return (NULL);
	path_with_slash = ft_strjoin(path, "/");
	full_path = ft_strjoin(path_with_slash, cmd);
	free(path_with_slash);
	return (full_path);
}

char *get_path(char *cmd, char **env)
{
    char **paths;
    char **s_cmd;
    char *full_path;
    int i;

    if (!cmd || !env)
		return (NULL);
    i = 0;
    s_cmd = ft_split(cmd, ' ');
    if (!s_cmd)
		return (NULL);
    paths = ft_split(find_in_env("PATH", env), ':');
    if (!paths)
		return(free(s_cmd), NULL);
    while (paths[i])
    {
        full_path = join_path_cmd(paths[i], s_cmd[0]);
        if (!full_path)
			return (free(s_cmd), free_tab(paths), NULL);
        if (access(full_path, F_OK | X_OK) == 0)
            return (free_tab(paths), free_tab(s_cmd), full_path);
        free(full_path);
        i++;
    }
    free_tab(paths);
    free_tab(s_cmd);
    return (cmd);
}

