/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 14:40:09 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/28 14:05:08 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exit_builtin(const char *input, t_ctx *ctx)
{
	char	**arg_array;
	int		i;
	int		should_exit;

	while (*input == ' ')
		input++;
	arg_array = ft_split(input, ' ');
	if (!arg_array)
	{
		ctx->exit_status = 1;
		clear_exit(ctx);
		free((void *)input);
		exit(1);
	}
	should_exit = process_exit_arg(arg_array, ctx);
	i = 0;
	free_args(arg_array);
	if (should_exit)
	{
		clear_exit(ctx);
		free((void *)input);
		exit(ctx->exit_status);
	}
	return (1);
}

int	handle_echo_builtin(const char *args, t_ctx *ctx)
{
	while (*args == ' ')
		args++;
	if (args[0] == '-'
		&& (ft_strchr(args, 'n') == args + 1
			|| (ft_strspn(args + 1, "n") == ft_strlen(args + 1))))
	{
		return (handle_echo_builtin_n(args, ctx));
	}
	write(STDOUT_FILENO, args, ft_strlen(args));
	write(STDOUT_FILENO, "\n", 1);
	ctx->exit_status = 0;
	return (0);
}

int	handle_echo_builtin_n(const char *args, t_ctx *ctx)
{
	t_token	*tokens;
	t_token	*current;
	int		no_newline;
	int		first;

	tokens = tokenize_input((char *)args);
	no_newline = 0;
	current = tokens;
	while (current && is_valid_n(current))
	{
		no_newline = 1;
		current = current->next;
	}
	first = 1;
	while (current)
	{
		if (!first)
			write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, current->value, ft_strlen(current->value));
		first = 0;
		current = current->next;
	}
	if (!no_newline)
		write(STDOUT_FILENO, "\n", 1);
	return (free_tokens(tokens), ctx->exit_status = 0, 0);
}

int	handle_error(const char *arg, t_ctx *ctx)
{
	ft_fprintf(2, "MiniBG: export: `%s': not a valid identifier\n", arg);
	ctx->exit_status = 1;
	return (1);
}
