/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:07:23 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/22 15:58:59 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void print_envir(char **env)
// {
// 	int i = 0;
// 	while (env && env[i])
// 	{
// 		printf("%s\n", env[i]);
// 		i++;
// 	}
// }

// int	exec(t_token *cmd_tokens, t_ctx *ctx)
// {
// 	char	**option_cmd;
// 	char	*path;
//     char    **env;

// 	option_cmd = prepare_args(cmd_tokens, ctx);
// 	if (!option_cmd[0])
// 	{
// 		fprintf(stderr, "Error: Command is empty\n");
// 		free_tab(option_cmd);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (strcmp(option_cmd[0], "cd") == 0)
// 	{
// 		ft_cd(option_cmd, ctx);
// 		free_tab(option_cmd);
// 		return (0);
// 	}
//     env = ctx_to_env_array(ctx);
//     //print_envir(env);
// 	path = get_path(option_cmd[0], env);
// 	if (execve(path, option_cmd, env) == -1)
// 	{
//         ctx->exit_status = 127;
// 		perror("exec command");
// 		free_tab(option_cmd);
//         free_tab(env);
//         ctx->exit_status = 127;
// 		exit(EXIT_FAILURE);
// 	}
//     ctx->exit_status = 127;
// 	free_tab(option_cmd);
// 	return (0);
// }

// int exec(t_token *cmd_tokens, t_ctx *ctx)
// {
//     char **option_cmd;
//     char *path;
//     char **env;

//     option_cmd = prepare_args(cmd_tokens, ctx);
//     if (!option_cmd || !option_cmd[0])
//     {
//         fprintf(stderr, "Error: Command is empty\n");
//         free_tab(option_cmd);
//         exit(EXIT_FAILURE);
//     }
//     if (is_builtin(option_cmd[0]))
//     {
//         int ret = execute_builtin(ctx, cmd_tokens);
//         free_tab(option_cmd);
//         exit(ret);
//     }
//     env = ctx_to_env_array(ctx);
//     // fprintf(stderr, "it went in\n");
//     path = get_path(option_cmd[0], env);
//     if (!path)
//         path = option_cmd[0];
//     execve(path, option_cmd, env);
//     perror("exec command");
//     free_tokens(cmd_tokens);
//     free_tab(option_cmd);
//     free_tab(env);
//     exit(127);
//     // free(path);
//     // exit(EXIT_FAILURE);
//     // if (execve(path, option_cmd, env) == -1)
//     // {
//     // }
//     // if (!path)
//     // {
//         // fprintf(stderr, "minishell: %s: command not found\n", option_cmd[0]);
//         // free_tab(option_cmd);
//         // free_tab(env);
//         // env = NULL;
//         // exit(127);
//     // }

//     // free_tab(option_cmd);
//     // free(path);
//     // path = NULL;
//     // free_tab(env);
//     // env = NULL;
//     return (0);
// }

// void    read_heredoc(int fd, char *limiter)
// {
// 	char	*buf;

// 	while(1)
// 	{
// 		buf = NULL;
// 		buf = readline("> ");
// 		if (!buf)
// 		{
// 			// exit_error();
// 			break ;
// 		}
// 		if (!ft_strncmp(limiter, buf, INT_MAX))
// 			break ;
// 		write(fd, buf, ft_strlen(buf));
// 		write(fd, "\n", 1);
// 		free(buf);
// 	}
// 	free(buf);
// 	close(fd);
// }

// int here_doc(char *limiter)
// {
//     int fd;

//     fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
//     if (fd < 0)
//         exit_error();
//     read_heredoc(fd, limiter);
//     fd = open(".heredoc.tmp", O_RDONLY);
//     if (fd >= 0)
//         unlink(".heredoc.tmp");
//     return fd;
// }

// void handle_input_redirection(t_token *redir_token, int *redirect, int *redirect_input)
// {
//     int input_fd;

//     if (ft_strcmp(redir_token->value, "<<") == 0)
//         input_fd = here_doc(redir_token->next->value);
//     else
//         input_fd = open(redir_token->next->value, O_RDONLY);
//     //input_fd = open(redir_token->next->value, O_RDONLY);
//     if (input_fd == -1)
//         exit_error();
//     dup2(input_fd, STDIN_FILENO);
//     close(input_fd);
//     *redirect = 1;
//     *redirect_input = 1;
// }

void handle_output_redirection(t_token *redir_token, int *redirect, int *redirect_output)
{
    int output_fd;

    if (ft_strcmp(redir_token->value, ">>") == 0)
        output_fd = open(redir_token->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        output_fd = open(redir_token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("Redirection error");
        exit(EXIT_FAILURE);
    }
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);
    *redirect = 1;
    *redirect_output = 1;
}

// void collect_exec_tokens(t_token *cmd_start, t_token *cmd_end, t_token **exec_tokens, int *redirect, int *redirect_input, int *redirect_output)
// {
//     t_token **exec_tokens_tail = exec_tokens;
//     t_token *redir_token = cmd_start;

//     while (redir_token != cmd_end)
// 	{
//         if (ft_strcmp(redir_token->value, "<") == 0 || ft_strcmp(redir_token->value, "<<") == 0)
// 		{
//             handle_input_redirection(redir_token, redirect, redirect_input);
//             redir_token = redir_token->next;
//         }
// 		else if (ft_strcmp(redir_token->value, ">") == 0
//         || ft_strcmp(redir_token->value, ">>") == 0)
// 		{
//             handle_output_redirection(redir_token, redirect, redirect_output);
//             redir_token = redir_token->next;
//         }
//         else
// 		{
//             *exec_tokens_tail = redir_token;
//             exec_tokens_tail = &(*exec_tokens_tail)->next;
//         }
//         redir_token = redir_token->next;
//     }
//     *exec_tokens_tail = NULL;
// }

// void setup_pipe_for_child(int prev_fd, int *pipe_fd, int redirect_input, int redirect_output, t_token *cmd_end)
// {
//     if (prev_fd != -1 && redirect_input != 1)
// 	{
//         dup2(prev_fd, STDIN_FILENO);
//         close(prev_fd);
//     }
//     if (cmd_end != NULL && ft_strcmp(cmd_end->value, "|") == 0 && redirect_output != 1)
// 	{
//         close(pipe_fd[0]);
//         dup2(pipe_fd[1], STDOUT_FILENO);
//         close(pipe_fd[1]);
//     }
// }

// void initialize_pipe_if_needed(int *pipe_fd, t_token *cmd_end)
// {
//     if (cmd_end != NULL && ft_strcmp(cmd_end->value, "|") == 0) {
//         if (pipe(pipe_fd) == -1)
//             exit_error();
//     }
// }

// void execute_command_in_child(t_token *cmd_start, t_token *cmd_end, int prev_fd, int *pipe_fd, t_ctx *ctx)
// {
//     int     redirect;
//     int     redirect_output;
//     int     redirect_input;
//     pid_t   pid;

//     pid = fork();
//     if (pid == -1)
//         exit_error();
//     if (pid == 0)
//     {
//         t_token *exec_tokens = NULL;
//         redirect = 0;
//         redirect_output = 0;
//         redirect_input = 0;

//         collect_exec_tokens(cmd_start, cmd_end, &exec_tokens, &redirect, &redirect_input, &redirect_output);
//         // printf("exec token value : %s\n", exec_tokens->value);
//         if (exec_tokens && is_builtin(exec_tokens->value))
//         {
//             if (prev_fd != -1) {
//                 dup2(prev_fd, STDIN_FILENO);
//                 close(prev_fd);
//             }
//             if (cmd_end != NULL && ft_strcmp(cmd_end->value, "|") == 0)
//             {
//                 close(pipe_fd[0]);
//                 dup2(pipe_fd[1], STDOUT_FILENO);
//                 close(pipe_fd[1]);
//             }
//             // ctx->exit_status = exec_simple_cmd(exec_tokens, ctx);
//             // exit(ctx->exit_status);
//             int status = exec_simple_cmd(exec_tokens, ctx);
//             exit(status);
//         }
//         setup_pipe_for_child(prev_fd, pipe_fd, redirect_input, redirect_output, cmd_end);
//         exec(exec_tokens, ctx);
//         exit_error();
//     }
// }

void cleanup_parent_resources(int *prev_fd, int *pipe_fd, t_token **cmd_start, t_token *cmd_end)
{
    if (*prev_fd != -1)
        close(*prev_fd);
    if (cmd_end != NULL)
	{
        close(pipe_fd[1]);
        *prev_fd = pipe_fd[0];
        *cmd_start = cmd_end->next;
    }
    else
        *cmd_start = NULL;
}

// void wait_for_all_children()
// {
//     int status;
//     while (wait(&status) > 0);
// }


int wait_for_all_children(int status, t_token *tokens, t_ctx *ctx)
{
    int exit_status;

    exit_status = (status >> 8) & 0xFF;
    if ((status & 0x7F) != 0)
        return ctx->exit_status;
    if (tokens && ft_strcmp(tokens->value, "cd") == 0)
    {
        t_token *next = tokens->next;
        t_token *next_next = NULL;

        if (next)
            next_next = next->next;
        if (next && next_next)
            ctx->exit_status = 1;
        else
            ctx->exit_status = exit_status;
    }
    else if (tokens && ft_strcmp(tokens->value, "exit") == 0)
        ctx->exit_status = exit_status;
    return ctx->exit_status;
}

// int process_pline(t_token *tokens, t_ctx *ctx)
// {
//     int     pipe_fd[2];
//     int     prev_fd;
//     t_token *cmd_start;
//     int status;

//     prev_fd = -1;
//     cmd_start = tokens;

//     if (!tokens && ctx->exit_status == 1)
//         return 1;
//     if (tokens && ft_strcmp(tokens->value, "exit") == 0)
//     {
//         execute_command_in_child(tokens, NULL, prev_fd, pipe_fd, ctx);
//         while (wait(&status) > 0)
//             wait_for_all_children(status, tokens, ctx);
//         // write(1, "exit\n", 5);
//         exit(ctx->exit_status);  // Quitte le processus parent
//     }
//     while (cmd_start)
// 	{
//         t_token *cmd_end = cmd_start;
//         while (cmd_end && ft_strcmp(cmd_end->value, "|") != 0)
//             cmd_end = cmd_end->next;
//         initialize_pipe_if_needed(pipe_fd, cmd_end);
//         execute_command_in_child(cmd_start, cmd_end, prev_fd, pipe_fd, ctx);
//         cleanup_parent_resources(&prev_fd, pipe_fd, &cmd_start, cmd_end);
//     }
//     // wait_for_all_children();
//     while (wait(&status) > 0)
//         wait_for_all_children(status, tokens, ctx);
//     set_term_attr(); //c'est pour remettre les settings par defauts, si quelqu'un essaie de kill vim par exemple
//     //  a mettre a pres chaque wait
//     return (ctx->exit_status);
// }