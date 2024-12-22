/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 13:36:08 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/22 13:52:46 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int handle_cd_builtin(const char *input, t_ctx *ctx)
{
    char **arg_array;
    int arg_count;
    
    while (*input == ' ')
        input++;
    if (!*input || strcmp(input, "~") == 0)
        return ft_cd_home(ctx);
    if (strcmp(input, "-") == 0)
        return ft_cd_oldpwd(ctx);
    arg_array = ft_split(input, ' ');
    if (!arg_array)
        return 1;
    arg_count = 0;
    while (arg_array[arg_count])
        arg_count++;
    if (arg_count > 1)
    {
        ft_fprintf(2, "cd: too many arguments\n");
        free_array(arg_array);
        ctx->exit_status = 0;
        return 1;
    }
    if (chdir(arg_array[0]) != 0)
    {
        ft_fprintf(2, "cd: %s: No such file or directory\n", arg_array[0]);
        free_array(arg_array);
        ctx->exit_status = 1;
        return 1;
    }
    free_array(arg_array);
    return ft_update_pwd(ctx);
}

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
