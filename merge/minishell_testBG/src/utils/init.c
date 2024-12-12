/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:minishell/src/utils/free.c
/*   Created: 2024/10/07 16:07:57 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/22 15:06:41 by llarrey          ###   ########.fr       */
=======
/*   Created: 2024/12/02 15:21:14 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/03 13:47:22 by fzayani          ###   ########.fr       */
>>>>>>> Fat:merge/minishell_testBG/src/utils/init.c
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
		free(ctx);
		return (1);
	}
	return (0);
}
