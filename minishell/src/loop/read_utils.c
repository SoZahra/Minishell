/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:05:43 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/24 15:12:40 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_cmd(t_token *cmd, int index)
{
	printf(" Command %d: ", index);
	while (cmd)
	{
		printf("%s ", cmd->value);
		cmd = cmd->next;
	}
	printf("\n");
}

void	print_cmds(t_token *cmd_tokens, int num_pipes)
{
	int	k;

	k = 0;
	printf("Number of pipes: %d\n", num_pipes);
	while (k <= num_pipes)
	{
		print_cmd(&cmd_tokens[k], k);
		k++;
	}
}

void	create_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("Echec de pipe");
		exit(EXIT_FAILURE);
	}
}

char	**prepare_print_args(t_token *cmd)
{
	char	**args;
	int		k;

	k = 0;
	args = prepare_args(cmd);
	if (args == NULL || args[0] == NULL)
	{
		fprintf(stderr, "Erreur lors de la preparation des args\n");
		exit(EXIT_FAILURE);
	}
	printf("Execution de la commande : %s\n", args[0]);
	while (args[k] != NULL)
	{
		printf("Arguments %d : %s\n", k, args[k]);
		k++;
	}
	return (args);
}
