/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:07:27 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/23 12:09:46 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**prepare_args(t_token *tokens)
{
	int		count;
	t_token	*current;
	char	**args;
	int		i;

	count = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		count++;
		current = current->next;
	}
	args = malloc((count + 1) * sizeof(char *));
	if (!args)
	{
		perror("malloc failed");
		return (NULL);
	}
	current = tokens;
	i = 0;
	while (current && current->type != TOKEN_PIPE)
	{
		args[i++] = strdup(current->value);
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

void	exec(t_token *cmd_tokens, char **env)
{
	char	**option_cmd;
	char	*path;

	option_cmd = prepare_args(cmd_tokens);
	if (!option_cmd[0])
	{
		fprintf(stderr, "Error: Command is empty\n");
		free_tab_2(option_cmd);
		exit(EXIT_FAILURE);
	}
	if (strcmp(option_cmd[0], "cd") == 0)
	{
		ft_cd(option_cmd);
		free_tab_2(option_cmd);
		return ;
	}
	path = get_path(option_cmd[0], env);
	if (execve(path, option_cmd, env) == -1)
	{
		perror("exec command");
		free_tab_2(option_cmd);
		exit(EXIT_FAILURE);
	}
	free_tab_2(option_cmd);
}

void	child(t_token *tokens, int *pipe_fd, char **env)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	exec(tokens, env);
}

void	parent(t_token *tokens, int *pipe_fd, char **env)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	exec(tokens, env);
}

int	check_consecutive_pipes(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE && (!current->next
				|| current->next->type == TOKEN_PIPE))
		{
			fprintf(stderr, "Error: Consecutive pipes or missing command\n");
			return (-1);
		}
		current = current->next;
	}
	return (0);
}

int	contains_pipe(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}
