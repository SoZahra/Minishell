/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:47:46 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 15:16:36 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_command_struct(int arg_count, int redir_count)
{
	t_command	*cmd;

	cmd = calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->pid = -1;
	cmd->pfd[0] = -1;
	cmd->pfd[1] = -1;
	cmd->arg_count = arg_count;
	cmd->args = calloc(arg_count + 1, sizeof(char *));
	if (!cmd->args)
		return (free_command_list(cmd), NULL);
	cmd->redirs = calloc(redir_count + 1, sizeof(t_redirection));
	if (!cmd->redirs)
		return (free_command_list(cmd), NULL);
	cmd->had_spaces = calloc(arg_count, sizeof(int));
	if (!cmd->had_spaces)
		return (free_command_list(cmd), NULL);
	return (cmd);
}

t_command	*link_commands(t_command *first_cmd, t_command *new_cmd)
{
	t_command	*last_cmd;

	last_cmd = first_cmd;
	if (!first_cmd)
		return (new_cmd);
	while (last_cmd->next)
		last_cmd = last_cmd->next;
	last_cmd->next = new_cmd;
	new_cmd->prev = last_cmd;
	return (first_cmd);
}

int	process_token(t_command *cmd, t_token **curr, int *arg_i, int *red_i)
{
	if (is_redirection((*curr)->type))
	{
		if (!(*curr)->next)
			return (1);
		if (add_redir(cmd, *curr, red_i))
			return (1);
		*curr = (*curr)->next;
	}
	else if (is_argument((*curr)->type))
	{
		if (add_argument(cmd, *curr, arg_i))
			return (1);
	}
	return (0);
}

int	fill_command(t_command *cmd, t_token *curr, t_token *end)
{
	int	arg_i;
	int	red_i;

	arg_i = 0;
	red_i = 0;
	while (curr && curr != end && curr->type != '|')
	{
		if (process_token(cmd, &curr, &arg_i, &red_i))
			return (1);
		if (curr)
			curr = curr->next;
	}
	cmd->args[arg_i] = NULL;
	cmd->redirs[red_i].type = 0;
	cmd->arg_count = arg_i;
	return (0);
}

t_command	*create_command_from_tokens_range(t_token *start, t_token *end)
{
	int			arg_count;
	int			redir_count;
	t_command	*cmd;

	if (!start)
		return (NULL);
	count_tokens(start, end, &arg_count, &redir_count);
	cmd = init_command_struct(arg_count, redir_count);
	if (!cmd)
		return (NULL);
	if (fill_command(cmd, start, end))
	{
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}
