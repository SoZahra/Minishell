/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:31:52 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/26 19:22:48 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile int		g_heredoc_active = false;

// sigemptyset(&data->shell.s_sigint.sa_mask);
// data->shell.s_sigint.sa_flags = 0;
// data->shell.s_sigint.sa_handler = _hndl_sigint;
// sigaction(SIGINT, &data->shell.s_sigint, NULL);
/////////////////////////////////////

void	setsig(struct sigaction *sa, int signum, void (*f)(int), int flags)
{
	sa->sa_flags = flags;
	sa->sa_handler = f;
	sigemptyset(&sa->sa_mask);
	sigaction(signum, sa, NULL);
}

void	handle_sigint(int signum)
{
	if (g_heredoc_active)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		exit(0);
	}
	(void)signum;
	get_ctx()->exit_status = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
