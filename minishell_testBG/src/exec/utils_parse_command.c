/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:10:05 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 15:11:49 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int count_args(char **args)
{
    int count = 0;
    while (args[count])
        count++;
    return count;
}

t_env_var *get_last_env_node(t_env_var **env)
{
    t_env_var *tmp;
    tmp = *env;
    while (tmp->next)
        tmp = tmp->next;
    return tmp;
}

int is_valid_id(const char *var)
{
    char *equal_sign = ft_strchr(var, '=');
    size_t len = equal_sign ? (size_t)(equal_sign - var) : ft_strlen(var);

    if (!var || !*var)
        return 0;    // Vérifier seulement la partie avant le '=' si présent
    if (!ft_isalpha(var[0]) && var[0] != '_')
        return 0;
    for (size_t i = 1; i < len; i++)
    {
        if (!ft_isalnum(var[i]) && var[i] != '_')
            return 0;
    }
    return 1;
}

int split_env_v(char *arg, char **var, char **value)
{
    char *equal_sign = ft_strchr(arg, '=');

    if (!equal_sign)
    {
        *var = strdup(arg);
        *value = NULL;
        return *var != NULL;
    }

    // Extraire le nom de la variable avant le '='
    *var = strndup(arg, equal_sign - arg);
    *value = strdup(equal_sign + 1); // On garde les quotes dans la valeur

    if (!*var || (equal_sign && !*value))
    {
        free(*var);
        free(*value);
        return 0;
    }

    return 1;
}