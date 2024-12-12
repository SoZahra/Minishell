/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:minishell/src/signals/signal.c
/*   Created: 2024/10/03 16:33:55 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/22 14:02:26 by llarrey          ###   ########.fr       */
=======
/*   Created: 2024/12/02 15:22:27 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 19:24:45 by fzayani          ###   ########.fr       */
>>>>>>> Fat:merge/minishell_testBG/src/signaux/handle_signal.c
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		g_var_global = 0;

char	*empty_completion(const char *text, int state)
{
	(void)text;
	(void)state;
	return (NULL);
}

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
    char *cleaned_arg;
    long exit_code;

    if (args[1])
    {
        cleaned_arg = strip_quotes(args[1]); // Supprimer les quotes
        if (!is_numeric_argument(cleaned_arg))
        {
            fprintf(stderr, "minishell: exit: %s: numeric argument required\n", cleaned_arg);
            free(cleaned_arg);
            ctx->exit_status = 2;
            return 1; // Indique une sortie immédiate avec code 2
        }
        exit_code = ft_atoi(cleaned_arg);
        free(cleaned_arg);
        if (args[2])
        {
            fprintf(stderr, "minishell: exit: too many arguments\n");
            ctx->exit_status = 1;
            return 0; // Ne pas quitter immédiatement
        }
        ctx->exit_status = (exit_code % 256 + 256) % 256; // Calculer modulo 256
    }
    else
        ctx->exit_status = 0;
    write(1, "exit\n", 5);
    exit(ctx->exit_status);
    return 1; // Pour respecter les conventions de retour (même si unreachable ici)
}
