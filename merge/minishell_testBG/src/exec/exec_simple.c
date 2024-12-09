/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:04:03 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/09 23:52:22 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int	exec_simple_cmd(t_token *tokens, t_ctx *ctx)
// {
// 	char	**args;
// 	int		result;

// 	args = prepare_args(tokens, ctx);
// 	if (!validate_args(args, ctx))
// 		return (0);
// 	if (is_builtin(args[0]))
// 	{
// 		// printf("test simple cmd : %s\n", args[0]);
// 		result = execute_builtin(ctx, args);
// 		free_tab(args);
// 		return (result);
// 	}
// 	print_command_not_found(args[0], ctx);
// 	free_tab(args);
// 	return (0);
// }

int exec_simple_cmd(t_token *tokens, t_ctx *ctx)
{
    if (!tokens)
        return 0;

    if (is_builtin(tokens->value))
        return execute_builtin(ctx, tokens);
    // Si ce n'est pas un builtin, afficher une erreur
    print_command_not_found(tokens->value, ctx);
    return 0;
}

// int	validate_args(char **args, t_ctx *ctx)
// {
// 	if (!args || !args[0])
// 	{
// 		perror("Erreur d'allocation de mémoire pour les arguments");
// 		free_tab(args);
// 		ctx->exit_status = 1;
// 		return (0);
// 	}
// 	return (1);
// }

int validate_args(char **args, t_ctx *ctx)
{
    if (!args || !args[0])
    {
        perror("Erreur d'allocation de mémoire pour les arguments");
        free_tab(args);
        ctx->exit_status = 1;
        return (0);
    }

    // Si c'est la commande export, on valide ses arguments
    if (ft_strcmp(args[0], "export") == 0)
    {
        int i = 1;
        while (args[i])
        {
            // Vérifie le cas "export ="
            if (ft_strcmp(args[i], "=") == 0)
            {
                fprintf(stderr, "MiniBG: export: `=': not a valid identifier\n");
                ctx->exit_status = 1;
                return (0);
            }
            // Vérifie si commence par un chiffre
            if (ft_isdigit(args[i][0]))
            {
                fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n", args[i]);
                ctx->exit_status = 1;
                return (0);
            }
            // Vérifie la présence de tirets dans le nom avant le '='
            char *equal_pos = ft_strchr(args[i], '=');
            char *dash_pos = ft_strchr(args[i], '-');
            if (dash_pos && (!equal_pos || dash_pos < equal_pos))
            {
                fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n", args[i]);
                ctx->exit_status = 1;
                return (0);
            }
            i++;
        }
    }
    return (1);
}

// int	execute_builtin(t_ctx *ctx, char **args)
// {
// 	char	**env_array;
// 	int		ret;

// 	env_array = convert_env_to_array(ctx);
// 	if (!env_array)
// 	{
// 		free_tab(args);
// 		ctx->exit_status = 1;
// 		return (0);
// 	}
// 	// printf("test builtin : %s\n", args[0]);
// 	ret = exec_builtin_cmd(args, env_array, ctx);
// 	free_tab(env_array);
// 	return (ret);
// }

void	print_command_not_found(const char *cmd, t_ctx *ctx)
{
	fprintf(stderr, "miniBG: %s: command not found\n", cmd);
	ctx->exit_status = 127;
}
