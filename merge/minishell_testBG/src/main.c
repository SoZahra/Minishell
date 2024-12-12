/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:minishell/src/main.c
/*   Created: 2024/10/02 14:32:37 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/18 15:28:25 by llarrey          ###   ########.fr       */
=======
/*   Created: 2024/12/02 15:21:43 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/12 14:26:53 by fzayani          ###   ########.fr       */
>>>>>>> Fat:merge/minishell_testBG/src/main.c
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ctx	*get_ctx(void)
{
<<<<<<< HEAD:minishell/src/main.c
	t_var myEnv;
=======
	static t_ctx	ctx;
>>>>>>> Fat:merge/minishell_testBG/src/main.c

	return (&ctx);
}

int get_term_attr()
{
    return (tcgetattr(STDIN_FILENO, &get_ctx()->term));
}

int set_term_attr()
{
    return (tcsetattr(STDIN_FILENO, TCSANOW, &get_ctx()->term));
}

int	main(int argc __attribute__((unused)), char **argv __attribute__((unused)), char **envp)
{
	if (initialize_ctx(get_ctx()))
		return (1);
	get_term_attr();
	if (!get_ctx())
		return (perror("Failed to initialize context"), 1);
	init_sig();
<<<<<<< HEAD:minishell/src/main.c
	myEnv.env = get_environment(envp);
	if (!myEnv.env)
		return (fprintf(stderr, "Failed to copy environment\n"), 1);
	loop(&myEnv);
	free_environment(myEnv.env);
=======
	get_ctx()->env_vars = build_env_list(envp);
	if (!get_ctx()->env_vars)
	{
		free_ctx(get_ctx());
		return (perror("Failed to build env list"), 1);
	}
	loop_with_pipes(get_ctx());
	free_ctx(get_ctx());
	// (void)envp;
	// char *args[] = {"echo", "hello", NULL};
    // t_ctx ctx = {0};
    // handle_echo_builtin(args, &ctx);
>>>>>>> Fat:merge/minishell_testBG/src/main.c
	return (0);
}
