/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:34:21 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/19 19:54:50 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_cd(char **args)
{
	static char	*oldpwd = NULL;

	if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
		ft_cd_home();
	else if (ft_strcmp(args[1], "-") == 0)
		ft_cd_oldpwd(&oldpwd);
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("cd");
			return (0);
		}
	}
	ft_update_pwd(&oldpwd);
	return (0);
}

int	ft_cd_home(void)
{
	char	*home;

	home = getenv("HOME");
	if (home == NULL)
	{
		fprintf(stderr, "cd: HOME not set\n");
		return (0);
	}
	if (chdir(home) != 0)
		perror("cd");
	return (0);
}

int	ft_cd_oldpwd(char **oldpwd)
{
	if (*oldpwd == NULL)
	{
		fprintf(stderr, "cd: OLDPWD not set\n");
		return (0);
	}
	if (chdir(*oldpwd) != 0)
	{
		perror("cd");
		return (0);
	}
	printf("%s\n", *oldpwd);
	return(0);
}

int	ft_update_pwd(char **oldpwd)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (*oldpwd)
		free(*oldpwd);
	if (cwd != NULL)
	{
		*oldpwd = strdup(cwd);
		printf("Current directory: %s\n", cwd);
		free(cwd);
	}
	else
	{
		perror("getcwd() error");
		*oldpwd = NULL;
	}
	return(0);
}
