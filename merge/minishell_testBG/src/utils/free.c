/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:57:25 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/17 17:44:48 by fzayani          ###   ########.fr       */
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
    if (ctx)
    {
        free_env(ctx->env_vars);
		ctx->env_vars = NULL;
        free(ctx->oldpwd);
		ctx->pwd = NULL;
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

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp->content);
		tmp->content = NULL;
		free(tmp);
		tmp = NULL;
		tokens = tokens->next;
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