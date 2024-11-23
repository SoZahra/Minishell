/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:06:51 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 12:25:58 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_child(t_token *cmd_tokens, int fd_in, int pipe_fd[2],
		int last_cmd)
{
	if (fd_in != 0)
		dup2(fd_in, STDIN_FILENO);
	if (!last_cmd)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	exec_cmd(cmd_tokens, fd_in, pipe_fd, last_cmd);
}

void	handle_parent(int pipe_fd[2], int *fd_in, pid_t pid)
{
	close(pipe_fd[1]);
	waitpid(pid, NULL, 0);
	if (*fd_in != 0)
		close(*fd_in);
	*fd_in = pipe_fd[0];
}

// void	handle_line(char *line, char **env)
// {
// 	t_token	*tokens;

// 	add_history(line);
// 	tokens = parse_command_line(line);
// 	if (tokens)
// 	{
// 		if (check_consecutive_pipes(tokens) == -1)
// 		{
// 			free_tokens(tokens);
// 			return ;
// 		}
// 		print_tokens(tokens);
// 		if (contains_pipe(tokens))
// 			process_pline(tokens, env);
// 		else
// 			exec_simple_cmd(tokens, env);
// 		free_tokens(tokens);
// 	}
// }

int handle_line(char *line, t_var *myEnv, t_ctx *ctx)
{
    t_token *tokens;

    add_history(line);
    tokens = parse_command_line(line, ctx);
    if (tokens)
    {
        if (check_consecutive_pipes(tokens) == -1)
        {
            free_tokens(tokens);
            return 0;
        }
        // print_tokens(tokens);
        if (contains_pipe(tokens))
            process_pline(tokens, myEnv, ctx);
        else
            exec_simple_cmd(tokens, myEnv, ctx); // Passer le contexte ici
        free_tokens(tokens);
    }
    return 0;
}

