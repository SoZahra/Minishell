/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:02:24 by llarrey           #+#    #+#             */
/*   Updated: 2024/12/15 17:42:24 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*resolve_command(char **args, t_ctx *ctx)
{
	char	**env;
	char	*path;

	env = ctx_to_env_array(ctx);
	path = get_path(args[0], env);
	free_tab(env);
	return (path);
}

void	execute_command(char **args, char *path, t_ctx *ctx)
{
	char	**env;

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

int	exec(t_token *cmd_tokens, t_ctx *ctx)
{
	char	**args;
	char	*path;

	args = prepare_args(cmd_tokens, ctx);
	if (!args[0])
	{
		free_tab(args);
		exit(EXIT_FAILURE);
	}
	path = resolve_command(args, ctx);
	execute_command(args, path, ctx);
	free_tab(args);
	return (0);
}

void	collect_exec_tokens(t_token *cmd_start, t_token *cmd_end,
		t_token **exec_tokens, t_redir **redir)
{
	t_token	*redir_token;

	redir_token = cmd_start;
	while (redir_token != cmd_end)
	{
		if (ft_strcmp(redir_token->value, "<") == 0
			|| ft_strcmp(redir_token->value, "<<") == 0)
		{
			handle_input_redirection(redir_token, *redir);
			redir_token = redir_token->next;
		}
		else if (ft_strcmp(redir_token->value, ">") == 0
			|| ft_strcmp(redir_token->value, ">>") == 0)
		{
			handle_output_redirection(redir_token, *redir);
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

void	prepare_child_execution(t_pipeline *pl, t_ctx *ctx)
{
	t_token	*exec_tokens;
	t_redir	*redir;

	exec_tokens = NULL;
	redir = malloc(sizeof(t_redir));
	if (!redir)
		exit(EXIT_FAILURE);
	redir->input = 0;
	redir->output = 0;
	collect_exec_tokens(pl->cmd_start, pl->cmd_end, &exec_tokens, &redir);
	if (exec_tokens && is_builtin(exec_tokens->value))
	{
		if (ft_strcmp(exec_tokens->value, "export") == 0
			||ft_strcmp(exec_tokens->value, "unset") == 0)
			return (free(redir));
		setup_redirects(pl->prev_fd, pl->pipe_fd, pl->cmd_end, redir);
		free(redir);
		execute_builtin(pl->cmd_line, ctx);
		exit(EXIT_SUCCESS);
	}
	setup_redirects(pl->prev_fd, pl->pipe_fd, pl->cmd_end, redir);
	free(redir);
	exec(exec_tokens, ctx);
	exit(EXIT_FAILURE);
}
