/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:48:48 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 16:55:21 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_echo(t_token *token_list, t_ctx *ctx)
{
	int	n_option;

	n_option = 0;
	if (token_list && ft_strcmp(token_list->value, "-n") == 0)
	{
		n_option = 1;
		token_list = token_list->next;
	}
	write_echo_content(token_list, n_option);
	ctx->exit_status = 0;
}

void	write_echo_content(t_token *token_list, int n_option)
{
	t_token	*current;
	int		first_arg;
	int		prev_was_quote;
	char	*cleaned;

	current = token_list;
	first_arg = 1;
	prev_was_quote = 0;
	while (current)
	{
		if (!first_arg && !(prev_was_quote))
			write(STDOUT_FILENO, " ", 1);
		if (current->value)
		{
			cleaned = clean_dollar_quotes(current->value);
			write(STDOUT_FILENO, cleaned, ft_strlen(cleaned));
			free(cleaned);
		}
		first_arg = 0;
		prev_was_quote = (current->type == SINGLE_QUOTE
				|| current->type == DOUBLEQUOTE);
		current = current->next;
	}
	if (!n_option)
		write(STDOUT_FILENO, "\n", 1);
}
