/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:22:27 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/27 15:36:19 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		g_heredoc_active = 0;

// sigemptyset(&data->shell.s_sigint.sa_mask);
// data->shell.s_sigint.sa_flags = 0;
// data->shell.s_sigint.sa_handler = _hndl_sigint;
// sigaction(SIGINT, &data->shell.s_sigint, NULL);
/////////////////////////////////////

void    sighndl_sigint_main(int sig)
{
	(void)sig;
    get_ctx()->exit_status = 130;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void    sighndl_sigint_heredoc(int sig)
{
    (void)sig;
    write(1, "\n", 1);
	get_ctx()->exit_status = 130;
	//cleanup_shell(get_ctx());
	cmd_clean_and_exit(get_ctx(), NULL, NULL, 130);
	//exit(130);
}


void sigint_main_handl(void)
{
    t_ctx *ctx;
	
	ctx = get_ctx();
    sigemptyset(&ctx->sa_int.sa_mask);
    ctx->sa_int.sa_handler = sighndl_sigint_main;
    ctx->sa_int.sa_flags = 0;
    sigaction(SIGINT, &ctx->sa_int, NULL);
}

void sigint_main_ignore(void)
{
    t_ctx *ctx;
	
	ctx = get_ctx();
    sigemptyset(&ctx->sa_int.sa_mask);
    ctx->sa_int.sa_handler = SIG_IGN;
    ctx->sa_int.sa_flags = 0;
    sigaction(SIGINT, &ctx->sa_int, NULL);
} 

void   init_sig(void)
{
    t_ctx *ctx;

	ctx = get_ctx();
	sigint_main_handl();
    sigemptyset(&ctx->sa_quit.sa_mask);
    ctx->sa_quit.sa_flags = 0;
    ctx->sa_quit.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &ctx->sa_quit, NULL);
}

void init_heredoc_sig(void)
{
    t_ctx *ctx;
	
	ctx = get_ctx();
    sigemptyset(&ctx->sa_int.sa_mask);
    ctx->sa_int.sa_flags = 0;
    ctx->sa_int.sa_handler = sighndl_sigint_heredoc;
    sigaction(SIGINT, &ctx->sa_int, NULL);
}

void restore_main_sig(void)
{
    init_sig();
}

void init_child_sig(void)
{
    t_ctx *ctx;
	
	ctx = get_ctx();
    sigemptyset(&ctx->sa_int.sa_mask);
    ctx->sa_int.sa_flags = 0;
    ctx->sa_int.sa_handler = SIG_DFL;
    sigaction(SIGINT, &ctx->sa_int, NULL);
	
    sigemptyset(&ctx->sa_quit.sa_mask);
    ctx->sa_quit.sa_flags = 0;
    ctx->sa_quit.sa_handler = SIG_DFL;
    sigaction(SIGQUIT, &ctx->sa_quit, NULL);
}
