/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:17:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 15:18:03 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env_var *build_env_list(char **envp)
{
    t_env_var *head = NULL;
    t_env_var *current = NULL;

    for (int i = 0; envp[i]; i++)
    {
        char *sep = strchr(envp[i], '=');
        if (!sep)
            continue;

        t_env_var *new_var = malloc(sizeof(t_env_var));
        if (!new_var)
        {
            perror("malloc failed");
            free_env(head);
            return NULL;
        }
        // Copier le nom et la valeur
        new_var->name = ft_strndup(envp[i], sep - envp[i]);
        new_var->value = ft_strdup(sep + 1);
        new_var->next = NULL;

        if (!new_var->name || !new_var->value)
        {
            free(new_var->name);
            free(new_var->value);
            free(new_var);
            free_env(head);
            return (perror("malloc failed"), NULL);
        }
        if (!head)
            head = new_var;
        else
            current->next = new_var;

        current = new_var;
    }
    return head;
}