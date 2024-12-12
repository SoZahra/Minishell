/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:07:27 by fzayani           #+#    #+#             */
<<<<<<< HEAD:minishell/src/pipe/pipe.c
/*   Updated: 2024/11/22 14:59:29 by llarrey          ###   ########.fr       */
=======
/*   Updated: 2024/12/03 17:16:40 by llarrey          ###   ########.fr       */
>>>>>>> Fat:merge/minishell_testBG/src/pipe/pipe.c
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// char	**prepare_args(t_token *tokens)
// {
// 	int		count;
// 	t_token	*current;
// 	char	**args;
// 	int		i;

// 	count = 0;
// 	current = tokens;
// 	while (current && current->type != TOKEN_PIPE)
// 	{
// 		count++;
// 		current = current->next;
// 	}
// 	args = malloc((count + 1) * sizeof(char *));
// 	if (!args)
// 	{
// 		perror("malloc failed");
// 		return (NULL);
// 	}
// 	current = tokens;
// 	i = 0;
// 	while (current && current->type != TOKEN_PIPE)
// 	{
// 		args[i++] = strdup(current->value);
// 		current = current->next;
// 	}
// 	args[i] = NULL;
// 	return (args);
// }

/* char **prepare_args(t_token *tokens, t_ctx *ctx)
{
	(void)ctx;
    int count = 0;
    t_token *current = tokens;
    char **args;
    int i = 0;

    // Compter les arguments jusqu'au prochain pipe
    while (current && current->type != TOKEN_PIPE)
    {
        count++;
        current = current->next;
    }

    args = malloc((count + 1) * sizeof(char *));
    if (!args)
    {
        perror("malloc failed");
        return NULL;
    }

    current = tokens;
    i = 0;
    while (current && current->type != TOKEN_PIPE)
    {
        args[i] = strdup(current->value); // Dupliquer la valeur après expansion
        if (!args[i])
        {
            perror("Duplication de mémoire échouée");
            for (int j = 0; j < i; j++)
                free(args[j]);
            free(args);
            return NULL;
        }
        i++;
        current = current->next;
    }
    args[i] = NULL;  // Ajouter NULL à la fin pour `execve`
    return args;
} */


// void	exec(t_token *cmd_tokens, char **env)
// {
// 	char	**option_cmd;
// 	char	*path;

// 	option_cmd = prepare_args(cmd_tokens);
// 	if (!option_cmd[0])
// 	{
// 		fprintf(stderr, "Error: Command is empty\n");
// 		free_tab_2(option_cmd);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (strcmp(option_cmd[0], "cd") == 0)
// 	{
// 		ft_cd(option_cmd);
// 		free_tab_2(option_cmd);
// 		return ;
// 	}
// 	path = get_path(option_cmd[0], env);
// 	if (execve(path, option_cmd, env) == -1)
// 	{
// 		perror("exec command");
// 		free_tab_2(option_cmd);
// 		exit(EXIT_FAILURE);
// 	}
// 	free_tab_2(option_cmd);
// }


/* int	exec(t_token *cmd_tokens, t_ctx *ctx)
{
	char	**option_cmd;
	char	*path;
	int exit_status = 0;

	option_cmd = prepare_args(cmd_tokens, ctx);
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
		return (0);
	}
	path = get_path(option_cmd[0], env);
	if (execve(path, option_cmd, env) == -1)
	{
		perror("exec command");
		free_tab_2(option_cmd);
		exit(EXIT_FAILURE);
	}
	free_tab_2(option_cmd);
	return (0);
} */

/* void	child(t_token *tokens, int *pipe_fd, char **env)
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
} */
