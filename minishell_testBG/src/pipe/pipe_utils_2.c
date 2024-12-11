/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:07:23 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/11 11:30:49 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* void print_envir(char **env)
{
	int i = 0;
	while (env && env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

int	exec(t_token *cmd_tokens, t_ctx *ctx)
{
	char	**option_cmd;
	char	*path;
    char    **env;

	option_cmd = prepare_args(cmd_tokens, ctx);
	if (!option_cmd[0])
	{
		fprintf(stderr, "Error: Command is empty\n");
		free_tab(option_cmd);
		exit(EXIT_FAILURE);
	}
	if (strcmp(option_cmd[0], "cd") == 0)
	{
		ft_cd(option_cmd, ctx);
		free_tab(option_cmd);
		return (0);
	}
    env = ctx_to_env_array(ctx);
    //print_envir(env);
	path = get_path(option_cmd[0], env);
	if (execve(path, option_cmd, env) == -1)
	{
        ctx->exit_status = 127;
		perror("exec command");
		free_tab(option_cmd);
        free_tab(env);
        ctx->exit_status = 127;
		exit(EXIT_FAILURE);
	}
    ctx->exit_status = 127;
	free_tab(option_cmd);
	return (0);
}

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
    
    if (ft_strcmp(redir_token->value, "<<") == 0) 
        input_fd = here_doc(redir_token->next->value);
    else
        input_fd = open(redir_token->next->value, O_RDONLY);
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

void collect_exec_tokens(t_token *cmd_start, t_token *cmd_end, t_token **exec_tokens, int *redirect, int *redirect_input, int *redirect_output)
{
    t_token **exec_tokens_tail = exec_tokens;
    t_token *redir_token = cmd_start;

    while (redir_token != cmd_end) 
	{
        if (ft_strcmp(redir_token->value, "<") == 0 || ft_strcmp(redir_token->value, "<<") == 0) 
		{
            handle_input_redirection(redir_token, redirect, redirect_input);
            redir_token = redir_token->next;
        } 
		else if (ft_strcmp(redir_token->value, ">") == 0
        || ft_strcmp(redir_token->value, ">>") == 0) 
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
    if (cmd_end != NULL && ft_strcmp(cmd_end->value, "|") == 0 && redirect_output != 1) 
	{
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
}

void initialize_pipe_if_needed(int *pipe_fd, t_token *cmd_end)
{
    if (cmd_end != NULL && ft_strcmp(cmd_end->value, "|") == 0) {
        if (pipe(pipe_fd) == -1)
            exit_error();
    }
}

void execute_command_in_child(t_token *cmd_start, t_token *cmd_end, int prev_fd, int *pipe_fd, t_ctx *ctx)
{
    int     redirect;
    int     redirect_output;
    int     redirect_input;
    pid_t   pid;
    
    pid = fork();
    if (pid == -1)
        exit_error();
    if (pid == 0) 
    {
        t_token *exec_tokens = NULL;
        redirect = 0;
        redirect_output = 0; 
        redirect_input = 0;

        collect_exec_tokens(cmd_start, cmd_end, &exec_tokens, &redirect, &redirect_input, &redirect_output);
        printf("exec token value : %s\n", exec_tokens->value);
        if (exec_tokens && is_builtin(exec_tokens->value)) 
        {
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (cmd_end != NULL && ft_strcmp(cmd_end->value, "|") == 0) 
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            //char **args = prepare_args(exec_tokens, ctx);
            printf("test exec : %s\n", exec_tokens->value);
            exec_simple_cmd(exec_tokens, ctx);
            exit(EXIT_SUCCESS);
        }
        setup_pipe_for_child(prev_fd, pipe_fd, redirect_input, redirect_output, cmd_end);
        exec(exec_tokens, ctx);
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
    } 
    else 
        *cmd_start = NULL;
}

void wait_for_all_children()
{
    int status;
    while (wait(&status) > 0);
}

int process_pline(t_token *tokens, t_ctx *ctx) 
{
    int     pipe_fd[2]; 
    int     prev_fd;
    t_token *cmd_start;
    
    prev_fd = -1;
    cmd_start = tokens;
    
    while (cmd_start) 
	{
        t_token *cmd_end = cmd_start;
        while (cmd_end && ft_strcmp(cmd_end->value, "|") != 0)
            cmd_end = cmd_end->next;
        initialize_pipe_if_needed(pipe_fd, cmd_end);
        execute_command_in_child(cmd_start, cmd_end, prev_fd, pipe_fd, ctx);
        cleanup_parent_resources(&prev_fd, pipe_fd, &cmd_start, cmd_end);
    } 
    wait_for_all_children();
    return 0;
} */

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

void handle_input_redirection(t_token *redir_token)
{
    int input_fd;
    if (ft_strcmp(redir_token->value, "<<") == 0) 
        input_fd = here_doc(redir_token->next->value);
    else
        input_fd = open(redir_token->next->value, O_RDONLY);
    if (input_fd == -1)
        exit_error();
    dup2(input_fd, STDIN_FILENO);
    close(input_fd);
}

void handle_output_redirection(t_token *redir_token)
{
    int output_fd;
    if (ft_strcmp(redir_token->value, ">>") == 0)
        output_fd = open(redir_token->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else 
        output_fd = open(redir_token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1)
        exit_error();
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);
}

void setup_pipe(t_pipeline *pl) 
{
    if (pl->cmd_end && ft_strcmp(pl->cmd_end->value, "|") == 0) 
    {
        if (pipe(pl->pipe_fd) == -1)
            exit_error();
    }
}

void setup_redirects(int prev_fd, int *pipe_fd, t_token *cmd_end) 
{
    if (prev_fd != -1) 
    {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }
    if (cmd_end && ft_strcmp(cmd_end->value, "|") == 0) 
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
}

void close_pipe(t_pipeline *pl) 
{
    if (pl->prev_fd != -1)
        close(pl->prev_fd);
    if (pl->cmd_end) 
    {
        close(pl->pipe_fd[1]);
        pl->prev_fd = pl->pipe_fd[0];
    }
}

void wait_for_children() 
{
    int status;
    while (wait(&status) > 0);
}

char *resolve_command(char **args, t_ctx *ctx) 
{
    char **env;
    char *path;

    env = ctx_to_env_array(ctx);
    path = get_path(args[0], env);
    free_tab(env);
    return path;
}

void execute_command(char **args, char *path, t_ctx *ctx) 
{
    char **env;

    env = ctx_to_env_array(ctx);
    if (execve(path, args, env) == -1) 
    {
        ctx->exit_status = 127;
        perror("exec command");
        free_tab(args);
        free_tab(env);
        exit(EXIT_FAILURE);
    }
    free_tab(env);
}

int exec(t_token *cmd_tokens, t_ctx *ctx) {
    char **args;
    char *path;

    args = prepare_args(cmd_tokens, ctx);
    if (!args[0]) 
    {
        free_tab(args);
        exit(EXIT_FAILURE);
    }
    if (is_builtin(args[0])) 
    {
        exec_simple_cmd(cmd_tokens, ctx);
        free_tab(args);
        return 0;
    }
    path = resolve_command(args, ctx);
    execute_command(args, path, ctx);
    free_tab(args);
    return 0;
}

void collect_exec_tokens(t_token *cmd_start, t_token *cmd_end, t_token **exec_tokens) 
{
    t_token *redir_token = cmd_start;
    while (redir_token != cmd_end) 
    {
        if (ft_strcmp(redir_token->value, "<") == 0 || ft_strcmp(redir_token->value, "<<") == 0)
        {
            handle_input_redirection(redir_token);
            redir_token = redir_token->next;
        }
        else if (ft_strcmp(redir_token->value, ">") == 0 || ft_strcmp(redir_token->value, ">>") == 0)
        {
            handle_output_redirection(redir_token);
            redir_token = redir_token->next;
        }
        else
        {
            *exec_tokens = redir_token;
            exec_tokens = &(*exec_tokens)->next;
        }
        redir_token = redir_token->next;
    }
    *exec_tokens = NULL;
}

void execute_in_child(t_pipeline *pl, t_ctx *ctx) {
    t_token *exec_tokens;
    int     redirect;
    int     redirect_input;
    int     redirect_output;
    pid_t   pid;

    pid = fork();
    if (pid == -1)
        exit_error();
    if (pid == 0)
    {
        redirect = 0;
        redirect_input = 0;
        redirect_output = 0;
        exec_tokens = NULL;

        collect_exec_tokens(pl->cmd_start, pl->cmd_end, &exec_tokens);
        if (exec_tokens && is_builtin(exec_tokens->value))
        {
            setup_redirects(pl->prev_fd, pl->pipe_fd, pl->cmd_end);
            exec_simple_cmd(exec_tokens, ctx);
            exit(EXIT_SUCCESS);
        }
        setup_redirects(pl->prev_fd, pl->pipe_fd, pl->cmd_end);
        exec(exec_tokens, ctx);
        exit_error();
    }
}

void process_single_builtin(t_pipeline *pl, t_ctx *ctx)
{
    if (is_builtin(pl->cmd_start->value))
        exec_simple_cmd(pl->cmd_start, ctx);
}

void process_pipeline_segment(t_pipeline *pl, t_ctx *ctx)
{
    setup_pipe(pl);
    execute_in_child(pl, ctx);
    close_pipe(pl);
}

int process_pline(t_token *tokens, t_ctx *ctx)
{
    t_pipeline pl;

    pl.prev_fd = -1;
    pl.cmd_start = tokens;
    while (pl.cmd_start)
    {
        pl.cmd_end = pl.cmd_start;
        while (pl.cmd_end && ft_strcmp(pl.cmd_end->value, "|") != 0)
            pl.cmd_end = pl.cmd_end->next;
        if (is_builtin(pl.cmd_start->value) && (!pl.cmd_end || pl.cmd_end->value[0] != '|'))
        {
            process_single_builtin(&pl, ctx);
            if (pl.cmd_end)
                pl.cmd_start = pl.cmd_end->next;
            else
            pl.cmd_start = NULL;
        }
        else
        {
            process_pipeline_segment(&pl, ctx);
            if (pl.cmd_end)
                pl.cmd_start = pl.cmd_end->next;
            else
                pl.cmd_start = NULL;
        }
    }
    wait_for_children();
    return 0;
}