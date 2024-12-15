/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:08:19 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/15 17:57:57 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int ft_cd_home(t_ctx *ctx)
{
    char *home;

    home = find_env_value("HOME", ctx->env_vars);
    if (!home)
    {
        ft_fprintf(2, "cd: HOME not set\n");
        ctx->exit_status = 1;
        return 1;
    }
    if (chdir(home) != 0)
    {
        perror("cd");
        ctx->exit_status = 1;
        return 1;
    }
    return ft_update_pwd(ctx);
}

int ft_cd_oldpwd(t_ctx *ctx)
{
    if (!ctx->oldpwd)
    {
        ft_fprintf(2, "cd: OLDPWD not set\n");
        ctx->exit_status = 1;
        return 1;
    }
    if (chdir(ctx->oldpwd) != 0)
    {
        perror("cd");
        ctx->exit_status = 1;
        return 1;
    }
    printf("%s\n", ctx->oldpwd);
    return ft_update_pwd(ctx);
}

int ft_update_pwd(t_ctx *ctx)
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("getcwd");
        ctx->exit_status = 1;
        return 1;
    }
    free(ctx->oldpwd);
    ctx->oldpwd = ctx->pwd;
    ctx->pwd = cwd;
    ctx->exit_status = 0;
    return 0;
}

void free_array(char **array)
{
    int i;

    i = 0;
    if (!array)
        return;
    while( array[i])
    {
        free(array[i]);
        i++;
    }   
    free(array);
}	
