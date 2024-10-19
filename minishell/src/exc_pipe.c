/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:02:31 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/17 14:03:44 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int validate_pipe_syntax(t_token *tokens)
{
    t_token *current = tokens;

    if (!current || current->type == TOKEN_PIPE) {
        fprintf(stderr, "bash: syntax error near unexpected token '|'\n");
        return -1;
    }

    while (current) {
        if (current->type == TOKEN_PIPE) {
            if (!current->next || current->next->type == TOKEN_PIPE) {
                fprintf(stderr, "bash: syntax error near unexpected token '|'\n");
                return -1;
            }
        }
        current = current->next;
    }
    return 0;
}


int valide_pipes(t_token *tokens)
{
	t_token *current = tokens;

	if(!current || current->type == TOKEN_PIPE)
		return(fprintf(stderr, "minibg : syntax error near unexpected token `|'\n"), -1);
	while(current)
	{
		if(current->type == TOKEN_PIPE)
		{
			if(!current->next || current->next->type == TOKEN_PIPE) //si le pipe est suivi d'un autre pipe ou de rien
				return(fprintf(stderr, "minibg : syntax error near unexpected token `|'\n"), -1);
		}
		current = current->next;
	}
	return (0);
}

void exc_error(t_token *tokens)
{
	(void)tokens;
	int pipefd[2];
	pid_t pid1;

	if(pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return;
	}
	pid1 = fork();
	if(pid1 == -1)
	{
		perror("fork failed");
		return;
	}
}

void exc_pipe(t_token *tokens)
{
	int pipefd[2];
	pid_t pid1;
	pid_t pid2;
	int status;
	char *grep_args;

	grep_args = NULL;
	if(pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return;
	}
	pid1 = fork();
	if(pid1 == -1)
	{
		perror("fork failed");
		return;
	}
	// exc_error(tokens);
	if(pid1 == 0) //premier enfant pour ecrire ls
	{
		close(pipefd[0]);//close une fois que la lecture est fini du pipe
		dup2(pipefd[1], STDOUT_FILENO); //rediriger vers l'entree standard
		close(pipefd[1]);//close une fois que la l'ecriture est faite
		char *args[] = {"/bin/ls", NULL}; // executer ls
		execve(args[0], args, NULL);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	pid2 = fork();
	if(pid2 == -1)//process pour grep
	{
		perror("fork failed");
		return;
	}
	if(pid2 == 0) //premier enfant pour ecrire ls
	{
		close(pipefd[1]);//close une fois que la lecture est fini du pipe
		dup2(pipefd[0], STDIN_FILENO); //rediriger vers l'entree standard
		close(pipefd[0]);//close une fois que la l'ecriture est faite
		t_token *current = tokens;
		while(current)
		{
			if(strcmp(current->value, "grep") == 0 && current->next)
			{
				grep_args = current->next->value; //stocker l'argument apres
				break;
			}
			current = current->next;
		}
		if(grep_args)
		{
			char *args[] = {"/bin/grep", grep_args, NULL}; //exc grep
			execve(args[0], args, NULL);
		}
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	// Parent : fermer les deux extrémités du pipe
    close(pipefd[0]);
    close(pipefd[1]);
    // Attendre la fin des deux processus enfants
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

}
