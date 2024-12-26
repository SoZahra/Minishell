/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:06:09 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/26 16:08:32 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	cmd_clean_and_exit(t_ctx *ctx, t_command *cmd, char **env_v,
		int exit_code)
{
	if (ctx->save_stdin > 2)
		close(ctx->save_stdin);
	if (ctx->save_stdout > 2)
		close(ctx->save_stdout);
	free_args(env_v);
	while (cmd && cmd->prev)
		cmd = cmd->prev;
	free_command_list(cmd);
	cleanup_shell(ctx);
	free_env(ctx->env_vars);
	exit(exit_code);
}

static int	check_parent_dir(const char *file)
{
	char	*parent_dir;
	char	*last_slash;
	int		ret;

	parent_dir = ft_strdup(file);
	if (!parent_dir)
		return (1);
	last_slash = ft_strrchr(parent_dir, '/');
	if (last_slash)
	{
		*last_slash = '\0';
		if (access(parent_dir, W_OK) == -1)
			ret = 1;
		else
			ret = 0;
	}
	else
		ret = 0;
	free(parent_dir);
	if (ret)
		ft_fprintf(2, "MiniBG: %s: Permission denied\n", file);
	return (ret);
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
	if (access(redir->file, F_OK) == 0)
	{
		if (access(redir->file, W_OK) == -1)
		{
			ft_fprintf(2, "MiniBG: %s: Permission denied\n", redir->file);
			return (1);
		}
	}
	else if (check_parent_dir(redir->file))
		return (1);
	if (cmd->out_fd > 0)
		close(cmd->out_fd);
	cmd->out_fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->out_fd == -1)
		return (perror("MiniBG"), 1);
	return (0);
}

static int	check_existing_file(const char *file)
{
	if (access(file, W_OK) == -1)
	{
		ft_fprintf(2, "MiniBG: %s: Permission denied\n", file);
		return (1);
	}
	return (0);
}


static int	open_output_file(t_command *cmd, t_redirection *redir)
{
	cmd->out_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->out_fd == -1)
	{
		ft_fprintf(2, "MiniBG: %s: Cannot create file\n", redir->file);
		return (1);
	}
	return (0);
}

int	open_outt(t_command *cmd, t_redirection *redir)
{
	if (cmd->out_fd > 0)
		close(cmd->out_fd);
	if (access(redir->file, F_OK) == 0)
	{
		if (check_existing_file(redir->file))
			return (1);
	}
	else if (check_parent_dir(redir->file))
		return (1);
	return (open_output_file(cmd, redir));
}

int	here_doc(char *delimiter, t_ctx *ctx)
{
	(void)ctx;
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_fprintf(2, "\nwarning: here-document delimited by end-of-file\n");
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int	handle_heredoc(t_command *cmd, t_redirection *redir)
{
	int	fd;

	fd = here_doc(redir->file, get_ctx());
	if (fd == -1)
		return (1);
	if (cmd->in_fd > 0)
		close(cmd->in_fd);
	cmd->in_fd = fd;
	return (0);
}

int	open_in(t_command *cmd, t_redirection *redir)
{
	if (redir->type == 'H')
		return (handle_heredoc(cmd, redir));
	if (access(redir->file, F_OK))
	{
		get_ctx()->exit_status = 1;
		return (ft_fprintf(2, "MiniBG: %s: No such file or directory\n",
				redir->file),
			1);
	}
	if (access(redir->file, R_OK))
	{
		get_ctx()->exit_status = 1;
		return (ft_fprintf(2, "MiniBG %s: Permission denied\n", redir->file), 1);
	}
	if (cmd->in_fd)
		close(cmd->in_fd);
	cmd->in_fd = open(redir->file, O_RDONLY);
	if (cmd->in_fd == -1)
		return (1);
	return (0);
}

int	create_files_first(t_command *cmd)
{
	t_redirection	*redir;
	t_redirection	*last_out;
	int				tmp_fd;

	redir = cmd->redirs;
	last_out = NULL;
	while (redir)
	{
		if (redir->type == '>' || redir->type == 'A')
			last_out = redir;
		redir = redir->next;
	}
	redir = cmd->redirs;
	while (redir)
	{
		if ((redir->type == '>' || redir->type == 'A') && redir != last_out)
		{
			tmp_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (tmp_fd == -1)
				return (1);
			close(tmp_fd);
		}
		redir = redir->next;
	}
	return (0);
}

int	create_output_file(t_redirection *redir)
{
	int	tmp_fd;

	if (access(redir->file, F_OK) == 0)
	{
		if (access(redir->file, W_OK) == -1)
		{
			ft_fprintf(2, "MiniBG: %s: Permission denied\n", redir->file);
			return (1);
		}
	}
	else if (check_parent_dir(redir->file))
		return (1);
	tmp_fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd == -1)
		return (1);
	close(tmp_fd);
	return (0);
}

static int	handle_redirection(t_command *cmd, t_redirection *redir)
{
	if (redir->type == '>')
		return (open_outt(cmd, redir));
	if (redir->type == 'A')
		return (open_outa(cmd, redir));
	if (redir->type == '<' || redir->type == 'H')
		return (open_in(cmd, redir));
	return (0);
}

int	open_outfiles(t_command *cmd)
{
	t_redirection	*curr;
	int				i;

	if (!cmd || !cmd->redirs)
		return (0);
	curr = cmd->redirs;
	i = 0;
	while (curr[i].type)
	{
		if (handle_redirection(cmd, &curr[i]))
			return (1);
		i++;
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

int	has_heredoc(t_command *cmd)
{
	int	i;

	if (!cmd || !cmd->redirs)
		return (0);
	i = 0;
	while (cmd->redirs[i].type)
	{
		if (cmd->redirs[i].type == 'H')
			return (1);
		i++;
	}
	return (0);
}

int	set_redirs(t_command *cmd)
{
	if (has_heredoc(cmd))
	{
		if (open_outfiles(cmd))
			return (1);
		if (set_fds(cmd))
			return (1);
		if (set_pfd(cmd))
			return (1);
	}
	else
	{
		if (set_pfd(cmd))
			return (1);
		if (open_outfiles(cmd))
			return (1);
		if (set_fds(cmd))
			return (1);
	}
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

void	cleanup_pipes(t_command *cmd)
{
	if (cmd->prev)
	{
		if (cmd->prev->pfd[0] > 2)
			close(cmd->prev->pfd[0]);
		if (cmd->prev->pfd[1] > 2)
			close(cmd->prev->pfd[1]);
	}
	if (cmd->pfd[0] > 2)
		close(cmd->pfd[0]);
	if (cmd->pfd[1] > 2)
		close(cmd->pfd[1]);
}

static int	check_direct_path(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
	{
		ft_fprintf(2, "MiniBG: %s: No such file or directory\n", path);
		return (127);
	}
	if (S_ISDIR(statbuf.st_mode))
	{
		ft_fprintf(2, "MiniBG: %s: Is a directory\n", path);
		return (126);
	}
	if (access(path, X_OK) == -1)
	{
		ft_fprintf(2, "MiniBG: %s: Permission denied\n", path);
		return (126);
	}
	return (0);
}

static int	check_errors(t_command *cmd)
{
	if (!cmd->args[0] || !*(cmd->args[0]))
		return (0);
	if (cmd->args[0][0] == '/'
		|| (cmd->args[0][0] == '.' && cmd->args[0][1] == '/'))
		return (check_direct_path(cmd->args[0]));
	return (0);
}

void	handle_command_not_found(t_ctx *ctx, t_command *cmd, char **env_v)
{
	ft_fprintf(2, "MiniBG: %s: command not found\n", cmd->args[0]);
	cmd_clean_and_exit(ctx, cmd, env_v, 127);
}

void	execute_command(t_ctx *ctx, t_command *cmd, char **env_v)
{
	int	error_code;

	error_code = check_errors(cmd);
	if (error_code)
		cmd_clean_and_exit(ctx, cmd, env_v, error_code);
	if (!cmd->args[0] || !*(cmd->args[0]))
		cmd_clean_and_exit(ctx, cmd, env_v, 0);
	cmd->path = find_command_path(cmd->args[0], ctx);
	if (!cmd->path)
		handle_command_not_found(ctx, cmd, env_v);
	env_v = create_env_array(ctx->env_vars);
	execve(cmd->path, cmd->args, env_v);
	handle_command_not_found(ctx, cmd, env_v);
}

int	child_process(t_ctx *ctx, t_command *cmd)
{
	char	**env_v;

	env_v = NULL;
	if (set_redirs(cmd))
		cmd_clean_and_exit(ctx, cmd, env_v, 1);
	if (is_builtin(cmd->args[0]))
		set_and_exec_builtin(ctx, cmd);
	execute_command(ctx, cmd, env_v);
	return (0);
}

int	exec_child(t_ctx *ctx, t_command *cmd)
{
	if (!cmd)
		return (1);
	if (cmd->next && pipe(cmd->pfd) == -1)
		return (1);
	cmd->pid = fork();
	if (cmd->pid == -1)
		return (1);
	if (!cmd->pid)
		child_process(ctx, cmd);
	if (cmd->pid)
		exec_parent(cmd);
	return (0);
}

int	save_std(t_ctx *ctx)
{
	ctx->save_stdin = dup(STDIN_FILENO);
	if (ctx->save_stdin == -1)
		return (1);
	ctx->save_stdout = dup(STDOUT_FILENO);
	if (ctx->save_stdout == -1)
	{
		close(ctx->save_stdin);
		return (1);
	}
	return (0);
}

int	restore_std(t_ctx *ctx)
{
	int	ret;

	ret = 0;
	if (dup2(ctx->save_stdin, STDIN_FILENO) == -1)
		ret = 1;
	close(ctx->save_stdin);
	if (dup2(ctx->save_stdout, STDOUT_FILENO) == -1)
		ret = 1;
	close(ctx->save_stdout);
	return (ret);
}

int	exec_builtin_once(t_ctx *ctx, t_command *cmd)
{
	char	*cmd_line;
	int		ret;

	if (!cmd)
		return (1);
	cmd_line = tokens_to_string_from_command(cmd);
	if (!cmd_line)
		return (1);
	if (save_std(ctx))
		return (free(cmd_line), 1);
	if (set_redirs(cmd))
	{
		restore_std(ctx);
		return (free(cmd_line), 1);
	}
	ctx->exit_status = execute_builtin(cmd_line, ctx);
	if (ft_strncmp(cmd->args[0], "exit", 5))
		free(cmd_line);
	ret = restore_std(ctx);
	// free(cmd_line);
	return (ret);
}

void	wait_loop(t_ctx *ctx, t_command *cmd)
{
	t_command	*tmp;
	int			status;

	tmp = cmd;
	status = 0;
	while (tmp)
	{
		if (tmp->pid > 0)
		{
			waitpid(tmp->pid, &status, 0);
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				status = WTERMSIG(status) + 128;
			set_term_attr();
			ctx->exit_status = status;
		}
		tmp = tmp->next;
	}
}

int	exec_loop(t_ctx *ctx, t_command *cmd)
{
	t_command	*tmp;
	int			has_child;
	int			ret;

	tmp = cmd;
	has_child = ((ret = 0));
	ctx->current_command = cmd;
	while (tmp)
	{
		if (!tmp->prev && !tmp->next && is_builtin(tmp->args[0]))
		{
			if (exec_builtin_once(ctx, tmp))
				ret = 1;
		}
		else
		{
			if (exec_child(ctx, tmp))
				ret = 1;
			has_child = 1;
		}
		tmp = tmp->next;
	}
	if (has_child)
		wait_loop(ctx, cmd);
	return (ret);
}
