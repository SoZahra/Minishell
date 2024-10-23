/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:33:55 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/23 17:18:30 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_var_global = 0; //suivre l'etat du signal

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();// prepaerer readline pour une nouvelle ligne
    rl_replace_line("", 0); //remplacer la ligne affichee avec une nouvelle
    rl_redisplay(); // redessiner le prompt
	g_var_global = 1;
	// printf("\n%s", PROMPT);
	// fflush(stdout);
}

void	handle_sigquit(int sig)
{
	(void)sig; //ne rien faire quand il recoit un ctrl /
}

void	init_sig(void)
{
	signal(SIGINT, handle_sigint);  // Gestion de Ctrl+C
	signal(SIGQUIT, SIG_IGN);  // Ignorer Ctrl+/
}

// void	setup_signals(void)
// {
// 	struct sigaction sa_quit;
// 	struct sigaction sa_int;

// 	 // Configuration de SIGQUIT
//     sa_quit.sa_handler = handle_sigquit;
//     sigemptyset(&sa_quit.sa_mask);
//     sa_quit.sa_flags = SA_RESTART; // Redémarre les appels système interrompus
//     sigaction(SIGQUIT, &sa_quit, NULL);

//     // Configuration de SIGINT
//     sa_int.sa_handler = handle_sigint;
//     sigemptyset(&sa_int.sa_mask);
//     sa_int.sa_flags = SA_RESTART; // Redémarre les appels système interrompus
//     sigaction(SIGINT, &sa_int, NULL);
// }
