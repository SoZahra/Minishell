/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:55:35 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/08 17:45:02 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int	handle_cd_builtin(char **args, t_ctx *ctx)
// {
// 	if (ft_cd(args, ctx) == 0)
// 		ctx->exit_status = 0;
// 	else
// 		ctx->exit_status = 1;
// 	return (1);
// }
int handle_cd_builtin(char **args, t_ctx *ctx)
{
    int result = ft_cd(args, ctx);
    if (result == 0)
        ctx->exit_status = 0;
    else
        ctx->exit_status = 1;
    return (ctx->exit_status);
}

int	handle_special_case(char *arg, t_ctx *ctx)
{
	if (arg[0] == '$')
	{
		print_export(ctx);
		return (1);
	}
	return (0);
}

int process_var_assignment(char *arg, t_ctx *ctx)
{
    char *var;
    char *value;
    int result;

    var = NULL;
    value = NULL;

    if (!split_env_v(arg, &var, &value))
    {
        ctx->exit_status = 1;  // Set exit status on error
        return (1);
    }

    result = create_and_add_var(ctx, var, value);
    if (result != 0)
    {
        ctx->exit_status = 1;
    }
    return result;
}

// int	process_var_assignment(char *arg, t_ctx *ctx)
// {
// 	char	*var;
// 	char	*value;

// 	var = NULL;
// 	value = NULL;
// 	if (ft_strchr(arg, '-') != NULL)
//     {
//         fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n", arg);
//         return (1);
//     }
// 	if (!split_env_v(arg, &var, &value))
// 		return (1);
// 	if (!var || !is_valid_id(var))
// 		return (handle_invalid_identifier(arg, var, value));
// 	// printf("test process var : %s\n", arg);
// 	return (create_and_add_var(ctx, var, value));
// }