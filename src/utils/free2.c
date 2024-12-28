/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:57:38 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 16:42:07 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredocs(t_command *cmd)
{
	int	i;

	if (!cmd || !cmd->redirs)
		return ;
	i = 0;
	while (cmd->redirs[i].type)
	{
		if (cmd->redirs[i].type == 'H' && cmd->redirs[i].heredoc_fd > 0)
		{
			close(cmd->redirs[i].heredoc_fd);
			cmd->redirs[i].heredoc_fd = -1;
		}
		i++;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return ;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		if (current->content && current->content != current->value)
		{
			free(current->content);
			current->content = NULL;
		}
		free(current);
		current = next;
	}
}

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
}
