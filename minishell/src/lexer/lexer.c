/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:39:48 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/23 14:52:51 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		perror("malloc failed");
		return (NULL);
	}
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **head, t_token_type type, const char *value)
{
	t_token	*new_token;
	t_token	*current;

	new_token = create_token(type, value);
	if (!new_token)
		return ;
	if (!*head)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
}

t_token_type	get_token_type(const char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(str, "<") == 0)
		return (TOKEN_REDIRECT_INPUT);
	if (ft_strcmp(str, ">") == 0)
		return (TOKEN_REDIRECT_OUTPUT);
	if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_REDIRECT_APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	return (TOKEN_ARGUMENT);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

t_token	*parse_command_line(char *line)
{
	t_token	*token_list;
	char	buffer[1024];
	int		i;
    int in_quotes;
	char quote_char;

	token_list = NULL;
	i = 0;
    in_quotes = 0;
    quote_char = 0;
	while (*line)
	{
		if ((*line == '"' || *line == '\'') && (!in_quotes
				|| *line == quote_char))
		{
			if (in_quotes)
			{
				in_quotes = 0;
				quote_char = 0;
				buffer[i] = '\0';
				add_token(&token_list, TOKEN_ARGUMENT, buffer);
				i = 0;
			}
			else
			{
				in_quotes = 1;
				quote_char = *line;
			}
			line++;
			continue ;
		}
		if (is_whitespace(*line) && !in_quotes)
		{
			if (i > 0)
			{
				buffer[i] = '\0';
				add_token(&token_list, TOKEN_ARGUMENT, buffer);
				i = 0;
			}
			line++;
			continue ;
		}
		if ((*line == '|' || *line == '<' || *line == '>') && !in_quotes)
		{
			if (i > 0)
			{
				buffer[i] = '\0';
				add_token(&token_list, TOKEN_ARGUMENT, buffer);
				i = 0;
			}
			char special[3] = {0};
			special[0] = *line;
			line++;
			if ((*special == '>' && *line == '>') || (*special == '<'
					&& *line == '<'))
			{
				special[1] = *line;
				line++;
			}
			add_token(&token_list, get_token_type(special), special);
		}
		else
			buffer[i++] = *line++;
	}
	if (in_quotes)
	{
		fprintf(stderr, "Error: Unmatched %c\n", quote_char);
		return (NULL);
	}
	if (i > 0)
	{
		buffer[i] = '\0';
		add_token(&token_list, TOKEN_ARGUMENT, buffer);
	}
	return (token_list);
}

t_token	*lexer(const char *input)
{
	t_token	*head;
	t_token	*tail;
	char	*input_copy;
	char	*ptr;
	t_token	*new_token;
	t_token	*tmp;

	head = NULL;
	tail = NULL;
	input_copy = ft_strdup(input);
	ptr = input_copy;
	int first_token = 1;
	while (*ptr)
	{
		if (*ptr == ' ')
		{
			ptr++;
			continue ;
		}
		if (*ptr == '|')
		{
			new_token = create_token(TOKEN_PIPE, "|");
			if (!new_token)
			{
				free(input_copy);
				return (NULL);
			}
			first_token = 1;
			if (!head)
			{
				head = new_token;
				tail = new_token;
			}
			else
			{
				tail->next = new_token;
				tail = new_token;
			}
			ptr++;
			continue ;
		}
		char *start = ptr;
		while (*ptr && *ptr != ' ' && *ptr != '|')
			ptr++;
		char *token_str = ft_strndup(start, ptr - start);
		new_token = NULL;
		if (first_token)
		{
			new_token = create_token(TOKEN_COMMAND, token_str);
			first_token = 0;
		}
		else
			new_token = create_token(TOKEN_ARGUMENT, token_str);
		free(token_str);
		if (!new_token)
		{
			while (head)
			{
				tmp = head;
				head = head->next;
				(free(tmp->value), free(tmp));
			}
			free(input_copy);
			return (NULL);
		}
		if (!head)
		{
			head = new_token;
			tail = new_token;
		}
		else
		{
			tail->next = new_token;
			tail = new_token;
		}
	}
	free(input_copy);
	return (head);
}
