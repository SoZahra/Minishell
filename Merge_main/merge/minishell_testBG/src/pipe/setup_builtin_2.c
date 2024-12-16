/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_builtin_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:46:21 by llarrey           #+#    #+#             */
/*   Updated: 2024/12/16 17:45:04 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*find_builtin_start(t_token *tokens)
{
	while (tokens)
	{
		if (is_builtin(tokens->value))
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

char	*reconstruct_cmd_line_from_token(t_token *start_token, t_ctx *ctx)
{
	char	*cmd_line;
	char	*temp;
	(void)ctx;

	cmd_line = NULL;
	if (!start_token)
		return (NULL);
	cmd_line = ft_strdup(start_token->value);
	if (!cmd_line)
		return NULL;
	start_token = start_token->next;
	while (start_token && ft_strcmp(start_token->value, ">") != 0)
	{
		if (!start_token->had_space)
		{
			temp = cmd_line;
			cmd_line = ft_strjoin(cmd_line, " ");
			free(temp);
		}
		temp = cmd_line;
		cmd_line = ft_strjoin(cmd_line, start_token->value);
		free(temp);
		start_token = start_token->next;
		ft_fprintf(2, "avant %s\n", cmd_line);
	}
	ft_fprintf(2, "%s\n", cmd_line);
	return (cmd_line);
}

int	adjust_cmd_line_to_builtin(t_pipeline *pl, t_ctx *ctx)
{
	t_token	*builtin_start;
	char	*new_cmd_line;

	builtin_start = find_builtin_start(pl->cmd_start);
	if (builtin_start)
	{
		new_cmd_line = reconstruct_cmd_line_from_token(builtin_start, ctx);
		if (!new_cmd_line)
			return 1;
		free(pl->cmd_line);
		pl->cmd_line = new_cmd_line;
	}
	return 0;
}

int	has_redirect(t_pipeline *pl)
{
	t_token	*current;

	current = pl->cmd_start;
	while (current)
	{
		// fprintf(stderr, " value current : %s\n", current->value);
		if (ft_strcmp(current->value, ">") == 0
			|| ft_strcmp(current->value, "<") == 0
			|| ft_strcmp(current->value, ">>") == 0
			|| ft_strcmp(current->value, "<<") == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

int	has_pipe(t_pipeline *pl)
{
	t_token	*current;

	current = pl->cmd_start;
	while (current)
	{
		if (ft_strcmp(current->value, "|") == 0)
			return (1);
		current = current->next;
	}
	return (0);
}
