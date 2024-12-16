/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:49:14 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/16 16:38:13 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int	is_builtin(char *cmd)
// {
// 	char	*builtins[8];
// 	int		i;

// 	builtins[0] = "echo";
// 	builtins[1] = "cd";
// 	builtins[2] = "pwd";
// 	builtins[3] = "export";
// 	builtins[4] = "unset";
// 	builtins[5] = "env";
// 	builtins[6] = "exit";
// 	builtins[7] = NULL;
// 	i = 0;
// 	while (builtins[i])
// 	{
// 		if (strcmp(cmd, builtins[i]) == 0)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

int is_builtin(const char *cmd)
{
    size_t len;
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
        len = ft_strlen(builtins[i]);
        if (strncmp(cmd, builtins[i], len) == 0 &&
            (cmd[len] == ' ' || cmd[len] == '\0'))
            return i + 1;
        i++;
    }
    return 0;
}

// int is_builtin(const char *cmd)
// {
//     // Skip leading spaces
//     while (*cmd == ' ')
//         cmd++;

//     const char *builtins[] = {
//         "echo", "cd", "pwd", "export",
//         "unset", "env", "exit", NULL
//     };

//     for (int i = 0; builtins[i]; i++)
//     {
//         size_t len = ft_strlen(builtins[i]);
//         if (strncmp(cmd, builtins[i], len) == 0 &&
//             (cmd[len] == ' ' || cmd[len] == '\0'))
//             return i + 1;  // Return builtin index + 1
//     }
//     return 0;
// }
