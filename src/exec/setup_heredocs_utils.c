/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_heredocs_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:09:48 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:11:26 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static BOOL	cond1(t_command *tmp, int i)
{
	return (tmp->redirs[i].type == 'H' && tmp->redirs[i].heredoc_fd <= 0);
}

static BOOL	cond2(t_command *tmp, int i)
{
	return (tmp->redirs[i].heredoc_fd == -1 || get_ctx()->exit_status == 130);
}

int	setup_heredocs(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->redirs)
		{
			i = 0;
			while (tmp->redirs[i].type)
			{
				if (cond1(tmp, i))
				{
					get_ctx()->current_command = tmp;
					tmp->redirs[i].heredoc_fd
						= here_doc(tmp->redirs[i].file, get_ctx());
					if (cond2(tmp, i))
						return (1);
				}
				i++;
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
