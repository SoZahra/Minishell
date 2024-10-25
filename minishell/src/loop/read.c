/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:37:16 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/25 12:11:08 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	print_cmd(t_token *cmd, int index)
// {
// 	printf(" Command %d: ", index);
// 	while (cmd)
// 	{
// 		printf("%s ", cmd->value);
// 		cmd = cmd->next;
// 	}
// 	printf("\n");
// }

// void	print_cmds(t_token *cmd_tokens, int num_pipes)
// {
// 	int	k;

// 	k = 0;
// 	printf("Number of pipes: %d\n", num_pipes);
// 	while (k <= num_pipes)
// 	{
// 		print_cmd(&cmd_tokens[k], k);
// 		k++;
// 	}
// }

// void	create_pipe(int pipe_fd[2])
// {
// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("Echec de pipe");
// 		exit(EXIT_FAILURE);
// 	}
// }

// char	**prepare_print_args(t_token *cmd)
// {
// 	char	**args;
// 	int		k;

// 	k = 0;
// 	args = prepare_args(cmd);
// 	if (args == NULL || args[0] == NULL)
// 	{
// 		fprintf(stderr, "Erreur lors de la preparation des args\n");
// 		exit(EXIT_FAILURE);
// 	}
// 	printf("Execution de la commande : %s\n", args[0]);
// 	while (args[k] != NULL)
// 	{
// 		printf("Arguments %d : %s\n", k, args[k]);
// 		k++;
// 	}
// 	return (args);
// }

void	exec_cmd(t_token *cmd, int fd_in, int pipe_fd[2], int last_cmd)
{
	char	**args;

	args = prepare_print_args(cmd);
	if (fd_in != 0)
		dup2(fd_in, STDIN_FILENO);
	if (!last_cmd)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	if (execvp(args[0], args) == -1)
	{
		perror("Échec de exec");
		free(args);
		exit(EXIT_FAILURE);
	}
}

// void	handle_child(t_token *cmd_tokens, int fd_in, int pipe_fd[2],
// 		int last_cmd)
// {
// 	if (fd_in != 0)
// 		dup2(fd_in, STDIN_FILENO);
// 	if (!last_cmd)
// 		dup2(pipe_fd[1], STDOUT_FILENO);
// 	close(pipe_fd[1]);
// 	close(pipe_fd[0]);
// 	exec_cmd(cmd_tokens, fd_in, pipe_fd, last_cmd);
// }

// void	handle_parent(int pipe_fd[2], int *fd_in, pid_t pid)
// {
// 	close(pipe_fd[1]);
// 	waitpid(pid, NULL, 0);
// 	if (*fd_in != 0)
// 		close(*fd_in);
// 	*fd_in = pipe_fd[0];
// }

void	process_tokens(t_token *cmd_tokens, int num_pipes)
{
	int		pipe_fd[2];
	int		fd_in;
	int		j;
	pid_t	pid;

	fd_in = 0;
	j = 0;
	print_cmds(cmd_tokens, num_pipes);
	while (j <= num_pipes)
	{
		if (j < num_pipes)
			create_pipe(pipe_fd);
		pid = fork();
		if (pid == -1)
		{
			perror("Echec de fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
			handle_child(&cmd_tokens[j], fd_in, pipe_fd, j == num_pipes);
		else
			handle_parent(pipe_fd, &fd_in, pid);
		j++;
	}
}

// void handle_line(char *line, char **env)
// {
// 	t_token *tokens;

// 	add_history(line);
// 	tokens = parse_command_line(line);
// 	if(tokens)
// 	{
// 		if(check_consecutive_pipes(tokens) == -1)
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

void	exec_simple_cmd(t_token *tokens, char **env)
{
	char **args = prepare_print_args(tokens);
    if (ft_strcmp(args[0], "cd") == 0)
    {
        ft_cd(args);
        free(args);
        return;
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Echec fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        exec(tokens, env);
        exit(0);
    }
    waitpid(pid, NULL, 0);
    free(args);
}

void	read_and_exec(char **env)
{
	char	*line;

	line = readline(PROMPT);
	if (line == NULL)
	{
		write(1, "exit\n", 5);
		exit(0);
	}
	if (*line)
		handle_line(line, env);
	free(line);
}

void	loop(char **env)
{
	while (1)
		read_and_exec(env);
}
