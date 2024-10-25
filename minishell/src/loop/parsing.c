/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:34:21 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/25 13:21:21 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	ft_cd_home(void)
// {
// 	char	*home;

// 	home = getenv("HOME");
// 	if (home == NULL)
// 	{
// 		fprintf(stderr, "cd: HOME not set\n");
// 		return ;
// 	}
// 	if (chdir(home) != 0)
// 		perror("cd");
// }

// void	ft_cd_oldpwd(char **oldpwd)
// {
// 	if (*oldpwd == NULL)
// 	{
// 		fprintf(stderr, "cd: OLDPWD not set\n");
// 		return ;
// 	}
// 	if (chdir(*oldpwd) != 0)
// 	{
// 		perror("cd");
// 		return ;
// 	}
// 	printf("%s\n", *oldpwd);
// }

// void	ft_uptdate_pwd(char **oldpwd, char *cwd)
// {

// 	if (*oldpwd)
// 		free(*oldpwd);
// 	if (getcwd(cwd, sizeof(cwd)) != NULL)
// 	{
// 		*oldpwd = strdup(cwd);
// 		printf("Current directory: %s\n", cwd);
// 	}
// 	else
// 	{
// 		perror("getcwd() error");
// 		*oldpwd = NULL;
// 	}
// }

// void	ft_cd(char **args)
// {
// 	static char	*oldpwd = NULL;
// 	char		cwd[PATH_MAX];

// 	if (args[1] == NULL || strcmp(args[1], "~") == 0)
// 		ft_cd_home();
// 	else if (strcmp(args[1], "-") == 0)
// 		ft_cd_oldpwd(&oldpwd);
// 	else
// 	{
// 		if (chdir(args[1]) != 0)
// 		{
// 			perror("cd");
// 			return;
// 		}
// 	}
// 	ft_uptdate_pwd(&oldpwd, cwd);
// }

void	ft_cd(char **args)
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
			return;
		}
	}
	ft_update_pwd(&oldpwd);
}

void	ft_cd_home(void)
{
	char *home = getenv("HOME");
	if (home == NULL)
	{
		fprintf(stderr, "cd: HOME not set\n");
		return;
	}
	if (chdir(home) != 0)
		perror("cd");
}

void	ft_cd_oldpwd(char **oldpwd)
{
	if (*oldpwd == NULL)
	{
		fprintf(stderr, "cd: OLDPWD not set\n");
		return;
	}
	if (chdir(*oldpwd) != 0)
	{
		perror("cd");
		return;
	}
	printf("%s\n", *oldpwd);
}

void	ft_update_pwd(char **oldpwd)
{
	char *cwd = getcwd(NULL, 0);

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
}

// void	ft_cd(char **args)
// {
// 	char		cwd[PATH_MAX];
// 	char		*home;
// 	static char	*oldpwd;

// 	home = getenv("HOME");
// 	oldpwd = NULL;
// 	if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
// 	{
// 		if (home == NULL)
// 		{
// 			fprintf(stderr, "cd: HOME not set\n");
// 			return ;
// 		}
// 		if (chdir(home) != 0)
// 			perror("cd");
// 	}
// 	else if (ft_strcmp(args[1], "-") == 0)
// 	{
// 		if (oldpwd == NULL)
// 		{
// 			fprintf(stderr, "cd: OLDPWD not set\n");
// 			return ;
// 		}
// 		if (chdir(oldpwd) != 0)
// 		{
// 			perror("cd");
// 			return ;
// 		}
// 		printf("%s\n", oldpwd);
// 	}
// 	else
// 	{
// 		if (chdir(args[1]) != 0)
// 		{
// 			perror("cd");
// 			return ;
// 		}
// 	}
// 	if (oldpwd)
// 		free(oldpwd);
// 	oldpwd = ft_strdup(getcwd(cwd, sizeof(cwd)));
// 	if (getcwd(cwd, sizeof(cwd)) != NULL)
// 		printf("Current directory: %s\n", cwd);
// 	else
// 		perror("getcwd() error");
// }
