/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:10:05 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/08 17:25:04 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

t_env_var	*get_last_env_node(t_env_var **env)
{
	t_env_var	*tmp;

	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

// int	is_valid_id(const char *var)
// {
// 	char	*equal_sign;
// 	size_t	len;
// 	size_t	i;

// 	if (!var || !*var)
// 		return (0);
// 	equal_sign = ft_strchr(var, '=');
// 	if (equal_sign)
// 		len = (size_t)(equal_sign - var);
// 	else
// 		len = ft_strlen(var);
// 	if (!ft_isalpha(var[0]) && var[0] != '_')
// 		return (0);
// 	i = 1;
// 	while (i < len)
// 	{
// 		if (!ft_isalnum(var[i]) && var[i] != '_')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

int is_valid_id(const char *var)
{
    size_t i;
    
    if (!var || !*var)
        return (0);
        
    // Vérifie si le premier caractère est une lettre ou underscore
    if (!ft_isalpha(var[0]) && var[0] != '_')
        return (0);
        
    // Vérifie le reste du nom
    i = 1;
    while (var[i] && var[i] != '=')
    {
        // Seuls les caractères alphanumériques et underscore sont valides
        if (!ft_isalnum(var[i]) && var[i] != '_')
            return (0);
        i++;
    }
    
    // Vérifie qu'il y a au moins un caractère avant le '='
    if (i == 0 || (var[0] == '=' && i == 1))
        return (0);
        
    return (1);
}

int is_valid_var_name(const char *str)
{
    int i;

    if (!str || !*str)
        return (0);
    
    // Premier caractère doit être une lettre ou underscore
    if (!ft_isalpha(str[0]) && str[0] != '_')
        return (0);
    
    i = 1;
    while (str[i])
    {
        // Seuls les caractères alphanumériques et underscore sont permis
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

int split_env_v(char *arg, char **var, char **value)
{
    char *equal_sign;

    // Validation initiale du nom
    equal_sign = ft_strchr(arg, '=');
    if (!equal_sign)
    {
        // Cas sans '=' - vérifier si le nom est valide
        if (!is_valid_var_name(arg))
        {
            fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n", arg);
            return (0);
        }
        *var = ft_strdup(arg);
        *value = NULL;
        return (*var != NULL);
    }
    // Cas avec '=' - vérifier la partie avant le '='
    *var = ft_strndup(arg, equal_sign - arg);
    if (!is_valid_var_name(*var))
    {
        free(*var);
        fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n", arg);
        return (0);
    }
    *value = ft_strdup(equal_sign + 1);
    if (!*var || (equal_sign && !*value))
    {
        free(*var);
        free(*value);
        return (0);
    }
    return (1);
}

// int	split_env_v(char *arg, char **var, char **value)
// {
// 	char	*equal_sign;

// 	equal_sign = ft_strchr(arg, '=');
// 	if (!equal_sign)
// 	{
// 		*var = ft_strdup(arg);
// 		*value = NULL;
// 		return (*var != NULL);
// 	}
// 	*var = ft_strndup(arg, equal_sign - arg);
// 	*value = ft_strdup(equal_sign + 1);
// 	if (!*var || (equal_sign && !*value))
// 	{
// 		free(*var);
// 		free(*value);
// 		return (0);
// 	}
// 	return (1);
// }
