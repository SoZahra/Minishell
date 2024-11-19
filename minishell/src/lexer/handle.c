/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:10:28 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/14 12:00:59 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	write_echo_content(t_token *token_list, int n_option)
{
	t_token	*current = token_list;
	int		first_arg = 1;

	while (current)
	{
		if (current->value && current->value[0] != '\0') // Ignore les arguments vides
		{
			if (!first_arg)
				write(STDOUT_FILENO, " ", 1); // Ajoute un espace seulement entre arguments non-vides
			write(STDOUT_FILENO, current->value, ft_strlen(current->value));
			first_arg = 0;
		}
		current = current->next;
	}
	if (!n_option)
		write(STDOUT_FILENO, "\n", 1);
}

void	handle_echo(t_token *token_list)
{
	int		n_option = 0;

	// Vérifie si le premier argument est "-n" pour ne pas ajouter de saut de ligne
	if (token_list && ft_strcmp(token_list->value, "-n") == 0)
	{
		n_option = 1;
		token_list = token_list->next; // Passe au token suivant
	}
	write_echo_content(token_list, n_option);
}

int	handle_quotes(char **line, int *in_quotes, char *quote_char, char *buffer,
		int *i, t_token **token_list)
{
	if (**line == '"' || **line == '\'')
	{
		if (*in_quotes && **line == *quote_char)
		{
			*in_quotes = 0;
			*quote_char = 0;
			if (*i > 0)  // Vérifie que le buffer contient des caractères avant d'ajouter le token
			{
				buffer[*i] = '\0';
				add_token(token_list, TOKEN_ARGUMENT, buffer);
				*i = 0;
			}
		}
		else if (!*in_quotes)
		{
			*in_quotes = 1;
			*quote_char = **line;
		}
		(*line)++;
		return (1);
	}
	return (0);
}

int	handle_whitespace(char **line, char *buffer, int *i, t_token **token_list,
		int in_quotes)
{
	if (is_whitespace(**line) && !in_quotes)
	{
		if (*i > 0)
		{
			buffer[*i] = '\0';
			add_token(token_list, TOKEN_ARGUMENT, buffer);
			*i = 0;
		}
		(*line)++;
		return (1);
	}
	return (0);
}

int	handle_special_chars(char **line, char *buffer, int *i,
		t_token **token_list, int in_quotes)
{
	char	special[3];

	ft_bzero(special, sizeof(special));
	if ((**line == '|' || **line == '<' || **line == '>') && !in_quotes)
	{
		if (*i > 0)
		{
			buffer[*i] = '\0';
			add_token(token_list, TOKEN_ARGUMENT, buffer);
			*i = 0;
		}
		special[0] = **line;
		(*line)++;
		if ((special[0] == '>' && **line == '>') || (special[0] == '<'
				&& **line == '<'))
			special[1] = *(*line)++;
		add_token(token_list, get_token_type(special), special);
		return (1);
	}
	return (0);
}
