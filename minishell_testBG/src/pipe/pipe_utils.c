/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:09:29 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/03 17:15:52 by llarrey          ###   ########.fr       */
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

	path_with_slash = ft_strjoin(path, "/");
	full_path = ft_strjoin(path_with_slash, cmd);
	free(path_with_slash);
	return (full_path);
}

/* char *get_path(char *cmd, char **env)
{
    char **paths;
    char *full_path;
    int i;
    // printf("%s\n", cmd);
    // Extraire uniquement la commande sans les arguments
    char **split_cmd = ft_split(cmd, ' ');  // Séparer les mots de la commande
    char *command_only = split_cmd[0];      // Prendre seulement la commande

    paths = ft_split(find_in_env("PATH", env), ':');
    i = 0;
    while (paths[i])
    {
        full_path = join_path_cmd(paths[i], command_only);  // Chercher uniquement la commande
        fprintf(stderr, "Checking path: %s\n", full_path);  // Debug pour vérifier les chemins testés
        if (access(full_path, F_OK | X_OK) == 0)
        {
            free_tab(paths);
            free_tab(split_cmd);
            return full_path;  // Retourner le chemin complet si la commande est trouvée
        }
        
        free(full_path);
        i++;
    }
    free_tab(paths);
    free_tab(split_cmd);
    return command_only;  // Si aucun chemin n'est trouvé, retourner la commande brute
} */

char *get_path(char *cmd, char **env)
{
    char **paths;
    char **s_cmd;
    char *full_path;
    int i;

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

