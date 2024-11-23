/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 14:08:55 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 12:08:27 by fzayani          ###   ########.fr       */
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


t_token *add_token(t_token **token_list, t_token_type type, const char *value)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        perror("malloc failed");
        return NULL;
    }
    new_token->value = ft_strdup(value); // Duplicate the string
    if (!new_token->value)
    {
        free(new_token);
        perror("ft_strdup failed");
        return NULL;
    }
    new_token->type = type;
    new_token->next = NULL;

    if (!*token_list)
    {
        *token_list = new_token; // Set the head if the list is empty
    }
    else
    {
        t_token *current = *token_list;
        while (current->next)
            current = current->next;
        current->next = new_token; // Append to the end of the list
    }

    return new_token; // Return the newly created token
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

int	finalize_tokens(int in_quotes, char quote_char, char *buffer, int *i,
		t_token **token_list)
{
	if (in_quotes)
	{
		fprintf(stderr, "Syntax error : unmatched %c\n", quote_char);
		return (-1);
	}
	if (*i > 0)
	{
		buffer[*i] = '\0';
		add_token(token_list, TOKEN_ARGUMENT, buffer);
	}
	return (0);
}
