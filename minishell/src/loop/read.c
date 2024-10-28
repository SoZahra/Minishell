/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:37:16 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/28 17:02:16 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	exec_simple_cmd(t_token *tokens, char **env)
{
	char	**args;
	pid_t	pid;

	args = prepare_print_args(tokens);
	// if (ft_strcmp(args[0], "cd") == 0)
	// {
	// 	ft_cd(args);
	// 	free(args);
	// 	return ;
	// }
	if(exec_builtin_cmd(args, env))
	{
		free(args);
		return ;
	}
	pid = fork();
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

void split_env_v(const char *input, char **var, char **value)
{
	const char *equal_sign;

	equal_sign = ft_strchr(input, '=');
	if(equal_sign)
	{
		size_t	var_len = equal_sign - input;
		*var = ft_strndup(input, var_len);
		*value = ft_strdup(equal_sign + 1); //apres le =
	}
	else
	{
		*var = ft_strdup(input);
		*value = NULL;
	}
}

int exec_builtin_cmd(char **args, char **env)
{
	if(ft_strcmp(args[0], "export") == 0 && args[1])
	{
		char *var = NULL;
		char *value = NULL;
		split_env_v(args[1], &var, &value);
		if(var && is_valid_id(var))
		{
			if(value)
				export_v(&env, var, value);
			else
				export_v(&env, var, "");
		}
		else
			fprintf(stderr, "not good caracter %s\n", args[1]);
		return (free(var), free(value), 1);
	}
	if(ft_strcmp(args[0], "unset") == 0 && args[1])
		return (unset_v(&env, args[1]), 1);
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args), 1);
	return (0);
}

void	read_and_exec(char **env)
{
	char	*line;
	int i;

	i = 0;
	line = readline(PROMPT);
	if (line == NULL)
	{
		write(1, "exit\n", 5);
		exit(0);
	}
	if(ft_strncmp(line, "exit", 4) == 0 && (line [4] == '\0' || line[4] == ' '))
	{
		write(1, "exit\n", 5);
		if(line[4] != '\0' && line[5] != '\0')
		{
			char *arg = &line[5];
			while(arg[i] && ft_isdigit(arg[i]))
				i++;
			if(arg[i] != '\0')
			{
				fprintf(stderr, "Exit : %s : numeric argument required\n", arg);
				exit(255);
			}
			exit(ft_atoi(arg));
		}
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
