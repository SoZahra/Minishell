/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:34:21 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/23 14:54:50 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_cd(char **args)
{
    char cwd[PATH_MAX];
    char *home;
    static char *oldpwd;

    home = getenv("HOME");
    oldpwd = NULL;
    if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
    {
        if (home == NULL)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
        if (chdir(home) != 0)
            perror("cd");
    }
    else if (ft_strcmp(args[1], "-") == 0)
    {
        if (oldpwd == NULL)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return;
        }
        if (chdir(oldpwd) != 0)
        {
            perror("cd");
            return;
        }
        printf("%s\n", oldpwd);
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("cd");
            return;
        }
    }
    if (oldpwd)
        free(oldpwd);
    oldpwd = ft_strdup(getcwd(cwd, sizeof(cwd)));
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("Current directory: %s\n", cwd);
    else
        perror("getcwd() error");
}
