/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:53:55 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 14:54:13 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(t_token_type type)
{
	return (type == '<' || type == '>' || type == 'A' || type == 'H');
}

int	add_redir(t_command *cmd, t_token *token, int *redir_i)
{
	if (!cmd || !cmd->redirs)
		return (1);
	cmd->redirs[*redir_i].type = token->type;
	if (token->type)
	{
		cmd->redirs[*redir_i].file = ft_strdup(token->next->value);
		if (!cmd->redirs[*redir_i].file)
			return (1);
	}
	(*redir_i)++;
	return (0);
}
