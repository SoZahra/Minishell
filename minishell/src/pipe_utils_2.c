/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:07:23 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/18 11:33:31 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token *extract_command(t_token *tokens)
{
	t_token *head;
	t_token *tail;

	head = 0;
	tail = 0;
	while(tokens && tokens->type != TOKEN_PIPE)
	{
		// t_token *new_t = malloc(sizeof(t_token));
        // new_t->type = tokens->type;
        // new_t->value = strdup(tokens->value);
        // new_t->next = NULL;
		t_token *new_t = malloc(sizeof(t_token));
		if (!new_t) {
		// Libérer la mémoire déjà allouée dans cette fonction si nécessaire
			perror("malloc failed");
			exit(EXIT_FAILURE);
		}
		new_t->value = strdup(tokens->value);
		if (!new_t->value) {
			free(new_t);
			perror("strdup failed");
			exit(EXIT_FAILURE);
		}
		if(!head)
		{
			head = new_t;
			tail = new_t;
		}
		else
		{
			tail->next =new_t;
			tail = new_t;
		}
		tokens = tokens->next;
	}
	return (head); // liste de tokens avant le pipe
}

t_token *extract_command_after(t_token *tokens)
{
	t_token *head;
	t_token *tail;

	head = NULL;
	tail = NULL;
	while(tokens && tokens->type != TOKEN_PIPE)
		tokens = tokens->next;
	if(tokens &&  tokens->type == TOKEN_PIPE)
		tokens = tokens->next;
	while(tokens) // apres pipe
	{
		// t_token *new_t = create_token(tokens->type, tokens->value);
		t_token *new_t = malloc(sizeof(t_token));
        new_t->type = tokens->type;
        new_t->value = strdup(tokens->value);
        new_t->next = NULL;
		if(!head)
		{
			head = new_t;
			tail = new_t;
		}
		else
		{
			tail->next =new_t;
			tail = new_t;
		}
		tokens = tokens->next;
	}
	return (head); // liste de tokens avant le pipe
}

int process_pline(t_token *tokens, char **env)
{
	pid_t pid1, pid2;
    int pipe_fd[2];
    int status;

    if (pipe(pipe_fd) == -1)
        exit_error();

    pid1 = fork();
    if (pid1 == -1)
        exit_error();
    if (pid1 == 0) // Processus enfant 1 (avant le pipe)
    {
        t_token *first_cmd_t = extract_command(tokens);
        child(first_cmd_t, pipe_fd, env);
    }
    pid2 = fork();
    if (pid2 == -1)
        exit_error();
    if (pid2 == 0) // Processus enfant 2 (après le pipe)
    {
        t_token *second_cmd_t = extract_command_after(tokens);
        parent(second_cmd_t, pipe_fd, env);
    }
    // Fermer les deux extrémités du pipe dans le processus parent
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    // Attendre la fin des deux processus enfants
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    return (0);
}
