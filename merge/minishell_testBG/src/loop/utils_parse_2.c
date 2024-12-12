/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:minishell/src/lexer/lexer_utils.c
/*   Created: 2024/10/10 17:41:00 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/22 14:59:53 by llarrey          ###   ########.fr       */
=======
/*   Created: 2024/12/02 15:13:19 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/03 10:08:43 by fzayani          ###   ########.fr       */
>>>>>>> Fat:merge/minishell_testBG/src/loop/utils_parse_2.c
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

char	*find_env_value(const char *name, t_env_var *env_vars)
{
	while (env_vars)
	{
		if (strcmp(env_vars->name, name) == 0)
			return (env_vars->value);
		env_vars = env_vars->next;
	}
	return (NULL);
}
