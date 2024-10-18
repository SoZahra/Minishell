/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:07:27 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/18 14:22:58 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **prepare_args(t_token *tokens)
{
    int count = 0;
    t_token *current = tokens;

    // Compter les tokens (jusqu'à un PIPE ou à la fin)
    while (current && current->type != TOKEN_PIPE)
    {
        count++;
        current = current->next;
    }
    // Allouer le tableau d'arguments
    char **args = malloc((count + 1) * sizeof(char *));
    if (!args)
    {
        perror("malloc failed");
        return NULL;
    }
    // Remplir le tableau d'arguments
    current = tokens;
    int i = 0;
    while (current && current->type != TOKEN_PIPE)
    {
        args[i++] = strdup(current->value); // Chaque token est un argument
        current = current->next;
    }
    args[i] = NULL; // Terminer par NULL pour execve
    return args;
}


// char **prepare_args(t_token *tokens)
// {
//     int count = 0;
//     t_token *current = tokens;

//     // Compter les tokens
//     while (current && current->type != TOKEN_PIPE)
//     {
//         count++;
//         current = current->next;
//     }

//     // Allouer le tableau d'arguments
//     char **args = malloc((count + 1) * sizeof(char *));
//     if (!args) {
//         perror("malloc failed");
//         return NULL;
//     }

//     // Remplir le tableau d'arguments
//     current = tokens;
//     int i = 0;
//     while (current && current->type != TOKEN_PIPE) {
//         args[i++] = strdup(current->value); // Duplication de la valeur pour chaque argument
//         current = current->next;
//     }
//     args[i] = NULL; // Terminer par NULL
//     return args;
// }


void	exec(t_token *cmd_tokens, char **env)
{
	char	**option_cmd;
	char	*path;

	option_cmd = prepare_args(cmd_tokens);
	if (!option_cmd[0]) {
    fprintf(stderr, "Error: Command is empty\n");
    free_tab_2(option_cmd);
    exit(EXIT_FAILURE);
	}
	if (!option_cmd || !option_cmd[0])
	{
		free_tab_2(option_cmd);
		perror("Invalid command");
		exit(EXIT_FAILURE);
	}
	path = get_path(option_cmd[0], env);
	if(execve(path, option_cmd, env) == -1)
	{
		perror("exec command");
		free_tab_2(option_cmd);
		exit(EXIT_FAILURE);
	}
}

void child(t_token *tokens, int *pipe_fd, char **env) {
    close(pipe_fd[0]); // Fermer la lecture du pipe dans l'enfant
    dup2(pipe_fd[1], STDOUT_FILENO); // Rediriger la sortie vers le pipe
    close(pipe_fd[1]); // Fermer après redirection
    exec(tokens, env);
}

void parent(t_token *tokens, int *pipe_fd, char **env) {
    close(pipe_fd[1]); // Fermer l'écriture du pipe dans l'enfant
    dup2(pipe_fd[0], STDIN_FILENO); // Rediriger l'entrée vers le pipe
    close(pipe_fd[0]); // Fermer après redirection
    exec(tokens, env);
}

int check_consecutive_pipes(t_token *tokens)
{
    t_token *current = tokens;

    while (current)
    {
        // Vérifie s'il y a deux pipes consécutifs ou si un pipe est suivi de NULL
        if (current->type == TOKEN_PIPE && (!current->next || current->next->type == TOKEN_PIPE))
        {
            fprintf(stderr, "Error: Consecutive pipes or missing command\n");
            return -1;  // Erreur trouvée
        }
        current = current->next;
    }
    return 0;  // Pas d'erreur
}

int contains_pipe(t_token *tokens)
{
    while (tokens)
    {
        if (tokens->type == TOKEN_PIPE)
            return 1; // Un pipe trouvé
        tokens = tokens->next;
    }
    return 0; // Aucun pipe trouvé
}


// void	child(t_token *tokens, int *pipe_fd, char **env)
// {
// 	close(pipe_fd[0]); // Fermer la lecture du pipe
//     dup2(pipe_fd[1], STDOUT_FILENO); // Rediriger la sortie standard vers le pipe
//     close(pipe_fd[1]); // Fermer l'écriture après redirection
//     exec(tokens, env);
// }

// void	parent(t_token *tokens, int *pipe_fd, char **env)
// {
// 	close(pipe_fd[1]); // Fermer l'écriture du pipe
//     dup2(pipe_fd[0], STDIN_FILENO); // Rediriger l'entrée standard vers le pipe
//     close(pipe_fd[0]); // Fermer la lecture après redirection
//     exec(tokens, env);
// }

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