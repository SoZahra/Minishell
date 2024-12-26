/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:06:46 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/26 15:58:34 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_export(t_ctx *ctx)
{
	t_env_var	*current;

	current = ctx->env_vars;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->name, current->value);
		else
			printf("%s\n", current->name);
		current = current->next;
	}
}

int	handle_no_args(t_ctx *ctx)
{
	print_export(ctx);
	ctx->exit_status = 0;
	return (0);
}

int	handle_no_equal(const char *arg, t_ctx *ctx)
{
	if (!is_valid_var_name(arg))
		return (handle_error(arg, ctx));
	return (create_and_add_var(ctx, ft_strdup(arg), NULL));
}

int	handle_single_arg(char *args, t_ctx *ctx)
{
	int		result;
	char	*arg;

	arg = ft_strdup(args);
	if (!arg)
		return (1);
	result = export_single_var(arg, ctx);
	free(arg);
	return (result);
}
