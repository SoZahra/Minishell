/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:10:05 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/12 18:24:23 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int	count_args(char **args)
// {
// 	int	count;

// 	count = 0;
// 	while (args[count])
// 		count++;
// 	return (count);
// }

int count_args(char **args)
{
   int count = 0;
   if (args)
       while (args[count])
           count++;
   return count;
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

static int is_valid_var_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

int is_valid_var_name(const char *name)
{
    // Cas spéciaux à rejeter immédiatement
    if (!name || !*name || *name == '=' || ft_isdigit(*name) || *name == '-')
        return 0;

    // Le premier caractère doit être une lettre ou _
    if (!ft_isalpha(*name) && *name != '_')
        return 0;

    // Vérifier chaque caractère
    while (*name)
    {
        if (!is_valid_var_char(*name))
            return 0;
        name++;
    }
    return 1;
}

// int is_valid_var_name(const char *name)
// {
//     // Le nom doit commencer par une lettre ou _
//     if (!name || (!ft_isalpha(*name) && *name != '_'))
//         return 0;

//     // Le reste du nom peut contenir des lettres, chiffres ou _
//     name++;
//     while (*name)
//     {
//         if (!ft_isalnum(*name) && *name != '_')
//             return 0;
//         name++;
//     }

//     return 1;
// }

// int is_valid_var_name(const char *str)
// {
//     int i;

//     if (!str || !*str)
//         return (0);

//     // Premier caractère doit être une lettre ou underscore
//     if (!ft_isalpha(str[0]) && str[0] != '_')
//         return (0);

//     i = 1;
//     while (str[i])
//     {
//         // Seuls les caractères alphanumériques et underscore sont permis
//         if (!ft_isalnum(str[i]) && str[i] != '_')
//             return (0);
//         i++;
//     }
//     return (1);
// }


int split_env_v(char *arg, char **var, char **value)
{
    // printf("Debug: Splitting arg: '%s'\n", arg);
    char *equal_sign = ft_strchr(arg, '=');

    if (!equal_sign)
    {
        // printf("Debug: No equal sign found\n");
        if (!is_valid_var_name(arg))
        {
            // printf("Debug: Invalid var name (no equal)\n");
            fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n", arg);
            return 0;
        }
        *var = ft_strdup(arg);
        *value = NULL;
        // printf("Debug: Created var without value: '%s'\n", *var);
        return (*var != NULL);
    }

    *var = ft_strndup(arg, equal_sign - arg);
    // printf("Debug: Extracted var name: '%s'\n", *var);

    if (!is_valid_var_name(*var))
    {
        // printf("Debug: Invalid var name\n");
        free(*var);
        fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n", arg);
        return 0;
    }

    *value = ft_strdup(equal_sign + 1);
    // printf("Debug: Extracted value: '%s'\n", *value);

    if (!*var || !*value)
    {
        free(*var);
        free(*value);
        // printf("Debug: Memory allocation failed\n");
        return 0;
    }

    return 1;
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


