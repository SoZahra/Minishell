/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:47:46 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:36:00 by ymanchon         ###   ########.fr       */
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

t_token	*find_next_command(t_token *current)
{
	while (current && current->type != '|')
		current = current->next;
	if (!current)
		return (NULL);
	return (current->next);
}

t_command	*create_command_from_tokens_range(t_token *start, t_token *end)
{
	int			arg_count;
	int			redir_count;
	int			arg_i;
	int			redir_i;
	t_token		*current;
	t_command	*cmd;

	if (!start)
		return (NULL);
	current = start;
	arg_count = ((redir_count = arg_i = redir_i = 0));
	while (current && current != end && current->type != '|')
	{
		if (is_redirection(current->type))
		{
			redir_count++;
			if (current->next)
				current = current->next;
		}
		else if (is_argument(current->type))
			arg_count++;
		if (current)
			current = current->next;
	}
	cmd = init_command_struct(arg_count, redir_count);
	if (!cmd)
		return (NULL);
	current = start;
	while (current && current != end && current->type != '|')
	{
		if (is_redirection(current->type))
		{
			if (add_redir(cmd, current, &redir_i))
				return (free_command_list(cmd), NULL);
			if (current->next)
				current = current->next;
		}
		else if (is_argument(current->type))
		{
			if (add_argument(cmd, current, &arg_i))
				return (free_command_list(cmd), NULL);
		}
		current = current->next;
	}
	cmd->args[arg_i] = NULL;
	cmd->redirs[redir_i].type = 0;
	cmd->arg_count = arg_i;
	return (cmd);
}
