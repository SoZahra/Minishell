/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:32:37 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/24 16:38:21 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
		char **envp)
{
	char	**env_copy;

	init_sig();
	env_copy = get_environment(envp);
	if (!env_copy)
		return (fprintf(stderr, "Failed to copy environment\n"), 1);
	loop(env_copy);
	free_tab(env_copy);
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
