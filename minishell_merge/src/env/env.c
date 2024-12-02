/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:41:18 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 13:44:56 by fzayani          ###   ########.fr       */
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

void free_environment(char **env_copy)
{
    int i = 0;

    if (!env_copy)
        return;

    while (env_copy[i])
    {
        free(env_copy[i]);
        i++;
    }
    free(env_copy);
}

void free_env_vars(t_env_var *head)
{
    t_env_var *current = head;
    t_env_var *tmp;

    while (current)
    {
        tmp = current;
        current = current->next;

        free(tmp->name);
        free(tmp->value);
        free(tmp);
    }
}


t_env_var *build_env_vars(char **envp)
{
    t_env_var *head = NULL;
    t_env_var *current = NULL;

    for (int i = 0; envp[i]; i++)
    {
        // Allouer un nouvel élément
        t_env_var *new_var = malloc(sizeof(t_env_var));
        if (!new_var)
        {
            perror("Failed to allocate memory for environment variable");
            free_env_vars(head); // Libérer les variables déjà créées
            return NULL;
        }

        // Trouver le séparateur '='
        char *sep = strchr(envp[i], '=');
        if (sep)
        {
            new_var->name = strndup(envp[i], sep - envp[i]); // Partie avant '='
            new_var->value = strdup(sep + 1);               // Partie après '='
        }
        else
        {
            new_var->name = strdup(envp[i]);
            new_var->value = NULL;
        }

        if (!new_var->name || (sep && !new_var->value))
        {
            perror("Failed to allocate memory for environment variable fields");
            free(new_var->name);
            free(new_var->value);
            free(new_var);
            free_env_vars(head); // Libérer les variables déjà créées
            return NULL;
        }

        new_var->next = NULL;

        // Ajouter à la liste chaînée
        if (!head)
            head = new_var;
        else
            current->next = new_var;

        current = new_var;
    }

    return head;
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


// char **export_v(char **env_copy, const char *var, const char *value)
// {
//     size_t i = 0;
//     size_t len = ft_strlen(var);
//     char *new_var;
//     char *new_entry;
//     int check_var = 0;

//     while (env_copy && env_copy[i])
//     {
//         if (ft_strncmp_export(env_copy[i], var, len) == 0 && env_copy[i][len] == '=')
//         {
//             fprintf(stderr, "why am i not in ? %s\n\ns", var);
//             free(env_copy[i]);
//             new_var = ft_strjoin(var, "=");
//             if (!new_var) {
//                 perror("Allocation failed for new_var");
//                 return NULL;
//             }
//             env_copy[i] = ft_strjoin(new_var, value);
//             free(new_var);
//             if (!env_copy[i]) {
//                 perror("Allocation failed for env value");
//                 return NULL;
//             }
//             check_var = 1;
//         }
//         i++;
//     }
//     if (check_var == 0)
//     {
//         new_var = ft_strjoin(var, "=");
//         if (!new_var) {
//             perror("Allocation failed for new_var");
//             return NULL;
//         }
//         new_entry = ft_strjoin(new_var, value);
//         free(new_var);
//         if (!new_entry) {
//             perror("Allocation failed for new_entry");
//             return NULL;
//         }
//         size_t env_size = i;
//         char **new_env = malloc((env_size + 2) * sizeof(char *));
//         if (!new_env)
//         {
//             free(new_env);
//             perror("malloc failed");
//             return NULL;
//         }
//         i = -1;
//         while (++i < env_size) {
//             new_env[i] = env_copy[i];
//         }
//         new_env[i++] = new_entry;
//         new_env[i] = NULL;
//         free(env_copy);
//         env_copy = new_env;
//     }
//     return env_copy;
// }

int unset_v(t_env_var **env_vars, const char *var)
{
    t_env_var *current;
    t_env_var *prev = NULL;

    if (!env_vars || !*env_vars || !var) // Vérification des pointeurs
        return -1;

    current = *env_vars;

    while (current)
    {
        // Vérifie si le nom de la variable correspond
        if (ft_strcmp(current->name, var) == 0)
        {
            if (prev)
                prev->next = current->next; // Relier le précédent au suivant
            else
                *env_vars = current->next;  // Supprimer le premier élément

            // Libérer les ressources associées
            if (current->name)
                free(current->name);
            if (current->value)
                free(current->value);
            free(current);
            return 0; // Succès
        }

        prev = current;
        current = current->next;
    }

    return 0; // Variable non trouvée
}



// int unset_v(char **env_copy, const char *var)
// {
// 	int i;
// 	size_t len;

// 	i = 0;
// 	len = ft_strlen(var);

// 	while((*env_copy)[i++])
// 	{
// 		if(ft_strncmp_export(env_copy[i], var, len) == 0 && env_copy[i][len] == '=')
// 		{
// 			free(env_copy[i]);
// 			while(env_copy[i + 1])
// 			{
// 				env_copy[i] = env_copy[i + 1];
// 				i++;
// 			}
// 			env_copy[i] = NULL;
// 			return (0);
// 		}
// 		//i++;
// 	}
// 	return(-1);
// }
