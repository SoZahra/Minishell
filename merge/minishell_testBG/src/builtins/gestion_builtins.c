/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*      clea                                          +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:49:14 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 18:30:31 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int is_builtin(const char *cmd)
{
    int i;
    if(!cmd)
        return (1);
    while (*cmd == ' ')
        cmd++;
    char	*builtins[8];
    builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;

    i = 0;
    while(builtins[i])
    {
        size_t len = strlen(builtins[i]);
        if (strncmp(cmd, builtins[i], len) == 0 &&
            (cmd[len] == ' ' || cmd[len] == '\0'))
            return i + 1;
        i++;
    }
    return 0;
}
