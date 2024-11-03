/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:07:23 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/03 12:10:00 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

int process_pline(t_token *tokens, char **env)
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
                /* dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]); */
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

