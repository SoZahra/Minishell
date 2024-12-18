/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:57:25 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/18 16:25:40 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_env_copy(char **env_copy)
{
	int	i;

	if (!env_copy)
		return ;
	i = 0;
	while (env_copy[i])
	{
		free(env_copy[i]);
		i++;
	}
	free(env_copy);
}

void	free_env(t_env_var *env_var)
{
	t_env_var	*to_free;

	while (env_var)
	{
		to_free = env_var;
		env_var = env_var->next;
		free(to_free->name);
		to_free->name = NULL;
		free(to_free->value);
		to_free->value = NULL;
		free(to_free);
		to_free = NULL;
	}
}

void free_ctx(t_ctx *ctx)
{
    if (!ctx)
        return;
    if (ctx->env_vars)
    {
        free_env(ctx->env_vars);
        ctx->env_vars = NULL;
    }
    if (ctx->tokens)
    {
        t_token *current = ctx->tokens;
        t_token *next;

        while (current)
        {
            next = current->next;
            if (current->value)
            {
                free(current->value);
                current->value = NULL;
            }
            // Ne pas free content car il pointe vers value
            current->content = NULL;
            free(current);
            current = next;
        }
        ctx->tokens = NULL;
    }
    if (ctx->oldpwd)
    {
        free(ctx->oldpwd);
        ctx->oldpwd = NULL;
    }
    if (ctx->pwd)
    {
        free(ctx->pwd);
        ctx->pwd = NULL;
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

// void free_tokens(t_token *tokens)
// {
//     t_token *current;
//     t_token *next;

//     if (!tokens)
//         return;

//     current = tokens;
//     while (current)
//     {
//         next = current->next;
//         if (current->value)
//         {
//             free(current->value);
//             current->value = NULL;
//         }
//         // Ne pas free content s'il pointe vers value
//         if (current->content && current->content != current->value)
//         {
//             free(current->content);
//             current->content = NULL;
//         }
//         free(current);
//         current = next;
//     }
// }

void free_tokens(t_token *tokens)
{
    t_token *current;
    t_token *next;

    if (!tokens)
        return;

    current = tokens;
    while (current)
    {
        next = current->next;
        if (current->value)
        {
            free(current->value);
            current->value = NULL;
        }
        // Ne pas gérer content du tout car il ne devrait pas être alloué
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

void free_pipeline_pipes(int **pipes, int num_commands)
{
    if (!pipes || num_commands <= 1)
        return;
        
    for (int i = 0; i < num_commands - 1; i++)
    {
        if (pipes[i])
		{
            free(pipes[i]);
			pipes[i] = NULL;
		}
    }
    free(pipes);
	pipes = NULL;
}