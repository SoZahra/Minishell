/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:22:27 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/26 20:15:37 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		g_heredoc_active = 0;

// sigemptyset(&data->shell.s_sigint.sa_mask);
// data->shell.s_sigint.sa_flags = 0;
// data->shell.s_sigint.sa_handler = _hndl_sigint;
// sigaction(SIGINT, &data->shell.s_sigint, NULL);
/////////////////////////////////////

void    setsig(struct sigaction *sa, int signum, void (*f)(int), int flags)
{
    sa->sa_flags = flags;
    sa->sa_handler = f;
    sigemptyset(&sa->sa_mask);
    sigaction(signum, sa, NULL);
}

void    handle_sigint(int signum)
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


void	sighndl_sigint_main(int sig)
{
	g_heredoc_active = sig;
	get_ctx()->exit_status = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

//void sigset_sigint_main(int sig)
//{
//    (void)sig;
//    sigemptyset(&get_ctx()->s_sigint.sa_mask);
//    get_ctx()->s_sigint.sa_flags = 0;
//    get_ctx()->s_sigint.sa_handler = sighndl_sigint_main;
//    sigaction(SIGINT, &get_ctx()->s_sigint, NULL);
//}

//void	sighndl_sigquit_ign(int sig)
//{
//    (void)sig;
//    return ;
//}

//void sigset_sigquit_main(int sig)
//{
//    (void)sig;
//    sigemptyset(&get_ctx()->s_sigint.sa_mask);
//    get_ctx()->s_sigint.sa_flags = 0;
//    get_ctx()->s_sigint.sa_handler = SIG_IGN;
//    sigaction(SIGQUIT, &get_ctx()->s_sigint, NULL);
//}

//void sigset_sigint_child(int sig)
//{
//    (void)sig;
//    sigemptyset(&get_ctx()->s_sigint.sa_mask);
//    get_ctx()->s_sigint.sa_flags = 0;
//    get_ctx()->s_sigint.sa_handler = SIG_DFL;
//    sigaction(SIGINT, &get_ctx()->s_sigint, NULL);
//}



//void sigset_sigquit_child(int sig)
//{
//    (void)sig;
//    sigemptyset(&get_ctx()->s_sigint.sa_mask);
//    get_ctx()->s_sigint.sa_flags = 0;
//    get_ctx()->s_sigint.sa_handler = SIG_DFL;
//    sigaction(SIGQUIT, &get_ctx()->s_sigint, NULL);
//}

void sighndl_sigint_heredoc(int sig)
{
    (void)sig;
    g_heredoc_active = 1;
    write(1, "\n", 1);  // Juste un saut de ligne
}

void init_heredoc_sig(void)
{
    t_ctx *ctx = get_ctx();

    sigemptyset(&ctx->s_sigint.sa_mask);
    sigemptyset(&ctx->s_sigquit.sa_mask);
    ctx->s_sigint.sa_flags = 0;
    ctx->s_sigquit.sa_flags = 0;
    ctx->s_sigint.sa_handler = sighndl_sigint_heredoc;
    ctx->s_sigquit.sa_handler = SIG_IGN;
    sigaction(SIGINT, &ctx->s_sigint, NULL);
    sigaction(SIGQUIT, &ctx->s_sigquit, NULL);
}
void restore_main_sig(void)
{
    t_ctx *ctx = get_ctx();

    sigemptyset(&ctx->s_sigint.sa_mask);
    sigemptyset(&ctx->s_sigquit.sa_mask);
    ctx->s_sigint.sa_flags = 0;
    ctx->s_sigquit.sa_flags = 0;

    // Restaurer le mode normal
    ctx->s_sigint.sa_handler = sighndl_sigint_main;
    ctx->s_sigquit.sa_handler = SIG_IGN;
    sigaction(SIGINT, &ctx->s_sigint, NULL);
    sigaction(SIGQUIT, &ctx->s_sigquit, NULL);
}


//// void sigset_sigint_heredoc(int sig)
//// {
////     (void)sig;
////     sigemptyset(&get_ctx()->s_sigint.sa_mask);
////     get_ctx()->s_sigint.sa_flags = 0;
////     get_ctx()->s_sigint.sa_handler = SIG_DFL;
////     sigaction(SIGINT, &get_ctx()->s_sigint, NULL);
//// }

//void sigset_sigint_heredoc(int sig)
//{
//    (void)sig;
//    sigemptyset(&get_ctx()->s_sigint.sa_mask);
//    get_ctx()->s_sigint.sa_flags = 0;
//    get_ctx()->s_sigint.sa_handler = sighndl_sigint_heredoc;
//    sigaction(SIGINT, &get_ctx()->s_sigint, NULL);
//}

//void sigset_sigquit_heredoc(int sig)
//{
//    (void)sig;
//    sigemptyset(&get_ctx()->s_sigint.sa_mask);
//    get_ctx()->s_sigint.sa_flags = 0;
//    get_ctx()->s_sigint.sa_handler = SIG_IGN;
//    sigaction(SIGQUIT, &get_ctx()->s_sigint, NULL);
//}
// void sigset_sigquit_main(int sig)
// {
//     (void)sig;
//     sigemptyset(&get_ctx()->s_sigint.sa_mask);
//     get_ctx()->s_sigint.sa_flags = 0;
//     get_ctx()->s_sigint.sa_handler = sighndl_sigquit_ign; // func_handler
//     // get_ctx()->s_sigint.sa_handler = NULL; // func_handler
//     sigaction(SIG_IGN, &get_ctx()->s_sigint, NULL);
// }

// void sigset_sigint_child(int sig)
// {
//     (void)sig;
//     sigemptyset(&get_ctx()->s_sigint.sa_mask);
//     get_ctx()->s_sigint.sa_flags = 0;
//     get_ctx()->s_sigint.sa_handler = NULL; // func_handler
//     sigaction(SIGINT, &get_ctx()->s_sigint, NULL);
// }

// void sigset_sigquit_child(int sig)
// {
//     (void)sig;
//     sigemptyset(&get_ctx()->s_sigint.sa_mask);
//     get_ctx()->s_sigint.sa_flags = 0;
//     get_ctx()->s_sigint.sa_handler = NULL; // func_handler
//     sigaction(SIGQUIT, &get_ctx()->s_sigint, NULL);
// }

// void sigset_sigint_heredoc(int sig)
// {
//     (void)sig;
//     sigemptyset(&get_ctx()->s_sigint.sa_mask);
//     get_ctx()->s_sigint.sa_flags = 0;
//     get_ctx()->s_sigint.sa_handler = NULL; // func_handler
//     sigaction(SIGINT, &get_ctx()->s_sigint, NULL);
// }

// void sigset_sigquit_heredoc(int sig)
// {
//     (void)sig;
//     sigemptyset(&get_ctx()->s_sigint.sa_mask);
//     get_ctx()->s_sigint.sa_flags = 0;
//     get_ctx()->s_sigint.sa_handler = sighndl_sigquit_ign; // func_handler
//     // get_ctx()->s_sigint.sa_handler = NULL; // func_handler
//     sigaction(SIG_IGN, &get_ctx()->s_sigint, NULL);
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
