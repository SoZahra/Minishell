#include "../../include/minishell.h"

void	cmd_clean_and_exit(t_ctx *ctx, t_command *cmd, char **env_v,
		int exit_code)
{
	free_args(env_v);
	free_command(cmd);
	cleanup_shell(ctx);
	exit(exit_code);
}

int	set_pfd(t_command *cmd)
{
	if (cmd->prev)
	{
		close(cmd->prev->pfd[1]);
		if (dup2(cmd->prev->pfd[0], STDIN_FILENO) == -1)
			return (1);
		close(cmd->prev->pfd[0]);
	}
	if (cmd->next)
	{
		close(cmd->pfd[0]);
		if (dup2(cmd->pfd[1], STDOUT_FILENO) == -1)
			return (1);
		close(cmd->pfd[1]);
	}
	return (0);
}

int	open_outa(t_command *cmd, t_redirection *redir)
{
	if (access(redir->file, W_OK) == -1)
	{
		ft_fprintf(2, "MiniBG: %s: Permission denied\n", redir->file);
		return (1);
	}
	if (cmd->out_fd > 0)
		close(cmd->out_fd);
	cmd->out_fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->out_fd == -1)
		return (perror("MiniBG"), 1);
	return (0);
}

int	open_outt(t_command *cmd, t_redirection *redir)
{
	if (access(redir->file, W_OK) == -1)
	{
		ft_fprintf(2, "MiniBG: %s: Permission denied\n", redir->file);
		return (1);
	}
	if (cmd->out_fd > 0)
		close(cmd->out_fd);
	cmd->out_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->out_fd == -1)
		return (perror("MiniBG"), 1);
	return (0);
}

int	open_in(t_command *cmd, t_redirection *redir)
{
	if (access(redir->file, F_OK))
	{
		get_ctx()->exit_status = 1;
		return (printf("bash: %s: No such file or directoryyy\n", redir->file),
			1);
	}
	if (access(redir->file, R_OK))
	{
		get_ctx()->exit_status = 1;
		return (printf("bash: %s: Permission denied\n", redir->file), 1);
	}
	if (cmd->in_fd)
		close(cmd->in_fd);
	cmd->in_fd = open(redir->file, O_RDONLY);
	if (cmd->in_fd == -1)
		return (1);
	return (0);
}

int	open_outfiles(t_command *cmd)
{
	t_redirection	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == 'A' && open_outa(cmd, redir))
			return (1);
		else if (redir->type == '>' && open_outt(cmd, redir))
			return (1);
		else if ((redir->type == '<' || redir->type == 'H') && open_in(cmd,
				redir))
			return (1);
		redir = redir->next;
	}
	return (0);
}

int	set_fds(t_command *cmd)
{
	if (cmd->in_fd)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			return (1);
		close(cmd->in_fd);
	}
	if (cmd->out_fd)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			return (1);
		close(cmd->out_fd);
	}
	return (0);
}

int	set_redirs(t_command *cmd)
{
	if (set_pfd(cmd))
		return (1);
	if (open_outfiles(cmd))
		return (1);
	if (set_fds(cmd))
		return (1);
	return (0);
}

int	set_and_exec_builtin(t_ctx *ctx, t_command *cmd)
{
	char	*cmd_line;

	cmd_line = tokens_to_string_from_command(cmd);
	if (!cmd_line)
		return (1);
	ctx->exit_status = execute_builtin(cmd_line, ctx);
	free(cmd_line);
	cmd_clean_and_exit(ctx, cmd, NULL, ctx->exit_status);
	return (0);
}

void	exec_parent(t_command *cmd)
{
	if (cmd->prev)
	{
		close(cmd->prev->pfd[0]);
		close(cmd->prev->pfd[1]);
	}
}

int	exec_child(t_ctx *ctx, t_command *cmd)
{
	char	**env_v;

	env_v = NULL;
	if (cmd->next)
		pipe(cmd->pfd);
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (1);
	if (!cmd->pid)
	{
		if (set_redirs(cmd))
			return (1);
		if (is_builtin(cmd->args[0]))
			set_and_exec_builtin(ctx, cmd);
		cmd->path = find_command_path(cmd->args[0], ctx);
		if (!cmd->path)
		{
			cmd->path = ft_strdup(cmd->args[0]);
			if (!cmd->path)
        		cmd_clean_and_exit(ctx, cmd, env_v, 127);
		}
		env_v = create_env_array(ctx->env_vars);
		execve(cmd->path, cmd->args, env_v);
		ft_fprintf(2, "bash: %s: command not found\n", cmd->args[0]);
		cmd_clean_and_exit(ctx, cmd, env_v, 127);
	}
	if (cmd->pid)
		exec_parent(cmd);
	return (0);
}

int save_std(t_ctx *ctx)
{
	ctx->save_stdin = dup(STDIN_FILENO);
	if (ctx->save_stdin == -1)
		return (1);
	ctx->save_stdout = dup(STDOUT_FILENO);
	if (ctx->save_stdout == -1)
		return (1);
	return (0);
}

int restore_std(t_ctx *ctx)
{
	if (dup2(ctx->save_stdin, STDIN_FILENO) == -1)
		return (1);
	close(ctx->save_stdin);
	if (dup2(ctx->save_stdout, STDOUT_FILENO) == -1)
		return (1);
	close(ctx->save_stdout);
	return (0);
}

int	exec_builtin_once(t_ctx *ctx, t_command *cmd)
{
	char	*cmd_line;

	if (!cmd)
		return (1);
	cmd_line = tokens_to_string_from_command(cmd);
	if (!cmd_line)
		return (1);
	if (save_std(ctx))
		return (1);
	if (set_redirs(cmd))
		return (1);
	ctx->exit_status = execute_builtin(cmd_line, ctx);
	if (restore_std(ctx))
		return (1);
	free(cmd_line);
	// free_command(cmd);
	return (0);
}

void wait_loop(t_ctx *ctx, t_command *cmd)
{
    t_command *tmp;
	int status;

	tmp = cmd;
	status = 0;
    while (tmp)
    {
        waitpid(tmp->pid, &status, 0);
		set_term_attr();
        if (WIFEXITED(status)) {
            status = WEXITSTATUS(status);
        }
        if (WIFSIGNALED(status)) {
            status = WTERMSIG(status) + 128;
        }
		ctx->exit_status = status;
        tmp = tmp->next;
    }
}

int	exec_loop(t_ctx *ctx, t_command *cmd)
{
	t_command	*tmp;
	int has_child = 0;

	tmp = cmd;
	while (tmp)
	{
		printf("n_args: %d\n", tmp->arg_count);
		if (!tmp->prev && !tmp->next && is_builtin(tmp->args[0]))
		{
			if (exec_builtin_once(ctx, tmp))
				return (1);
		}
		else
		{
			if(exec_child(ctx, tmp))
				return(1);
			has_child = 1;
		}
		tmp = tmp->next;
	}
	if(has_child)
		wait_loop(ctx, cmd);
	return (0);
}
