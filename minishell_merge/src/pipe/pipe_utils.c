/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:09:29 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 09:56:46 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exit_error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

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

char *get_env_value(const char *name, t_env_var *env)
{
    t_env_var *current = env;

    while (current)
    {
        if (ft_strcmp(current->name, name) == 0)
            return current->value;
        current = current->next;
    }
    return NULL; // Si la variable n'est pas trouvée
}


char	*join_path_cmd(const char *path, const char *cmd)
{
	char	*full_path;
	char	*path_with_slash;

	path_with_slash = ft_strjoin(path, "/");
    if (!path_with_slash)
		return (NULL);
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

// char *search_in_path(const char *cmd, const char *path_env)
// {
//     char *path_copy, *path_token, *full_path;
//     struct stat sb;

//     // Si la commande contient un '/', c'est un chemin absolu ou relatif
//     if (ft_strchr(cmd, '/'))
//     {
//         if (stat(cmd, &sb) == 0 && (sb.st_mode & S_IXUSR))
//             return ft_strdup(cmd); // Commande trouvée
//         return NULL; // Commande non trouvée ou non exécutable
//     }

//     // Dupliquer PATH pour le tokeniser
//     path_copy = ft_strdup(path_env);
//     if (!path_copy)
//         return NULL;

//     // Parcourir les chemins dans PATH
//     path_token = strtok(path_copy, ":");
//     while (path_token)
//     {
//         full_path = join_path_cmd(path_token, cmd);
//         if (!full_path)
//         {
//             free(path_copy);
//             return NULL;
//         }

//         if (stat(full_path, &sb) == 0 && (sb.st_mode & S_IXUSR))
//         {
//             free(path_copy); // Libérer la copie de PATH
//             return full_path; // Commande trouvée
//         }

//         free(full_path);
//         path_token = strtok(NULL, ":");
//     }

//     free(path_copy); // Libérer la copie de PATH
//     return NULL; // Commande non trouvée
// }


char *search_in_path(const char *cmd, const char *path_env)
{
    char *path_copy, *path_token, *full_path;
    struct stat sb;

    // Si la commande contient un '/', c'est un chemin absolu ou relatif
    if (ft_strchr(cmd, '/'))
    {
        if (stat(cmd, &sb) == 0 && (sb.st_mode & S_IXUSR))
            return ft_strdup(cmd); // Commande trouvée
        return NULL; // Commande non trouvée ou non exécutable
    }

    // Dupliquer PATH pour le tokeniser
    path_copy = ft_strdup(path_env);
    if (!path_copy)
        return NULL;

    // Parcourir les chemins dans PATH
    path_token = strtok(path_copy, ":");
    while (path_token)
    {
        full_path = join_path_cmd(path_token, cmd);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }

        if (stat(full_path, &sb) == 0 && (sb.st_mode & S_IXUSR))
        {
            free(path_copy); // Libérer la copie de PATH
            return full_path; // Commande trouvée
        }

        free(full_path);
        path_token = strtok(NULL, ":");
    }

    free(path_copy); // Libérer la copie de PATH
    return NULL; // Commande non trouvée
}

char *get_path(char *cmd, t_env_var *env)
{
    char *path_env = get_env_value("PATH", env); // Obtenir la valeur de PATH
    char *path = NULL;

    if (!path_env)
        return NULL; // Si PATH n'existe pas
    path = search_in_path(cmd, path_env); // Rechercher la commande dans PATH
    return path;
}


// char *get_path(char *cmd, t_env_var *env)
// {
//     char **env_array = convert_env_to_array(env);
//     char *path = NULL;

//     if (!env_array)
//         return NULL;

//     // Code pour rechercher dans PATH, par exemple :
//     char *path_env = get_environment("PATH"); // Si vous utilisez des fonctions système
//     if (path_env)
//         path = search_in_path(cmd, path_env);
//     free(env_array); // Libérer le tableau temporaire
//     return path;
// }



// char *get_path(char *cmd, char **env)
// {
//     char **paths;
//     char **s_cmd;
//     char *full_path;
//     int i;

//     i = 0;
//     s_cmd = ft_split(cmd, ' ');
//     paths = ft_split(find_in_env("PATH", env), ':');
//     while (paths[i])
//     {
//         full_path = join_path_cmd(paths[i], s_cmd[0]);
//         if (access(full_path, F_OK | X_OK) == 0)
//             return (free_tab(paths), free_tab(s_cmd), full_path);
//         free(full_path);
//         i++;
//     }
//     free_tab(paths);
//     free_tab(s_cmd);
//     return (cmd);
// }

