/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:59:08 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:18:15 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env_var	*create_new_env_var(char *env_str)
{
	char		*sep;
	t_env_var	*new_var;

	sep = ft_strchr(env_str, '=');
	if (!sep)
		return (NULL);
	new_var = calloc(1, sizeof(t_env_var));
	if (!new_var)
		return (NULL);
	new_var->name = ft_strndup(env_str, sep - env_str);
	if (!new_var->name)
	{
		free(new_var->name);
		new_var->name = NULL;
		free(new_var);
		new_var = NULL;
		return (NULL);
	}
	new_var->value = ft_strdup(sep + 1);
	if (!new_var->value)
	{
		free(new_var->name);
		return (free(new_var), NULL);
	}
	return (new_var);
}

static int	replace_existing_var(t_env_var *current, t_env_var *new_var)
{
	free(current->value);
	current->value = new_var->value;
	return (free(new_var->name), free(new_var), 1);
}

int	add_env_var_to_list(t_env_var **head, t_env_var *new_var)
{
	t_env_var	*current;
	t_env_var	*prev;

	if (!head || !new_var)
		return (1);
	if (!*head)
		return (*head = new_var, 1);
	current = *head;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, new_var->name) == 0)
			return (replace_existing_var(current, new_var));
		prev = current;
		current = current->next;
	}
	prev->next = new_var;
	return (0);
}

t_env_var	*build_env_list(char **envp)
{
	t_env_var	*head;
	int			i;
	t_env_var	*new_var;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_var = create_new_env_var(envp[i]);
		if (new_var)
			add_env_var_to_list(&head, new_var);
		i++;
	}
	return (head);
}
