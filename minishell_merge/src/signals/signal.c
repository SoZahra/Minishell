/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 16:33:55 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/22 17:57:38 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		g_var_global = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	rl_clear_history();
	g_var_global = 1;
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	init_sig(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

int process_exit_arg(char **args, t_ctx *ctx)
{
    long exit_code;

    write(1, "exit\n", 5);

    if (!args[1])
        exit(ctx->exit_status);

    if (args[2])
    {
        fprintf(stderr, "exit: too many arguments\n");
        ctx->exit_status = 1;
        return 1;  // Ne pas exit, retourner au shell
    }

    // Extraire le signe si présent
    const char *str = args[1];
    int sign = 1;
    if (*str == '+')
        str++;
    else if (*str == '-')
    {
        sign = -1;
        str++;
    }

    // Retirer les quotes si présentes
    char *cleaned = strip_quotes(str);
    if (!cleaned)
    {
        fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
        exit(2);
    }

    // Vérifier que c'est un nombre valide
    for (const char *p = cleaned; *p; p++)
    {
        if (!ft_isdigit(*p))
        {
            free(cleaned);
            fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
            exit(2);
        }
    }

    exit_code = sign * ft_atoi(cleaned);
    free(cleaned);

    // Calculer le code de sortie modulo 256
    if (exit_code < 0)
        exit_code = 256 - ((-exit_code) % 256);
    else
        exit_code = exit_code % 256;

    exit(exit_code);
}

