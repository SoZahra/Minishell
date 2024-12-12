/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:21:43 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/13 00:16:26 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ctx	*get_ctx(void)
{
	static t_ctx	ctx;

	return (&ctx);
}

int get_term_attr()
{
    return (tcgetattr(STDIN_FILENO, &get_ctx()->term));
}

int set_term_attr()
{
    return (tcsetattr(STDIN_FILENO, TCSANOW, &get_ctx()->term));
}

int	main(int argc __attribute__((unused)), char **argv __attribute__((unused)), char **envp)
{
	// atexit(__lsan_do_leak_check);
	if (initialize_ctx(get_ctx()))
		return (1);
	get_term_attr();
	if (!get_ctx())
		return (perror("Failed to initialize context"), 1);
	init_sig();
	get_ctx()->env_vars = build_env_list(envp);
	if (!get_ctx()->env_vars)
	{
		free_all(get_ctx());
		return (perror("Failed to build env list"), 1);
	}
	loop_with_pipes(get_ctx());
	free_all(get_ctx());
	// (void)envp;
	// char *args[] = {"echo", "hello", NULL};
    // t_ctx ctx = {0};
    // handle_echo_builtin(args, &ctx);
	return (0);
}
