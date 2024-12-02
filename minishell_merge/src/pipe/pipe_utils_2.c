/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:07:23 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 17:27:12 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

/* int process_pline(t_token *tokens, char **env)
{
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t pid;
    int status;
    t_token *cmd_start = tokens;
    int redirect = 0;
    int redirect_output = 0;
    int redirect_input = 0;

    while (cmd_start != NULL)
    {
        t_token *cmd_end = cmd_start;
        while (cmd_end != NULL && cmd_end->type != TOKEN_PIPE)
            cmd_end = cmd_end->next;

        if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE)
        {
            if (pipe(pipe_fd) == -1)
                exit_error();
        }
        pid = fork();
        if (pid == -1)
            exit_error();
        if (pid == 0)
        {
            t_token *exec_tokens = NULL;
            t_token **exec_tokens_tail = &exec_tokens;
            t_token *redir_token = cmd_start;

            while (redir_token != cmd_end)
            {
                if (redir_token->type == TOKEN_REDIRECT_INPUT)
                {
                    int input_fd = open(redir_token->next->value, O_RDONLY);
                    if (input_fd == -1)
                        exit_error();
                    dup2(input_fd, STDIN_FILENO);
                    close(input_fd);
                    redir_token = redir_token->next;
                    redirect = 1;
                    redirect_input = 1;
                }
                else if (redir_token->type == TOKEN_REDIRECT_OUTPUT)
                {
                    fprintf(stderr, "Output redirection \n");
                    int output_fd = open(redir_token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (output_fd == -1)
                        exit_error();
                    dup2(output_fd, STDOUT_FILENO);
                    close(output_fd);
                    redir_token = redir_token->next;
                    redirect_output = 1;
                    redirect = 1;
                }
                else
                {
                    *exec_tokens_tail = redir_token;
                    exec_tokens_tail = &(*exec_tokens_tail)->next;
                    redirect = 0;
                }
                fprintf(stderr, "Did I leave ? \n");
                redir_token = redir_token->next;
            }
            *exec_tokens_tail = NULL;
            if (prev_fd != -1 && redirect_input != 1)
            {
                fprintf(stderr, "First if \n");
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
                redirect_input = 0;
            }
            if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE && redirect_output != 1)
            {
                fprintf(stderr, "Second if \n");
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE && redirect != 1)
            {
                fprintf(stderr, "Second / half if \n");
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            else if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE && redirect_output == 1)
            {
                fprintf(stderr, "Third if \n");
            	close(pipe_fd[0]);
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
                //dup2(pipe_fd[1], STDOUT_FILENO);
                //close(pipe_fd[1]);
            }
            fprintf(stderr, "Check on tokens value : %s \n\n", exec_tokens->value);
            exec(exec_tokens, env);
            exit_error();
        }
        wait(0);
        if (prev_fd != -1)
            close(prev_fd);
        if (cmd_end != NULL)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
            cmd_start = cmd_end->next;
        }
        else
            cmd_start = NULL;
    }
    while (wait(&status) > 0);
    return 0;
}
 */

void    read_heredoc(int fd, char *limiter)
{
	char	*buf;

	while(1)
	{
		buf = NULL;
		buf = readline("> ");
		if (!buf)
		{
			exit_error();
			break ;
		}
		if (!ft_strncmp(limiter, buf, INT_MAX))
			break ;
		write(fd, buf, ft_strlen(buf));
		write(fd, "\n", 1);
		free(buf);
	}
	free(buf);
	close(fd);
}

int here_doc(char *limiter) 
{
    int fd;
    
    fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0)
        exit_error();
    read_heredoc(fd, limiter);
    fd = open(".heredoc.tmp", O_RDONLY);
    if (fd >= 0)
        unlink(".heredoc.tmp");
    return fd;
}

void handle_input_redirection(t_token *redir_token, int *redirect, int *redirect_input) 
{
    int input_fd;
    
    if (redir_token->type == TOKEN_HEREDOC) {
        input_fd = here_doc(redir_token->next->value);
    } else {
        input_fd = open(redir_token->next->value, O_RDONLY);
    }
    //input_fd = open(redir_token->next->value, O_RDONLY);
    if (input_fd == -1)
        exit_error();
    dup2(input_fd, STDIN_FILENO);
    close(input_fd);
    *redirect = 1;
    *redirect_input = 1;
}

void handle_output_redirection(t_token *redir_token, int *redirect, int *redirect_output) 
{
    int output_fd = open(redir_token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1)
        exit_error();
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);
    *redirect = 1;
    *redirect_output = 1;
}

void collect_exec_tokens(t_token *cmd_start, t_token *cmd_end, t_token **exec_tokens, int *redirect, int *redirect_input, int *redirect_output)
{
    t_token **exec_tokens_tail = exec_tokens;
    t_token *redir_token = cmd_start;

    while (redir_token != cmd_end) 
	{
        if (redir_token->type == TOKEN_REDIRECT_INPUT || redir_token->type == TOKEN_HEREDOC) 
		{
            handle_input_redirection(redir_token, redirect, redirect_input);
            redir_token = redir_token->next;
        } 
		else if (redir_token->type == TOKEN_REDIRECT_OUTPUT) 
		{
            handle_output_redirection(redir_token, redirect, redirect_output);
            redir_token = redir_token->next;
        }
        else 
		{
            *exec_tokens_tail = redir_token;
            exec_tokens_tail = &(*exec_tokens_tail)->next;
        }
        redir_token = redir_token->next;
    }
    *exec_tokens_tail = NULL;
}

void setup_pipe_for_child(int prev_fd, int *pipe_fd, int redirect_input, int redirect_output, t_token *cmd_end)
{
    if (prev_fd != -1 && redirect_input != 1) 
	{
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }
    if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE && redirect_output != 1) 
	{
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
}

void initialize_pipe_if_needed(int *pipe_fd, t_token *cmd_end)
{
    if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE) {
        if (pipe(pipe_fd) == -1)
            exit_error();
    }
}

void execute_command_in_child(t_token *cmd_start, t_token *cmd_end, int prev_fd, int *pipe_fd, char **env)
{
    pid_t pid = fork();
    if (pid == -1)
        exit_error();

    if (pid == 0) {
        t_token *exec_tokens = NULL;
        int redirect = 0, redirect_output = 0, redirect_input = 0;

        collect_exec_tokens(cmd_start, cmd_end, &exec_tokens, &redirect, &redirect_input, &redirect_output);
        setup_pipe_for_child(prev_fd, pipe_fd, redirect_input, redirect_output, cmd_end);
        exec(exec_tokens, env);
        exit_error();
    }
}

void cleanup_parent_resources(int *prev_fd, int *pipe_fd, t_token **cmd_start, t_token *cmd_end)
{
    if (*prev_fd != -1)
        close(*prev_fd);
    if (cmd_end != NULL) 
	{
        close(pipe_fd[1]);
        *prev_fd = pipe_fd[0];
        *cmd_start = cmd_end->next;
    } else 
        *cmd_start = NULL;
}

void wait_for_all_children()
{
    int status;
    while (wait(&status) > 0);
}

int process_pline(t_token *tokens, t_var *myEnv) {
    int pipe_fd[2], prev_fd = -1;
    t_token *cmd_start = tokens;
    
    //signal(SIGTSTP, SIG_DFL);
    while (cmd_start != NULL) 
	{
        t_token *cmd_end = cmd_start;
        while (cmd_end != NULL && cmd_end->type != TOKEN_PIPE)
            cmd_end = cmd_end->next;

        initialize_pipe_if_needed(pipe_fd, cmd_end);
        execute_command_in_child(cmd_start, cmd_end, prev_fd, pipe_fd, myEnv->env);
        //wait(0);  //that was useless...

        cleanup_parent_resources(&prev_fd, pipe_fd, &cmd_start, cmd_end);
    } 
    wait_for_all_children();
    return 0;
}