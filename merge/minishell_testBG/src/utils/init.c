/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:21:14 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/17 15:50:45 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	initialize_ctx(t_ctx *ctx)
{
	if (!ctx)
	{
		perror("Failed to allocate memory for t_ctx");
		return (1);
	}
	ctx->env_vars = NULL;
	ctx->exit_status = 0;
	ctx->num_pipes = 0;
	ctx->oldpwd = NULL;
	ctx->pwd = getcwd(NULL, 0);
	if (!ctx->pwd)
	{
		perror("Failed to get current working directory");
		return (1);
	}
	return (0);
}
