/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:02:31 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 22:10:29 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	validate_pipe_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (!current || current->type == TOKEN_PIPE)
	{
		ft_fprintf(2, "bash: syntax error near unexpected token '|'\n");
		return (-1);
	}
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next || current->next->type == TOKEN_PIPE)
			{
				ft_fprintf(2,
					"bash: syntax error near unexpected token '|'\n");
				return (-1);
			}
		}
		current = current->next;
	}
	return (0);
}

int	valide_pipes(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (!current || current->type == TOKEN_PIPE)
		return (ft_fprintf(2,
				"minibg : syntax error near unexpected token `|'\n"), -1);
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next || current->next->type == TOKEN_PIPE)
				return (ft_fprintf(2,
						"Minibg : syntax error near unexpected token `|'\n"),
					-1);
		}
		current = current->next;
	}
	return (0);
}

int	exc_error(t_token *tokens)
{
	int		pipefd[2];
	pid_t	pid1;

	(void)tokens;
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return 0;
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork failed");
		return 0;
	}
	return 0;
}

int	exc_pipe(t_token *tokens)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;
	char	*grep_args;
	t_token	*current;

	grep_args = NULL;
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return (0);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork failed");
		return (0);
	}
	if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		char *args[] = {"/bin/ls", NULL};
		execve(args[0], args, NULL);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork failed");
		return (0);
	}
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		current = tokens;
		while (current)
		{
			if (strcmp(current->value, "grep") == 0 && current->next)
			{
				grep_args = current->next->value;
				break ;
			}
			current = current->next;
		}
		if (grep_args)
		{
			char *args[] = {"/bin/grep", grep_args, NULL};
			execve(args[0], args, NULL);
		}
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0);
	set_term_attr();
	waitpid(pid2, &status, 0);
	set_term_attr();
	return(0);
}

// int	exc_pipe(t_token *tokens)
// {
// 	int		pipefd[2];
// 	pid_t	pid1;
// 	pid_t	pid2;
// 	int		status;
// 	char	*grep_args;
// 	t_token	*current;

// 	grep_args = NULL;
// 	if (pipe(pipefd) == -1)
// 	{
// 		perror("pipe failed");
// 		return (0);
// 	}
// 	pid1 = fork();
// 	if (pid1 == -1)
// 	{
// 		perror("fork failed");
// 		return (0);
// 	}
// 	if (pid1 == 0)
// 	{
// 		close(pipefd[0]);
// 		dup2(pipefd[1], STDOUT_FILENO);
// 		close(pipefd[1]);
// 		char *args[] = {"/bin/ls", NULL};
// 		execve(args[0], args, NULL);
// 		perror("execve failed");
// 		exit(EXIT_FAILURE);
// 	}
// 	pid2 = fork();
// 	if (pid2 == -1)
// 	{
// 		perror("fork failed");
// 		return (0);
// 	}
// 	if (pid2 == 0)
// 	{
// 		close(pipefd[1]);
// 		dup2(pipefd[0], STDIN_FILENO);
// 		close(pipefd[0]);
// 		current = tokens;
// 		while (current)
// 		{
// 			if (strcmp(current->value, "grep") == 0 && current->next)
// 			{
// 				grep_args = current->next->value;
// 				break ;
// 			}
// 			current = current->next;
// 		}
// 		if (grep_args)
// 		{
// 			char *args[] = {"/bin/grep", grep_args, NULL};
// 			execve(args[0], args, NULL);
// 		}
// 		perror("execve failed");
// 		exit(EXIT_FAILURE);
// 	}
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	waitpid(pid1, &status, 0);
// 	waitpid(pid2, &status, 0);
// 	return(0);
// }
