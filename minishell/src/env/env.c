/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:41:18 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/14 15:32:34 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**get_environment(char **envp)
{
	int		i;
	int		count;
	char	**env_copy;

	i = 0;
	count = 0;
	while (envp[count])
		count++;
	env_copy = (char **)malloc((count + 1) * sizeof(char *));
	if (!env_copy)
		return (perror("malloc failed"), NULL);
	while (i < count)
	{
		env_copy[i] = strdup(envp[i]);
		if (!env_copy[i])
		{
			perror("strdup failed");
			while (--i >= 0)
				free(env_copy[i]);
			return (free(env_copy), NULL);
		}
		i++;
	}
	return (env_copy[count] = NULL, env_copy);
}

/* int export_v(char ***env_copy, const char *var, const char *value)
{
    int i;
    size_t len;
    char *new_var;

    i = 0;
    len = strlen(var);

    // Recherche si la variable existe déjà
    while ((*env_copy)[i]) {
        if (ft_strncmp_export((*env_copy)[i], var, len) == 0 && (*env_copy)[i][len] == '=') {
            // Libère l'ancienne valeur et crée une nouvelle variable
            free((*env_copy)[i]);
            new_var = ft_strjoin(var, "="); // Crée une nouvelle chaîne de base
            char *value_joined = ft_strjoin(new_var, value); // Concatène la valeur

            // Met à jour la variable dans l'environnement
            (*env_copy)[i] = value_joined;
            free(new_var); // Libère la mémoire de la chaîne intermédiaire
            return 0;
        }
        i++;
    }

    // Si la variable n'existe pas, ajoute-la
    new_var = ft_strjoin(var, "=");
    char *new_entry = ft_strjoin(new_var, value);
    free(new_var); // Libère la mémoire de la chaîne intermédiaire

    // Réalloue l'espace pour ajouter une nouvelle variable
    size_t current_size = i; // Nombre actuel d'éléments
    *env_copy = realloc(*env_copy, (current_size + 2) * sizeof(char *));
    if (!*env_copy) {
        perror("realloc failed");
        return -1; // Erreur de réallocation
    }

    (*env_copy)[current_size] = new_entry; // Ajoute la nouvelle entrée
    (*env_copy)[current_size + 1] = NULL; // Termine avec NULL
    return 0;
} */


int export_v(char **env_copy, const char *var, const char *value)
{
    int i = 0;
    size_t len = ft_strlen(var);
    char *new_var;
    char *new_entry;
    // char **env_tutu = *env_copy;

    while (env_copy && env_copy[i])
    {
        fprintf(stderr, "test 1\n");
        if (ft_strncmp_export(env_copy[i], var, len) == 0 && env_copy[i][len] == '=')
        {
            fprintf(stderr, "test 1\n");
            free(env_copy[i]);
            new_var = ft_strjoin(var, "=");
            if (!new_var) {
                perror("Allocation failed for new_var");
                return -1;
            }
            env_copy[i] = ft_strjoin(new_var, value);
            free(new_var);
            if (!env_copy[i]) {
                perror("Allocation failed for env value");
                return -1;
            }
            return 0;
        }
        i++;
    }
    new_var = ft_strjoin(var, "=");
    if (!new_var) {
        perror("Allocation failed for new_var");
        return -1;
    }
    new_entry = ft_strjoin(new_var, value);
    free(new_var);
    if (!new_entry) {
        perror("Allocation failed for new_entry");
        return -1;
    }
    size_t env_size = i;
    char **new_env = malloc((env_size + 2) * sizeof(char *));
    if (!new_env)
    {
        free(new_env);
        perror("malloc failed");
        return -1;
    }
    for (size_t i = 0; i < env_size; i++) {
        new_env[i] = env_copy[i];
    }
    new_env[env_size] = new_entry;
    new_env[env_size + 1] = NULL;
    //free(env_copy);
    env_copy = new_env;
    printf("Environment after export:\n");
    for (int j = 0; env_copy[j]; j++) {
        printf("env[%d] = %s\n", j, env_copy[j]);
    }
    fprintf(stderr, "env_copy last value, %s \n\n", env_copy[env_size + 1]);
    return 0;
}

int unset_v(char **env_copy, const char *var)
{
	int i;
	size_t len;

	i = 0;
	len = ft_strlen(var);

	while((*env_copy)[i++])
	{
		if(ft_strncmp_export(env_copy[i], var, len) == 0 && env_copy[i][len] == '=')
		{
			free(env_copy[i]);
			while(env_copy[i + 1])
			{
				env_copy[i] = env_copy[i + 1];
				i++;
			}
			env_copy[i] = NULL;
			return (0);
		}
		//i++;
	}
	return(-1);
}
