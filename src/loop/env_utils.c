/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 16:26:52 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 16:27:13 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ps_get_env_var(char *var_name, t_ctx *ctx)
{
	t_env_var	*tmp;

	if (!var_name || !*var_name)
		return (ft_strdup(""));
	tmp = ctx->env_vars;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, var_name))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*handle_env_var(char *result, const char *str, int *i, t_ctx *ctx)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		j;

	j = (*i) + 1;
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;
	var_name = ft_substr(str, (*i) + 1, j - (*i + 1));
	var_value = ps_get_env_var(var_name, ctx);
	free(var_name);
	if (var_value)
	{
		temp = ft_strjoin(result, var_value);
		free(result);
		result = temp;
	}
	*i = j;
	return (result);
}
