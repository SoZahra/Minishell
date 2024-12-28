/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:56:59 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 16:42:04 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_env(t_env_var *env_var)
{
	t_env_var	*current;
	t_env_var	*next;

	if (!env_var)
		return ;
	current = env_var;
	while (current)
	{
		next = current->next;
		if (current->name)
		{
			free(current->name);
			current->name = NULL;
		}
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = NULL;
		current = next;
	}
}

void	*free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	return (NULL);
}

void	free_command_list(t_command *cmd)
{
	t_command	*current;
	t_command	*next;

	current = cmd;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}
