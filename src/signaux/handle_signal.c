/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:31:52 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/26 16:32:02 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		g_var_global = 0;

// sigemptyset(&data->shell.s_sigint.sa_mask);
// data->shell.s_sigint.sa_flags = 0;
// data->shell.s_sigint.sa_handler = _hndl_sigint;
// sigaction(SIGINT, &data->shell.s_sigint, NULL);
/////////////////////////////////////


void	sighndl_sigint_main(int sig)
{
	g_var_global = sig;
	get_ctx()->exit_status = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigset_sigint_main(int sig)
{
	(void)sig;
	sigemptyset(&get_ctx()->s_sigint.sa_mask);
	get_ctx()->s_sigint.sa_flags = 0;
	get_ctx()->s_sigint.sa_handler = sighndl_sigint_main;
	sigaction(SIGINT, &get_ctx()->s_sigint, NULL);
}

void	sighndl_sigquit_ign(int sig)
{
	(void)sig;
	return ;
}

void	sigset_sigquit_main(int sig)
{
	(void)sig;
	sigemptyset(&get_ctx()->s_sigint.sa_mask);
	get_ctx()->s_sigint.sa_flags = 0;
	get_ctx()->s_sigint.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &get_ctx()->s_sigint, NULL);
}

void	sigset_sigint_child(int sig)
{
	(void)sig;
	sigemptyset(&get_ctx()->s_sigint.sa_mask);
	get_ctx()->s_sigint.sa_flags = 0;
	get_ctx()->s_sigint.sa_handler = SIG_DFL;
	sigaction(SIGINT, &get_ctx()->s_sigint, NULL);
}


void	sigset_sigquit_child(int sig)
{
	(void)sig;
	sigemptyset(&get_ctx()->s_sigint.sa_mask);
	get_ctx()->s_sigint.sa_flags = 0;
	get_ctx()->s_sigint.sa_handler = SIG_DFL;
	sigaction(SIGQUIT, &get_ctx()->s_sigint, NULL);
}

void	sigset_sigint_heredoc(int sig)
{
	(void)sig;
	sigemptyset(&get_ctx()->s_sigint.sa_mask);
	get_ctx()->s_sigint.sa_flags = 0;
	get_ctx()->s_sigint.sa_handler = SIG_DFL;
	sigaction(SIGINT, &get_ctx()->s_sigint, NULL);
}

void	sigset_sigquit_heredoc(int sig)
{
	(void)sig;
	sigemptyset(&get_ctx()->s_sigint.sa_mask);
	get_ctx()->s_sigint.sa_flags = 0;
	get_ctx()->s_sigint.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &get_ctx()->s_sigint, NULL);
}
