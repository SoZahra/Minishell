/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:56:58 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/03 13:58:34 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*strip_quotes(char *arg)
{
	if ((*arg == '"' || *arg == '\'') && arg[strlen(arg) - 1] == *arg)
	{
		return (ft_strndup(arg + 1, strlen(arg) - 2));
	}
	return (ft_strdup(arg));
}

int	is_numeric_argument(const char *arg)
{
	if (!arg || (*arg != '+' && *arg != '-' && !ft_isdigit(*arg)))
		return (0);
	if (*arg == '+' || *arg == '-')
		arg++;
	while (*arg)
	{
		if (!ft_isdigit(*arg))
			return (0);
		arg++;
	}
	return (1);
}

//fonction a enlever
char **convert_env_to_array(t_ctx *ctx)
{
    int count = 0;
    t_env_var *current = ctx->env_vars;
    char **env_array;

    while (current)
    {
        count++;
        current = current->next;
    }

    env_array = malloc((count + 1) * sizeof(char *));
    if (!env_array)
        return (NULL);

    current = ctx->env_vars;
    for (int i = 0; i < count && current; i++)
    {
        int len = ft_strlen(current->name)
	+ (current->value ? ft_strlen(current->value) : 0) + 2;
        env_array[i] = malloc(len);
        if (!env_array[i])
        {
            free_tab(env_array);
            return (NULL);
        }
        if (current->value)
            snprintf(env_array[i], len, "%s=%s", current->name,
	current->value);
        else
            snprintf(env_array[i], len, "%s=", current->name);
        current = current->next;
    }
    env_array[count] = NULL;
    return (env_array);
}
