/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:07:23 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/25 19:03:42 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* t_token *extract_command(t_token *tokens)
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
} */

#include <stdlib.h>
#include <string.h>

#include <stdlib.h>

t_token *extract_command(t_token *tokens)
{
    t_token *command_list = NULL;      // Start of the sub-list for the command
    t_token *last_command = NULL;      // Pointer to keep track of the end of the new list

    while (tokens != NULL && tokens->type != TOKEN_PIPE && tokens->type != TOKEN_REDIRECT_INPUT && tokens->type != TOKEN_REDIRECT_OUTPUT)
    {
        // Duplicate the current token
        t_token *new_token = malloc(sizeof(t_token));
        if (!new_token)
            exit_error(); // Handle memory allocation error

        new_token->value = strdup(tokens->value); // Copy the value
        new_token->type = tokens->type;
        new_token->next = NULL;

        // Link the new token to the command list
        if (command_list == NULL)
            command_list = new_token;  // First token becomes the head of the new list
        else
            last_command->next = new_token; // Link the previous node to this one
        
        last_command = new_token; // Update the last command pointer

        tokens = tokens->next; // Move to the next token in the original list
    }

    return command_list; // Return the sub-list of tokens representing the command
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

t_token *extract_command_n(t_token *tokens, int n)
{
    t_token *current = tokens;
    t_token *cmd_start = NULL;  // Start of the command to extract
    int command_count = 0;      // Track the command number

    while (current != NULL)
    {
		if (current->type == TOKEN_REDIRECT_INPUT
			|| current->type == TOKEN_REDIRECT_INPUT)
            current = current->next;
        if (current->type == TOKEN_PIPE)
        {
            command_count++;
        }
        else if (command_count == n)
        {
            if (cmd_start == NULL)
                cmd_start = current;
        }
        else if (command_count > n)
            break;
        current = current->next;
    }
    if (cmd_start != NULL)     // If we found the n-th command, return the starting token of it
        return (cmd_start);
    return NULL;
}

t_token *extract_input_after_redirect(t_token *tokens) 
{
    t_token *current = tokens;

    while (current != NULL) 
    {
        if (current->type == TOKEN_REDIRECT_INPUT) 
        { 
            if (current->next != NULL) 
                return current->next;
        }
        current = current->next;
    }
    return NULL;
}

int	**pipe_tab(t_token *tokens)
{
	int		**pipe_fd;
	int		i;

	i = count_commands(tokens);
	pipe_fd = (int **)malloc(sizeof(int *) * i + 1);
	if (pipe_fd == NULL)
		return (NULL);
	while (i > 0)
	{
		pipe_fd[i] = (int *)malloc(sizeof(int) * 2);
		if (pipe_fd[i] == NULL)
			return (NULL);
		i--;
	}
	return (pipe_fd);
}

int get_output_fd(t_token *output_token)
{ 
    int fd = open(output_token->value, O_RDONLY);

    if (output_token == NULL) {
        exit_error();
    }
    if (fd == -1)
        exit_error();
    return fd;  // Return the valid file descriptor
}

int get_input_fd(t_token *input_token)
{ 
    int fd = open(input_token->value, O_RDONLY);

    if (input_token == NULL) {
        exit_error();
    }
    if (fd == -1)
        exit_error();
    return fd;  // Return the valid file descriptor
}

t_token *extract_output(t_token *tokens) 
{
    t_token *current = tokens;

    while (current != NULL) 
    {
        if (current->type == TOKEN_REDIRECT_OUTPUT) 
        { 
            if (current->next != NULL) 
                return current->next;
        }
        current = current->next;
    }
    return NULL;
}

t_token *extract_input(t_token *tokens) 
{
    t_token *current = tokens;

    while (current != NULL) 
    {
        if (current->type == TOKEN_REDIRECT_INPUT) 
        { 
            if (current->next != NULL) 
                return current->next;
        }
        current = current->next;
    }
    return NULL;
}

int	count_commands(t_token *tokens)
{
	int	i;
	t_token	*current;

	i = 0;
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			i++;
		current = current->next;
	}
	return (i);
}

int	check_redirection_input(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_INPUT)
			return(1);
		current = current->next;
	}
	return (0);
}

int	is_pipe(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			return(1);
		current = current->next;
	}
	return (0);
}

int	check_redirection_output(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_OUTPUT)
			return(1);
		current = current->next;
	}
	return (0);
}

int is_command(t_token *token) 
{
    if (token->type == TOKEN_PIPE ||
        token->type != TOKEN_REDIRECT_INPUT ||
        token->type != TOKEN_REDIRECT_OUTPUT)
        return(0);
    return (1);
}

/* int process_pline(t_token *tokens, char **env)
{
	pid_t pid1, pid2;
    int **pipe_fd;
    int status;
	int	i;

	pipe_fd = pipe_tab(tokens);
	i = count_commands(tokens);
    if (pipe(pipe_fd) == -1)
        exit_error();
	while (--i + 2 > 0)
	{
		if (pipe(pipe_fd[i]) == -1)
			exit_error();
	}
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
} */

int process_pline(t_token *tokens, char **env)
{
    int i;
    int num_cmds = count_commands(tokens);  // Number of commands through number of pipe
    int pipe_fd[2];                         // Pipe file descriptors: pipe_fd[0] = read, pipe_fd[1] = write
    int prev_fd = -1;                       // Previous pipe's read end (initially none)
    pid_t pid;
    int status;

    //fprintf(stderr, "Test : %i\n", num_cmds);
    for (i = 0; i <= num_cmds; i++)
    {
        if (i < num_cmds)// Create a pipe for the next command, except for the last one
        {
            if (pipe(pipe_fd) == -1)
                exit_error();
        }
        pid = fork();
        if (pid == -1)
            exit_error();
        if (pid == 0) // Child process
        {
          /*   if (i = 0 && check_redirection_input(tokens))
            {
                fprintf(stderr, "Test Input \n\n");
                dup2(get_input_fd(extract_input(tokens)), 0);
                //close (pipe_fd[0]);
            } */
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (i == num_cmds - 1)
            {
                dup2(STDOUT_FILENO, 1);
                close (pipe_fd[0]);
            }
            if (i < num_cmds - 1 && check_redirection_output(tokens))
            {
                fprintf(stderr, "Test Output \n\n");
                close(pipe_fd[0]);
                dup2(get_output_fd(extract_output(tokens)), STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            if (i < num_cmds)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            //fprintf(stderr, "Execute command\n\n");
            t_token *cmd_t = extract_command_n(tokens, i);// Extract the i-th command
            exec(cmd_t, env); // execute the current command
            exit_error();
        }
        wait(0);
        if (prev_fd != -1)
            close(prev_fd);
        if (i < num_cmds)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];  // Keep the read end for the next iteration
        }
    }
    for (i = 0; i <= num_cmds; i++)
        wait(&status);
    return (0);
}
