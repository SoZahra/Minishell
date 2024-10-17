/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:07:27 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/17 18:20:42 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// char **prepare_args_p(t_token *tokens)
// {
// 	int count;
// 	t_token *current;
// 	char **args;
// 	int i;

// 	count = 0;
// 	current = tokens;
// 	i = 0;
// 	while(current && current->type != TOKEN_PIPE)
// 	{
// 		count++;
// 		current = current->next;
// 	}
// 	args = malloc((count + 1) * sizeof(char *));
// 	if(!args)
// 		return(perror("malloc failed"), NULL);
// 	current = tokens;
// 	while(current && current->type != TOKEN_PIPE)
// 	{
// 		args[i++] = current->value;
// 		current = current->next;
// 	}
// 	args[i] = NULL;
// 	return (args);
// }

char **prepare_args_p(t_token *cmd_tokens) {
    int count = 0;
    t_token *current = cmd_tokens;

    // Compter le nombre d'arguments
    while (current) {
        count++;
        current = current->next;
    }

    // Allocation de mémoire pour le tableau d'arguments
    char **args = malloc((count + 1) * sizeof(char *));
    if (!args) {
        perror("Erreur d'allocation mémoire pour args");
        return NULL;
    }

    current = cmd_tokens;
    int i = 0;
    while (current) {
        args[i] = strdup(current->value); // Assurez-vous de dupliquer la valeur
        if (!args[i]) {
            perror("Erreur lors de la duplication d'un argument");
            free(args);
            return NULL;
        }
        current = current->next;
        i++;
    }
    args[i] = NULL; // Terminer le tableau d'arguments

    return args;
}

void	exec(t_token *cmd_tokens, char **env)
{
	char	**option_cmd;
	char	*path;

	option_cmd = prepare_args_p(cmd_tokens);
	if (!option_cmd || !option_cmd[0])
	{
		free_tab(option_cmd);
		perror("Invalid command");
		exit(EXIT_FAILURE);
	}
	path = get_path(option_cmd[0], env);
	if(execve(path, option_cmd, env) == -1)
	{
		perror("exec command");
		free_tab(option_cmd);
		exit(EXIT_FAILURE);
	}
}

void	child(t_token *tokens, int *pipe_fd, char **env)
{
	// int	fd;

	// fd = open(input_file, O_RDONLY);
	// if (fd < 0)
	// {
	// 	perror("open infile file failed");
	// 	close(pipe_fd[1]);
	// 	close(pipe_fd[0]);
	// 	exit(0);
	// }
	// dup2(fd, 0);
	// dup2(pipe_fd[1], 1);
	// close(pipe_fd[1]);
	// close(pipe_fd[0]);
	// close(fd);
	// exec(tokens, env);
	close(pipe_fd[0]); // Fermer la lecture du pipe

    dup2(pipe_fd[1], STDOUT_FILENO); // Rediriger la sortie standard vers le pipe
    close(pipe_fd[1]); // Fermer l'écriture après redirection

    exec(tokens, env);
}

void	parent(t_token *tokens, int *pipe_fd, char **env)
{
	// int	fd;

	// fd = open(output_file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	// if (fd < 0)
	// {
	// 	perror("open output file failed");
	// 	close(pipe_fd[1]);
	// 	close(pipe_fd[0]);
	// 	exit(0);
	// }
	// dup2(fd, 1);
	// dup2(pipe_fd[0], 0);
	// close(pipe_fd[0]);
	// close(pipe_fd[1]);
	// close(fd);
	// exec(tokens, env);
	close(pipe_fd[1]); // Fermer l'écriture du pipe

    dup2(pipe_fd[0], STDIN_FILENO); // Rediriger l'entrée standard vers le pipe
    close(pipe_fd[0]); // Fermer la lecture après redirection

    exec(tokens, env);
}

// int	main(int ac, char *argv[], char *env[])
// {
// 	pid_t	pid1;
// 	pid_t	pid2;
// 	int		pipe_fd[2];
// 	int		status;

// 	if (ac != 5 || env == NULL)
// 		return (write(2, "check the arguments / env potentialy empty", 43), 0);
// 	if (pipe(pipe_fd) == -1)
// 		exit_error();
// 	pid1 = fork();
// 	if (pid1 == -1)
// 		exit_error();
// 	if (pid1 == 0)
// 		child(argv, pipe_fd, env);
// 	pid2 = fork();
// 	if (pid2 == -1)
// 		exit_error();
// 	if (pid2 == 0)
// 		parent(argv, pipe_fd, env);
// 	close(pipe_fd[0]);
// 	close(pipe_fd[1]);
// 	waitpid(pid1, &status, 0);
// 	waitpid(pid2, &status, 0);
// 	return (0);
// }