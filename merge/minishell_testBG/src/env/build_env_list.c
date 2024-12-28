/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:17:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/28 11:58:56 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


// t_env_var	*create_new_env_var(char *env_str)
// {
// 	char		*sep;
// 	t_env_var	*new_var;

// 	sep = ft_strchr(env_str, '=');
// 	if (!sep)
// 		return (NULL);
// 	new_var = malloc(sizeof(t_env_var));
// 	if (!new_var)
// 		return (perror("malloc failed"), NULL);
// 	new_var->name = ft_strndup(env_str, sep - env_str);
// 	new_var->value = ft_strdup(sep + 1);
// 	new_var->next = NULL;
// 	if (!new_var->name || !new_var->value)
// 	{
// 		free(new_var->name);
// 		free(new_var->value);
// 		free(new_var);
// 		return (perror("malloc failed"), NULL);
// 	}
// 	return (new_var);
// }

// t_env_var *create_new_env_var(char *env_str)
// {
//     char *sep;
//     t_env_var *new_var;
    
//     sep = ft_strchr(env_str, '=');
//     if (!sep)
//         return NULL;
//     new_var = malloc(sizeof(t_env_var));
//     if (!new_var)
//         return NULL;
//     new_var->name = NULL;
//     new_var->value = NULL;
//     new_var->next = NULL;
//     new_var->name = ft_strndup(env_str, sep - env_str);
//     if (!new_var->name)
//     {
//         free(new_var);
//         new_var = NULL;
//         free(env_str);
//         env_str = NULL;
//         return NULL;
//     }
//     new_var->value = ft_strdup(sep + 1);
//     if (!new_var->value)
//     {
//         free(new_var->name);
//         new_var->name = NULL;
//         free(new_var->value);
//         new_var->value = NULL;
//         free(new_var);
//         new_var = NULL;
//         free(sep);
//         sep = NULL;
//         return NULL;
//     }
//     return new_var;
// }

void free_env_var(t_env_var *var)
{
    if (var)
    {
        free(var->name);
        var->name = NULL;
        free(var->value);
        var->value = NULL;
        free(var);
    }
}

void free_env_list(t_env_var *head)
{
    t_env_var *temp;

    while (head)
    {
        temp = head->next; // Sauvegarde du pointeur vers l'élément suivant
        free_env_var(head); // Libère l'élément actuel
        head = temp;        // Passe au suivant
    }
}


t_env_var *create_new_env_var(char *env_str)
{
    char *sep;
    t_env_var *new_var;
    
    sep = ft_strchr(env_str, '=');
    if (!sep)
        return NULL;
    new_var = calloc(1, sizeof(t_env_var)); // Utiliser calloc pour init à 0
    if (!new_var)
        return NULL;
    new_var->name = ft_strndup(env_str, sep - env_str);
    if (!new_var->name)
    {
        free(new_var->name);
        new_var->name = NULL;
        free(new_var);
        new_var = NULL;
        return NULL;
    }
    new_var->value = ft_strdup(sep + 1);
    if (!new_var->value)
    {
        free(new_var->name);
        free(new_var);
        return NULL;
    }
    return new_var;
}

int add_env_var_to_list(t_env_var **head, t_env_var *new_var)
{
    if (!head || !new_var)
        return 1;
    if (!*head)
    {
        *head = new_var;
        return 1;
    }
    t_env_var *current;
    t_env_var *prev;

    current = *head;
	prev = NULL;
    while (current)
    {
        if (ft_strcmp(current->name, new_var->name) == 0)
        {
            free(current->value);
            current->value = new_var->value;
            return (free(new_var->name), free(new_var), 1);
        }
        prev = current;
        current = current->next;
    }
    if (!*head)
        *head = new_var;
    else
        prev->next = new_var;
    return(0);
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
