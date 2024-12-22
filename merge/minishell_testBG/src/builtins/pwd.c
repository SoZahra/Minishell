/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:50:39 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/22 13:51:08 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int handle_pwd_builtin(const char *input, t_ctx *ctx)
{
    char *cwd;

    while (*input == ' ')
        input++;
    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("pwd");
        ctx->exit_status = 1;
        return 1;
    }
    printf("%s\n", cwd);
    free(cwd);
    ctx->exit_status = 0;
    return 0;
}
