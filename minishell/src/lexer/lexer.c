/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:39:48 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/30 17:45:30 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*parse_command_line(char *line)
{
	t_token	*token_list;
	char	buffer[1024];
	int		i;
	int		in_quotes;
	char	quote_char;

	token_list = NULL;
	i = 0;
	in_quotes = 0;
	quote_char = 0;
	while (*line)
	{
		if (handle_quotes(&line, &in_quotes, &quote_char, buffer, &i,
				&token_list))
			continue ;
		if (handle_whitespace(&line, buffer, &i, &token_list, in_quotes))
			continue ;
		if (handle_special_chars(&line, buffer, &i, &token_list, in_quotes))
			continue ;
		buffer[i++] = *line++;
	}
	if (finalize_tokens(in_quotes, quote_char, buffer, &i, &token_list) == -1)
		return (NULL);
	return (token_list);
}

t_token	*add_pipe_token(t_token **head, t_token **tail)
{
	t_token	*n_token;

	n_token = create_token(TOKEN_PIPE, "|");
	if (n_token)
	{
		if (!*head)
			*head = n_token;
		else
			(*tail)->next = n_token;
		*tail = n_token;
	}
	return (n_token);
}

void	process_token(t_token **head, t_token **tail, char *start, char *ptr,
		int first_token)
{
	char	*token_str;

	(void)tail;
	token_str = ft_strndup(start, ptr - start);
	if (!token_str)
		return ;
	if (first_token)
		add_token(head, TOKEN_COMMAND, token_str);
	else
		add_token(head, TOKEN_ARGUMENT, token_str);
	free(token_str);
}

void	handle_token(t_token **head, t_token **tail, char **ptr,
		int *first_token)
{
	char	*start;

	start = *ptr;
	while (**ptr && **ptr != ' ' && **ptr != '|')
		(*ptr)++;
	process_token(head, tail, start, *ptr, *first_token);
	*first_token = 0;
}

int	handle_env_var(char **ptr, t_token **head, t_token **tail, int *first_token)
{
	(void)tail;
	char	*start = *ptr;
	int		len = 1;

	// Vérifier si le prochain caractère est une lettre ou un underscore
	if (!isalpha(start[1]) && start[1] != '_')
		return (0); // Si ce n'est pas un nom valide de variable, on retourne 0

	(*ptr)++; // Avance au caractère suivant après le '$'
	while (isalnum(**ptr) || **ptr == '_')
	{
		(*ptr)++;
		len++;
	}

	// Extraire le nom de la variable (sans le '$')
	char *var_name = ft_strndup(start + 1, len - 1);
	if (!var_name)
		return (-1);

	// Ajouter un token pour cette variable d'environnement
	if (*first_token)
		add_token(head, TOKEN_COMMAND, var_name);
	else
		add_token(head, TOKEN_ARGUMENT, var_name);

	free(var_name);
	*first_token = 0;
	return (1); // Retourne 1 pour indiquer qu'on a traité un token de variable
}

t_token	*lexer(const char *input)
{
	t_token	*head = NULL;
	t_token	*tail = NULL;
	char	*ptr = ft_strdup(input);
	int		first_token = 1;

	if (!ptr)
		return (NULL);

	while (*ptr)
	{
		if (handle_space(&ptr))
			continue;
		if (*ptr == '|')
		{
			if (!create_token_from_pipe(&head, &tail))
				return (free(ptr), NULL);
			first_token = 1;
			ptr++;
			continue;
		}
		if (*ptr == '$' && handle_env_var(&ptr, &head, &tail, &first_token))
			continue;
		handle_token(&head, &tail, &ptr, &first_token);
	}
	return (free(ptr), head);
}

// t_token	*lexer(const char *input)
// {
// 	t_token	*head;
// 	t_token	*tail;
// 	char	*ptr;
// 	int		first_token;

// 	head = ((tail = NULL));
// 	ptr = ft_strdup(input);
// 	first_token = 1;
// 	if (!ptr)
// 		return (NULL);
// 	while (*ptr)
// 	{
// 		if (handle_space(&ptr))
// 			continue ;
// 		if (*ptr == '|')
// 		{
// 			if (!create_token_from_pipe(&head, &tail))
// 				return (free(ptr), NULL);
// 			first_token = 1;
// 			ptr++;
// 			continue ;
// 		}
// 		handle_token(&head, &tail, &ptr, &first_token);
// 	}
// 	return (free(ptr), head);
// }
