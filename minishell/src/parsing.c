/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:34:21 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/22 14:55:43 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_cd(char **args)
{
    char cwd[PATH_MAX];
    char *home = getenv("HOME");
    static char *oldpwd = NULL;  // Pour stocker le répertoire précédent

    if (args[1] == NULL || strcmp(args[1], "~") == 0) // Si 'cd' est appelé sans argument ou avec '~'
    {
        if (home == NULL)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
        if (chdir(home) != 0) // Aller au répertoire HOME
            perror("cd");
    }
    else if (strcmp(args[1], "-") == 0) // Si 'cd -' est appelé
    {
        if (oldpwd == NULL)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return;
        }
        if (chdir(oldpwd) != 0) // Retourner au répertoire précédent
        {
            perror("cd");
            return;
        }
        printf("%s\n", oldpwd); // Afficher le répertoire précédent
    }
    else
    {
        if (chdir(args[1]) != 0) // Essayer d'aller au répertoire spécifié
        {
            perror("cd");
            return;
        }
    }
    if (oldpwd)// Mettre à jour OLDPWD et CWD après chaque changement de répertoire
        free(oldpwd);
    oldpwd = strdup(getcwd(cwd, sizeof(cwd))); // Stocker l'ancien répertoire
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("Current directory: %s\n", cwd);
    else
        perror("getcwd() error");
}
