/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:41:18 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/18 17:37:07 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **get_environment(char **envp)
{
    int i = 0;
    int count = 0;

    // Compter le nombre de variables d'environnement
    while (envp[count])
        count++;

    // Allouer de la mémoire pour un tableau de chaînes de caractères
    char **env_copy = (char **)malloc((count + 1) * sizeof(char *));
    if (!env_copy)
    {
        perror("malloc failed");
        return NULL;
    }
    while (i < count) // Copier chaque variable d'environnement
    {
        env_copy[i] = strdup(envp[i]); // Dupliquer chaque chaîne
        if (!env_copy[i])
        {
            perror("strdup failed");
            while (--i >= 0)  // Libérer la mémoire en cas d'échec
                free(env_copy[i]);
            free(env_copy);
            return NULL;
        }
        i++;
    }
    env_copy[i] = NULL; // Terminer le tableau par un pointeur NULL
    return env_copy;
}