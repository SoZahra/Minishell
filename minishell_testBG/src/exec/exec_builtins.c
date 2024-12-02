/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:05:20 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 15:05:29 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int exec_builtin_cmd(char **args, char **env, t_ctx *ctx)
{
    int i;
    (void)env;

    if (ft_strcmp(args[0], "exit") == 0)
    {
        if (process_exit_arg(args, ctx))
            return 1;
        return 1;
    }

    if (ft_strcmp(args[0], "echo") == 0)
    {
        t_token *token_list = create_token_list(args + 1);
        if (token_list)
        {
            handle_echo(token_list, ctx);
            // free_tokens(token_list);
        }
        ctx->exit_status = 0;
        return 1;
    }
    if (ft_strcmp(args[0], "export") == 0)
    {
        if (args[1] && (ft_strcmp(args[1], "\"\"") == 0 || ft_strlen(args[1]) == 0))
        {
            fprintf(stderr, "bash: export: `': not a valid identifier\n");
            ctx->exit_status = 1;
            return 1;
        }

        // Cas spécial pour $ seul
        if (args[1] && ft_strcmp(args[1], "$") == 0)
        {
            fprintf(stderr, "bash: export: `$': not a valid identifier\n");
            ctx->exit_status = 1;
            return 1;
        }

        // Si pas d'arguments
        if (!args[1])
        {
            print_export(ctx);
            ctx->exit_status = 0;
            return 1;
        }
        i = 1;
        int has_error = 0;
        while (args[i])
        {
            char *var = NULL;
            char *value = NULL;


            if (args[i][0] == '$')
            {
                print_export(ctx);
                i++;
                continue;
            }
            if (!split_env_v(args[i], &var, &value))
            {
                has_error = 1;
                i++;
                continue;
            }
            if (!var || !is_valid_id(var))
            {
                fprintf(stderr, "export: `%s`: not a valid identifier\n", var ? var : args[i]);
                free(var);
                if (value)
                    free(value);
                has_error = 1;
                i++;
                continue;
            }
            t_env_var *new = malloc(sizeof(t_env_var));
            if (!new)
            {
                free(var);
                if (value)
                    free(value);
                has_error = 1;
                i++;
                continue;
            }
            new->name = var;
            new->value = value; // La valeur peut contenir n'importe quoi
            new->next = NULL;
            get_last_env_node(&get_ctx()->env_vars)->next = new;
            i++;
        }
        ctx->exit_status = has_error ? 1 : 0;
        return 1;
    }
    // if (ft_strcmp(args[0], "unset") == 0)
    // {
    //     if (!args[1])
    //     {
    //         ctx->exit_status = 0;
    //         return 1;
    //     }
	// 	for (int i = 1; args[i]; i++)
    //     {
    //         if (!is_valid_id(args[i]))
    //         {
    //             fprintf(stderr, "unset: `%s`: not a valid identifier\n", args[i]);
	// 			ctx->exit_status = 1;
    //             continue;
    //         }
    //         unset_v(&(ctx->env_vars), args[i]);
    //     }
    //     ctx->exit_status = 0;
    //     return 1;
    // }
	if (ft_strcmp(args[0], "pwd") == 0)
	{
		if (args[1]) // Check for extra arguments
			;
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			printf("%s\n", cwd);
			ctx->exit_status = 0; // Success
		}
		else
		{
			perror("pwd");
			ctx->exit_status = 1; // Failure
		}
		return 1;
	}
    if (ft_strcmp(args[0], "cd") == 0)
    {
        if (ft_cd(args, ctx) == 0)
            ctx->exit_status = 0;
        else
            ctx->exit_status = 1;
        return 1;
		// char **expanded_args = malloc((sizeof(char *) * (count_args(args) + 1)));
		// if (!expanded_args)
		// {
		// 	perror("malloc failed for expanded_args");
		// 	ctx->exit_status = 1;
		// 	return 1;
		// }
		// // Expansion des arguments
		// for (int i = 0; args[i]; i++)
		// 	expanded_args[i] = expand_variables(args[i], ctx, STRING); // Expansion normale
		// expanded_args[count_args(args)] = NULL;
		// // Exécute la commande cd
		// if (ft_cd(expanded_args, ctx) == 0)
		// 	ctx->exit_status = 0;
		// else
		// 	ctx->exit_status = 1;
		// free_args(expanded_args); // Libérer les arguments expandis
		// return 1;
	}
    if (ft_strcmp(args[0], "env") == 0)
    {
        print_env(get_ctx());
        return 1;
    }
    return 0; // Not a built-in command
}