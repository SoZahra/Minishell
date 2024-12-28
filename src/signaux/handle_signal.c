/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:31:52 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:15:13 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile int		g_heredoc_active = false;

// sigemptyset(&data->shell.s_sigint.sa_mask);
// data->shell.s_sigint.sa_flags = 0;
// data->shell.s_sigint.sa_handler = _hndl_sigint;
// sigaction(SIGINT, &data->shell.s_sigint, NULL);
/////////////////////////////////////

void	init_sig(void)
{
	setsig(&get_ctx()->s_sigint, SIGINT, handle_sigint, 0);
	setsig(&get_ctx()->s_sigquit, SIGQUIT, SIG_IGN, 0);
}

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
	cmd_clean_and_exit(get_ctx(), get_ctx()->current_command, NULL, 131);
}

void	handle_sigint(int signum)
{
	(void)signum;
	get_ctx()->exit_status = 130;
	rl_on_new_line();
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	if (g_heredoc_active)
		cmd_clean_and_exit(get_ctx(), get_ctx()->current_command, NULL, 130);
	rl_redisplay();
}
