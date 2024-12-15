/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:09:37 by llarrey           #+#    #+#             */
/*   Updated: 2024/12/15 18:14:13 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_for_pipe(t_pipeline *pl)
{
	t_token	*tokens;
	int		flag;

	flag = 0;
	tokens = pl->cmd_start;
	while (tokens)
	{
		if (is_builtin(tokens->value))
			flag = 1;
		if (ft_strcmp(tokens->value, "|") == 0
			&& is_builtin(tokens->next->value) != 0)
			flag = 0;
		tokens = tokens->next;
	}
	return (flag);
}

int	check_for_pipe_builtin(t_pipeline *pl)
{
	t_token	*tokens;
	int		flag;

	flag = 0;
	tokens = pl->cmd_start;
	while (tokens)
	{
		if (ft_strcmp(tokens->value, "export") == 0)
			flag = 1;
		tokens = tokens->next;
	}
	return (flag);
}

void	read_heredoc(int fd, char *limiter)
{
	char	*buf;

	while (1)
	{
		buf = NULL;
		buf = readline("> ");
		if (!buf)
		{
			exit(EXIT_FAILURE);
			break ;
		}
		if (!ft_strncmp(limiter, buf, INT_MAX))
			break ;
		write(fd, buf, ft_strlen(buf));
		write(fd, "\n", 1);
		free(buf);
	}
	free(buf);
	close(fd);
}

int	here_doc(char *limiter)
{
	int	fd;

	fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		exit(EXIT_FAILURE);
	read_heredoc(fd, limiter);
	fd = open(".heredoc.tmp", O_RDONLY);
	if (fd >= 0)
		unlink(".heredoc.tmp");
	return (fd);
}

void	handle_input_redirection(t_token *redir_token, t_redir *redir)
{
	int	input_fd;

	if (ft_strcmp(redir_token->value, "<<") == 0)
		input_fd = here_doc(redir_token->next->value);
	else
		input_fd = open(redir_token->next->value, O_RDONLY);
	if (input_fd == -1)
		exit(EXIT_FAILURE);
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
	redir->input = 1;
}