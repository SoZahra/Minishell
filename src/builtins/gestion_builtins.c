/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:49:14 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/26 15:56:49 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(const char *cmd)
{
	int		i;
	char	*builtins[8];
	size_t	len;

	if (!cmd)
		return (1);
	while (*cmd == ' ')
		cmd++;
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	i = 0;
	while (builtins[i])
	{
		len = ft_strlen(builtins[i]);
		if (strncmp(cmd, builtins[i], len) == 0
			&& (cmd[len] == ' ' || cmd[len] == '\0'))
			return (i + 1);
		i++;
	}
	return (0);
}
