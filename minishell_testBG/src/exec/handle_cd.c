/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:55:35 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/06 15:30:17 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_cd_builtin(char **args, t_ctx *ctx)
{
	if (ft_cd(args, ctx) == 0)
		ctx->exit_status = 0;
	else
		ctx->exit_status = 1;
	return (1);
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

int	process_var_assignment(char *arg, t_ctx *ctx)
{
	char	*var;
	char	*value;

	var = NULL;
	value = NULL;
	if (!split_env_v(arg, &var, &value))
		return (1);
	if (!var || !is_valid_id(var))
		return (handle_invalid_identifier(arg, var, value));
	printf("test process var : %s\n", arg);
	return (create_and_add_var(ctx, var, value));
}
