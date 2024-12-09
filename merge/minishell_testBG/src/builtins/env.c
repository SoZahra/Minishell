/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:08:19 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/09 11:35:44 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_cd(char **args, t_ctx *ctx)
{
	if (args[1] && args[2])
	{
		fprintf(stderr, "cd: too many arguments\n");
		ctx->exit_status = 1;
		return (1);
	}
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		return (ft_cd_home(ctx));
	else if (ft_strcmp(args[1], "-") == 0)
		return (ft_cd_oldpwd(ctx));
	if (chdir(args[1]) != 0)
	{
		fprintf(stderr, "cd: %s: No such file or directory\n", args[1]);
        ctx->exit_status = 1;
        return (1);
	}
	return (ft_update_pwd(ctx));
}

int	ft_cd_home(t_ctx *ctx)
{
	char	*home;

	home = find_env_value("HOME", ctx->env_vars);
	if (!home)
	{
		fprintf(stderr, "cd: HOME not set\n");
		return (1);
	}
	if (chdir(home) != 0)
	{
		perror("cd");
		return (1);
	}
	return (ft_update_pwd(ctx));
}

int	ft_cd_oldpwd(t_ctx *ctx)
{
	if (ctx->oldpwd == NULL)
	{
		fprintf(stderr, "cd: OLDPWD not set\n");
		return (1);
	}
	if (chdir(ctx->oldpwd) != 0)
	{
		perror("cd");
		return (1);
	}
	printf("%s\n", ctx->oldpwd);
	return (ft_update_pwd(ctx));
}

int	ft_update_pwd(t_ctx *ctx)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (1);
	}
	if (ctx->oldpwd)
		free(ctx->oldpwd);
	ctx->oldpwd = ctx->pwd;
	ctx->pwd = cwd;
	return (0);
}
