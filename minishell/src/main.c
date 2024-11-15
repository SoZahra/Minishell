/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:32:37 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/15 17:56:58 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
		char **envp)
{
	t_var myEnv;

	init_sig();
	myEnv.env = get_environment(envp);
	if (!myEnv.env)
		return (fprintf(stderr, "Failed to copy environment\n"), 1);
	loop(&myEnv);
	free_tab(myEnv.env);
	return (0);
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: '%s'\n", tokens->value);
		tokens = tokens->next;
	}
}
