/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:40:11 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/08 19:33:04 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int handle_exit_builtin(char **args, t_ctx *ctx)
{
    process_exit_arg(args, ctx); 
    write(1, "exit\n", 5);
    exit(ctx->exit_status);
    return (1); // Ne sera jamais atteint
}

int handle_echo_builtin(char **args, t_ctx *ctx)
{
    t_token *token_list;

    token_list = create_token_list(args + 1);
    if (!token_list)
    {
        ctx->exit_status = 1;
        return (1);
    }
    handle_echo(token_list, ctx);
    return (1);
}


// int	handle_export_builtin(char **args, t_ctx *ctx)
// {
// 	// printf("test export : %s\n", args[0]);
	
// 	if (args[1] && (ft_strcmp(args[1], "\"\"") == 0 || ft_strlen(args[1]) == 0))
// 	{
// 		fprintf(stderr, "MiniBG: export: `': not a valid identifier\n");
// 		ctx->exit_status = 1;
// 		return (1);
// 	}
// 	if (args[1] && ft_strcmp(args[1], "$") == 0)
// 	{
// 		fprintf(stderr, "MiniBG: export: `$': not a valid identifier\n");
// 		ctx->exit_status = 1;
// 		return (1);
// 	}
// 	if (!args[1])
// 	{
// 		print_export(ctx);
// 		ctx->exit_status = 0;
// 		return (1);
// 	}
// 	return (handle_export_loop(args, ctx));
// }

int handle_export_builtin(char **args, t_ctx *ctx)
{
    // Si pas d'arguments, affiche les variables exportées
    if (!args[1])
    {
        print_export(ctx);
        ctx->exit_status = 0;
        return (1);
    }

    // Vérifie les chaînes vides
    if (args[1] && (ft_strcmp(args[1], "\"\"") == 0 || ft_strlen(args[1]) == 0))
    {
        fprintf(stderr, "MiniBG: export: `': not a valid identifier\n");
        ctx->exit_status = 1;
        return (1);
    }

    // Vérifie le symbole dollar seul
    if (args[1] && ft_strcmp(args[1], "$") == 0)
    {
        fprintf(stderr, "MiniBG: export: `$': not a valid identifier\n");
        ctx->exit_status = 1;
        return (1);
    }

    // Vérifie si l'argument commence par un chiffre
    if (args[1] && ft_isdigit(args[1][0]))
    {
        fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n", args[1]);
        ctx->exit_status = 1;
        return (1);
    }

    // Vérifie si l'argument est juste un '='
    if (args[1] && ft_strcmp(args[1], "=") == 0)
    {
        fprintf(stderr, "MiniBG: export: `=': not a valid identifier\n");
        ctx->exit_status = 1;
        return (1);
    }

    // Si aucune des conditions d'erreur n'est rencontrée,
    // traite les arguments avec handle_export_loop
    return (handle_export_loop(args, ctx));
}

int handle_export_loop(char **args, t_ctx *ctx)
{
    int i;
    int has_error;

    i = 1;
    has_error = 0;
    while (args[i])
    {
        if (handle_special_case(args[i], ctx))
        {
            i++;
            continue;
        }
        int current_error = process_var_assignment(args[i], ctx);
        has_error |= current_error;
        if (current_error)
            ctx->exit_status = 1;
        i++;
    }
    return (has_error ? 1 : 0);  // Retourne le bon code d'erreur
}

// int	handle_export_loop(char **args, t_ctx *ctx)
// {
// 	int	i;
// 	int	has_error;

// 	i = 1;
// 	has_error = 0;
// 	while (args[i])
// 	{
// 		if (handle_special_case(args[i], ctx))
// 		{
// 			i++;
// 			continue ;
// 		}
// 		// printf("test export loop : %s\n", args[0]);
// 		has_error |= process_var_assignment(args[i], ctx);
// 		i++;
// 	}
// 	ctx->exit_status = has_error;
// 	return (1);
// }
