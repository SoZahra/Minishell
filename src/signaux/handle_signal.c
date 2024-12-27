/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:31:52 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 14:18:52 by ymanchon         ###   ########.fr       */
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

void	handle_sigquit(int signum)
{
	(void)signum;
	write(STDERR_FILENO, "Quit (core dumped)\n", 20);
	get_ctx()->exit_status = 131;
	cmd_clean_and_exit(get_ctx(), NULL, NULL, 131);
	//exit(131);
}

void	handle_sigint(int signum)
{
	(void)signum;
	get_ctx()->exit_status = 130;
	rl_on_new_line();
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	if (g_heredoc_active)
		cmd_clean_and_exit(get_ctx(), NULL, NULL, 130);
		//exit(130);
	rl_redisplay();
}
