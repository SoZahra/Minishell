/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:17:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/10 11:29:34 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


t_env_var	*create_new_env_var(char *env_str)
{
	char		*sep;
	t_env_var	*new_var;

	sep = ft_strchr(env_str, '=');
	new_var = malloc(sizeof(t_env_var));
	if (!sep)
		return (NULL);
	if (!new_var)
		return (perror("malloc failed"), NULL);
	new_var->name = ft_strndup(env_str, sep - env_str);
	new_var->value = ft_strdup(sep + 1);
	new_var->next = NULL;
	if (!new_var->name || !new_var->value)
	{
		free(new_var->name);
		free(new_var->value);
		free(new_var);
		return (perror("malloc failed"), NULL);
	}
	return (new_var);
}

void add_env_var_to_list(t_env_var **head, t_env_var *new_var)
{
    t_env_var *current;
    t_env_var *prev = NULL;

    // printf("Debug: Adding var '%s=%s' to list\n", new_var->name, new_var->value);

    // Si la variable existe déjà, on met à jour sa valeur
    current = *head;
    while (current)
    {
        if (ft_strcmp(current->name, new_var->name) == 0)
        {
            // printf("Debug: Updating existing var '%s'\n", current->name);
            free(current->value);
            current->value = new_var->value;
            free(new_var->name);
            free(new_var);
            return;
        }
        prev = current;
        current = current->next;
    }

    // Si on arrive ici, la variable n'existe pas encore
    if (!*head)
    {
        // printf("Debug: Adding as first var\n");
        *head = new_var;
    }
    else
    {
        // printf("Debug: Adding at end of list\n");
        prev->next = new_var;
    }
}

// void	add_env_var_to_list(t_env_var **head, t_env_var *new_var)
// {
// 	t_env_var	*current;
// 	// printf("inside end \n");
// 	if (!*head)
// 		*head = new_var;
// 	else
// 	{
// 		current = *head;
// 		while (current->next)
// 			current = current->next;
// 		current->next = new_var;
// 	}
// }

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
