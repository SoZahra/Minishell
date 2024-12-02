/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:21:43 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 16:49:07 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ctx	*get_ctx(void)
{
	static t_ctx	ctx;

	return (&ctx);
}

int	main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
		char **envp)
{
	if (initialize_ctx(get_ctx()))
		return (1);
	if (!get_ctx())
		return (perror("Failed to initialize context"), 1);
	init_sig();
	get_ctx()->env_vars = build_env_list(envp);
	if (!get_ctx()->env_vars)
	{
		free_ctx(get_ctx());
		return (perror("Failed to build env list"), 1);
	}
	loop_with_pipes(envp, get_ctx());
	free_ctx(get_ctx());
	return (0);
}
