/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:21:43 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/19 16:08:16 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ctx	*get_ctx(void)
{
	static t_ctx	ctx;

	return (&ctx);
}

int	get_term_attr(void)
{
	return (tcgetattr(STDIN_FILENO, &get_ctx()->term));
}

int	set_term_attr(void)
{
	return (tcsetattr(STDIN_FILENO, TCSANOW, &get_ctx()->term));
}

void cleanup_shell(t_ctx *ctx)
{
	if (!ctx)
        return;
    free_ctx(ctx);
}

int	main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
		char **envp)
{
	if (initialize_ctx(get_ctx()))
		return (1);
	get_term_attr();
	init_sig();
	get_ctx()->env_vars = build_env_list(envp);
	if (!get_ctx()->env_vars)
	{
		cleanup_shell(get_ctx());
		return (perror("Failed to build env list"), 1);
	}
	get_ctx()->exit_status = process(get_ctx());
	cleanup_shell(get_ctx());
	return (get_ctx()->exit_status);
}
