/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:38:04 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:14:15 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	initialize_ctx(t_ctx *ctx)
{
	if (!ctx)
		return (1);
	ctx->env_vars = NULL;
	ctx->exit_status = 0;
	ctx->num_pipes = 0;
	ctx->oldpwd = NULL;
	ctx->pwd = getcwd(NULL, 0);
	if (!ctx->pwd)
	{
		perror("Failed to allocate pwd");
		free(ctx->pwd);
		ctx->pwd = NULL;
		return (1);
	}
	return (0);
}
