/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 10:26:38 by llarrey           #+#    #+#             */
/*   Updated: 2024/09/29 13:01:30 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* void	pipex(int f1, int f2, char *cmd1, char *cmd2) // global idea
{
	int		pipe_fd[2];
	int		status;
	pid_t	parent;
	pid_t	child;
	char	*path;
	char	**option_cmd;

	pipe(pipe_fd);
	child = fork();
	if (child < 0)
		return(perror("Fork: "));
	if (child == 0)
	{
		useExecve(cmd1, f1);
	}
	parent = fork();
	if (parent < 0)
		return(perror("Fork: "));
	if (parent == 0)
		useExecve(cmd2, f2);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(child, &status, 0);
	waitpid(parent, &status, 0);
} */

void	exec(char *cmd, char **env)
{
	char	**option_cmd;
	char	*path;

	option_cmd = ft_split(cmd, ' ');
	path = get_path(option_cmd[0], env);
	if (execve(path, option_cmd, env) == -1)
	{
		perror("exec command");
		free_tab(option_cmd);
		exit(0);
	}
}

void	child(char **av, int *pipe_fd, char **env)
{
	int	fd;

	fd = open(av[1], O_RDONLY);
	if (fd < 0)
	{
		perror("open infile file failed");
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		exit(0);
	}
	dup2(fd, 0);
	dup2(pipe_fd[1], 1);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	close(fd);
	exec(av[2], env);
}

void	parent(char **av, int *pipe_fd, char **env)
{
	int	fd;

	fd = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open output file failed");
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		exit(0);
	}
	dup2(fd, 1);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(fd);
	exec(av[3], env);
}

int	main(int ac, char *argv[], char *env[])
{
	pid_t	pid1;
	pid_t	pid2;
	int		pipe_fd[2];
	int		status;

	if (ac != 5 || env == NULL)
		return (write(2, "check the arguments / env potentialy empty", 43), 0);
	if (pipe(pipe_fd) == -1)
		exit_error();
	pid1 = fork();
	if (pid1 == -1)
		exit_error();
	if (pid1 == 0)
		child(argv, pipe_fd, env);
	pid2 = fork();
	if (pid2 == -1)
		exit_error();
	if (pid2 == 0)
		parent(argv, pipe_fd, env);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	return (0);
}
