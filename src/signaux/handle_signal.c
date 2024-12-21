/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:22:27 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/17 15:25:40 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// sigemptyset(&data->shell.s_sigint.sa_mask);
// data->shell.s_sigint.sa_flags = 0;
// data->shell.s_sigint.sa_handler = _hndl_sigint;
// sigaction(SIGINT, &data->shell.s_sigint, NULL);
///////////////////////////////////////
// void	sighndl_sigint_main(int sig)
// {
// 	g_var_global = sig;
// 	get_ctx()->exit_status = 130;
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

// void sigset_sigint_main(int sig)
// {
//     sigemptyset(&get_ctx()->s_sigint.sa_mask);
//     get_ctx()->s_sigint.sa_flags = 0;
//     get_ctx()->s_sigint.sa_handler = sighndl_sigint_main; // func_handler
//     sigaction(SIGINT, get_ctx()->s_sigint, NULL);
// }

// void	sighndl_sigquit_ign(int sig)
// {
//     return ;
// }

// void sigset_sigquit_main(int sig)
// {
//     sigemptyset(&get_ctx()->s_sigint.sa_mask);
//     get_ctx()->s_sigint.sa_flags = 0;
//     // get_ctx()->s_sigint.sa_handler = sighndl_sigquit_ign; // func_handler
//     get_ctx()->s_sigint.sa_handler = NULL; // func_handler
//     sigaction(SIG_IGN, get_ctx()->s_sigint, NULL);
// }

// void sigset_sigint_child(int sig)
// {
//     sigemptyset(&get_ctx()->s_sigint.sa_mask);
//     get_ctx()->s_sigint.sa_flags = 0;
//     get_ctx()->s_sigint.sa_handler = NULL; // func_handler
//     sigaction(SIGINT, get_ctx()->s_sigint, NULL);
// }

// void sigset_sigquit_child(int sig)
// {
//     sigemptyset(&get_ctx()->s_sigint.sa_mask);
//     get_ctx()->s_sigint.sa_flags = 0;
//     get_ctx()->s_sigint.sa_handler = NULL; // func_handler
//     sigaction(SIGQUIT, get_ctx()->s_sigint, NULL);
// }

// void sigset_sigint_heredoc(int sig)
// {
//     sigemptyset(&get_ctx()->s_sigint.sa_mask);
//     get_ctx()->s_sigint.sa_flags = 0;
//     get_ctx()->s_sigint.sa_handler = NULL; // func_handler
//     sigaction(SIGINT, get_ctx()->s_sigint, NULL);
// }

// void sigset_sigquit_heredoc(int sig)
// {
//     sigemptyset(&get_ctx()->s_sigint.sa_mask);
//     get_ctx()->s_sigint.sa_flags = 0;
//     // get_ctx()->s_sigint.sa_handler = sighndl_sigquit_ign; // func_handler
//     get_ctx()->s_sigint.sa_handler = NULL; // func_handler
//     sigaction(SIG_IGN, get_ctx()->s_sigint, NULL);
// }

//////////////////////////////////////
// int		g_var_global = 0;

// char	*empty_completion(const char *text, int state)
// {
// 	(void)text;
// 	(void)state;
// 	return (NULL);
// }

// void	handle_sigint(int sig)
// {
// 	(void)sig;
// 	printf("\n");
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// 	g_var_global = 130;
// }

// void	handle_sigquit(int sig)
// {
// 	(void)sig;
// }

// void	init_sig(void)
// {
// 	signal(SIGINT, handle_sigint);
// 	signal(SIGQUIT, SIG_IGN);
// }
