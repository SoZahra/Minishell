/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:57:25 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/03 13:44:53 by fzayani          ###   ########.fr       */
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
		free(to_free->value);
		free(to_free);
	}
}

void	free_ctx(t_ctx *ctx)
{
	t_ctx	*to_free;

	while (ctx)
	{
		to_free = ctx;
		ctx = ctx->next;
		free_env(to_free->env_vars);
		free(to_free->oldpwd);
		free(to_free->pwd);
		free(to_free);
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
		tokens = tokens->next;
		free(tmp->value);
		free(tmp->content);
		free(tmp);
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
		i++;
	}
	free(args);
}