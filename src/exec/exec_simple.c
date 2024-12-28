/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:02:02 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:03:04 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_pos(char *arg, t_ctx *ctx);
int	validate_args_while(char **args, t_ctx *ctx);

int	validate_args(char **args, t_ctx *ctx)
{
	if (!args || !args[0])
	{
		perror("Erreur d'allocation de mémoire pour les arguments");
		free_tab(args);
		ctx->exit_status = 1;
		return (0);
	}
	if (ft_strcmp(args[0], "export") == 0)
		validate_args_while(args, ctx);
	return (1);
}

int	validate_args_while(char **args, t_ctx *ctx)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (ft_strcmp(args[i], "=") == 0)
		{
			fprintf(stderr, "MiniBG: export: `=': not a valid identifier\n");
			ctx->exit_status = 1;
			return (0);
		}
		if (ft_isdigit(args[i][0]))
		{
			fprintf(stderr, "MiniBG: export: `%s':\
 not a valid identifier\n", args[i]);
			ctx->exit_status = 1;
			return (0);
		}
		if (!check_pos(args[i], ctx))
			return (0);
		i++;
	}
	return (1);
}

int	check_pos(char *arg, t_ctx *ctx)
{
	char	*equal_pos;
	char	*dash_pos;

	equal_pos = ft_strchr(arg, '=');
	dash_pos = ft_strchr(arg, '-');
	if (dash_pos && (!equal_pos || dash_pos < equal_pos))
	{
		fprintf(stderr, "MiniBG: export: \
`%s': not a valid identifier\n", arg);
		ctx->exit_status = 1;
		return (0);
	}
	return (1);
}
