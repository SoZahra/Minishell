/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:57:25 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/13 13:35:26 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


// Fonction pour free une liste de redirections
void free_redirections(t_redirection *redirs)
{
   t_redirection *curr;
   t_redirection *next;

   curr = redirs;
   while (curr)
   {
       next = curr->next;
       free(curr->file);
       free(curr);
       curr = next;
   }
}

// Fonction pour free une liste chaînée de commandes
void free_commands(t_command *cmd)
{
   t_command *curr;
   t_command *next;

   curr = cmd;
   while (curr)
   {
       next = curr->next;
       // Free les arguments
       if (curr->args)
       {
           for (int i = 0; i < curr->arg_count; i++)
               free(curr->args[i]);
           free(curr->args);
       }
       // Free le chemin
       free(curr->path);
       // Free les redirections
       free_redirections(curr->redirs);
       // Free le tableau had_spaces
       free(curr->had_spaces);
       // Free la commande elle-même
       free(curr);
       curr = next;
   }
}

// Fonction pour free une liste chaînée de variables d'environnement
void free_env_vars(t_env_var *env)
{
   t_env_var *curr;
   t_env_var *next;

   curr = env;
   while (curr)
   {
       next = curr->next;
       free(curr->name);
       free(curr->value);
       free(curr);
       curr = next;
   }
}

// Fonction pour free tous les tokens
void free_tokens(t_token *tokens)
{
   t_token *curr;
   t_token *next;

   curr = tokens;
   while (curr)
   {
       next = curr->next;
       free(curr->value);
       free(curr->content);
       free(curr);
       curr = next;
   }
}

void free_token_resources(t_token *token)
{
    if (token)
    {
        free(token->value);
        free(token->content);
        free(token);
    }
}

// Fonction principale pour tout free
void free_all(t_ctx *ctx)
{
    if (!ctx)
        return;
    free_env_vars(ctx->env_vars);
    free(ctx->oldpwd);
    free(ctx->pwd);
    if (ctx->next)
    {
        free_all(ctx->next);
        ctx->next = NULL;
    }
    // Maintenant on peut free ctx
    free(ctx);
}

//---------------
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

// void	free_tokens(t_token *tokens)
// {
// 	t_token	*tmp;

// 	while (tokens)
// 	{
// 		tmp = tokens;
// 		tokens = tokens->next;
// 		free(tmp->value);
// 		free(tmp->content);
// 		free(tmp);
// 	}
// }

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
