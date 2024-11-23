/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:32:37 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 13:37:30 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ctx *initialize_ctx(void)
{
    t_ctx *ctx = malloc(sizeof(t_ctx));
    if (!ctx)
    {
        perror("Failed to allocate memory for t_ctx");
        return NULL;
    }

    ctx->env_vars = NULL; // Important : initialiser à NULL
    ctx->exit_status = 0;
    ctx->num_pipes = 0;
    ctx->oldpwd = NULL;
    ctx->pwd = NULL;

    return ctx;
}

void free_ctx(t_ctx *ctx)
{
    if (!ctx)
        return;

    // Libérer les variables d'environnement
    free_env_vars(ctx->env_vars);

    // Libérer oldpwd et pwd
    if (ctx->oldpwd)
        free(ctx->oldpwd);
    if (ctx->pwd)
        free(ctx->pwd);

    free(ctx);
}


int main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
         char **envp)
{
    t_var myEnv;
    t_ctx *ctx;

    // Initialisation des signaux
    init_sig();
    ctx = initialize_ctx();
    if (!ctx)
        return (fprintf(stderr, "Failed to initialize context\n"), 1);

    // Initialiser l'environnement dans t_var
    myEnv.env = get_environment(envp);
    // myEnv.env_vars= build_env_vars(envp);
    if (!myEnv.env)
    {
        free(ctx); // Libérer le contexte en cas d'erreur
        return (fprintf(stderr, "Failed to copy environment\n"), 1);
    }
    // Construire les variables d'environnement dans t_ctx
    ctx->env_vars = build_env_vars(envp);
    if (!ctx->env_vars)
    {
        free_environment(myEnv.env); // Libérer t_var
        free(ctx);                   // Libérer le contexte
        return (fprintf(stderr, "Failed to initialize environment variables\n"), 1);
    }
    // Lancer la boucle principale
    loop(&myEnv);
    // Libérer les ressources
    free_environment(myEnv.env);
    free_ctx(ctx);

    return (ctx->exit_status);
}


// int	main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
// 		char **envp)
// {
// 	t_var myEnv;

// 	init_sig();
// 	myEnv.env = get_environment(envp);
// 	if (!myEnv.env)
// 		return (fprintf(stderr, "Failed to copy environment\n"), 1);
// 	loop(&myEnv);
// 	free_environment(myEnv.env);
// 	return (0);
// }

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: '%s'\n", tokens->value);
		tokens = tokens->next;
	}
}
