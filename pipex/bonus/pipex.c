/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 10:26:38 by llarrey           #+#    #+#             */
/*   Updated: 2024/09/29 11:54:19 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	first_command(char **av, int *pipe_fd, char **env)
{
	int		fd;
	pid_t	pid;

	fd = open(av[1], O_RDONLY);
	if (fd < 0)
	{
		perror("open infile file failed");
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		exit(0);
	}
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(fd, 0);
		dup2(pipe_fd[1], 1);
		close_fd(pipe_fd, fd);
		exec(av[2], env);
	}
	else
		close(fd);
}

void	middle_command(char *cmd, int pipe_fd[1024][2], int i, char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		close(pipe_fd[i - 3][1]);
		close(pipe_fd[i - 2][0]);
		dup2(pipe_fd[i - 3][0], 0);
		dup2(pipe_fd[i - 2][1], 1);
		close(pipe_fd[i - 3][0]);
		close(pipe_fd[i - 2][1]);
		exec(cmd, env);
	}
	else
	{
		ft_close_pipe(pipe_fd, i - 3);
		close(pipe_fd[i - 2][1]);
		waitpid(pid, NULL, 0);
	}
}

// middle_command(argv[i], pipe_fd[i - 3], pipe_fd[i - 2], env);
// last_command(argv, pipe_fd[i - 3], pipe_fd[i - 2], env);

void	last_command(char **av, int pipe_fd[1024][2], int i, char **env)
{
	pid_t	pid;
	int		ac;
	int		fd;

	ac = 0;
	while (av[ac] != NULL)
		ac++;
	fd = ft_opener(av, ac, pipe_fd[i - 2]);
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		ft_close_pipe(pipe_fd, i - 3);
		close(pipe_fd[i - 2][0]);
		dup2(pipe_fd[i - 3][0], 0);
		dup2(fd, 1);
		close(pipe_fd[i - 2][1]);
		close_fd(pipe_fd[i - 3], fd);
		exec(av[ac - 2], env);
	}
	else
		close_fd(pipe_fd[i - 2], fd);
}

int	main(int ac, char *argv[], char *env[])
{
	int		i;
	int		pipe_fd[1024][2];
	int		status;

	if (ac < 5 || env == NULL)
		return (1);
	i = -1;
	while (++i < ac - 3)
	{
		if (pipe(pipe_fd[i]) == -1)
			return (1);
	}
	i = 2;
	first_command(argv, pipe_fd[0], env);
	while (++i < ac - 2)
		middle_command(argv[i], pipe_fd, i, env);
	last_command(argv, pipe_fd, i, env);
	ft_close_pipe(pipe_fd, i);
	waitpid(-1, &status, 0);
	return (0);
}

/* void	exec_pipe_n(char *cmd, char **env, int *pipe_fd)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	exec(cmd, env);
	perror("exec");
	exit(0);
} */

/* void	pipe_n(char *cmd, char **env, int ac, int fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		perror("pipe");
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		close(fd);
		exec_pipe_n(cmd, env, pipe_fd);
	}
	else if (ac - 2 > 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
	else
	{
		dup2(fd, STDOUT_FILENO);
		close_fd(pipe_fd, fd);
		exec(cmd, env);
	}
} */

//fill in the here_doc
/* void	fill_here_doc(char	**av, int *pipe_fd)
{
	char	*buffer;
	int		fd;

	fd = open(av[5], O_RDONLY);
	while (!0)
	{
		buffer = get_next_line(fd);
		if (strncmp(buffer, av[2], ft_strlen(av[2])) == 0)
		{
			free(buffer);
			exit(EXIT_FAILURE);
		}
		ft_putstr_fd(buffer, pipe_fd[1]);
		free(buffer);
	}
}

//here doc main function
void	here_doc(char **av)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		perror("pipe");
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		fill_here_doc(av, pipe_fd);
	}
	else
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		wait(NULL);
	}
} */

// if (strcmp(argv[1], "here_doc") == 0)
// 	{
// 		here_doc(argv);
// 	}
// think about when env is empty
