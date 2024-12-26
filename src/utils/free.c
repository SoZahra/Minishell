/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:32:55 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/26 16:36:28 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_env_copy(char **env_copy)
{
	int	i;

	if (!env_copy)
		return ;
	i = 0;
	while (env_copy[i])
	{
		free(env_copy[i]);
		i++;
	}
	free(env_copy);
}

void	free_env(t_env_var *env_var)
{
	t_env_var	*current;
	t_env_var	*next;

	if (!env_var)
		return ;
	current = env_var;
	while (current)
	{
		next = current->next;
		if (current->name)
		{
			free(current->name);
			current->name = NULL;
		}
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = NULL;
		current = next;
	}
}

void	free_ctx(t_ctx *ctx)
{
	t_token	*current;
	t_token	*next;

	if (!ctx)
		return ;
	if (ctx->env_vars)
	{
		free_env(ctx->env_vars);
		ctx->env_vars = NULL;
	}
	if (ctx->tokens)
	{
		current = ctx->tokens;
		while (current)
		{
			next = current->next;
			if (current->value)
			{
				free(current->value);
				current->value = NULL;
			}
			current->content = NULL;
			free(current);
			current = next;
		}
		ctx->tokens = NULL;
	}
	if (ctx->oldpwd)
	{
		free(ctx->oldpwd);
		ctx->oldpwd = NULL;
	}
	if (ctx->pwd)
	{
		free(ctx->pwd);
		ctx->pwd = NULL;
	}
	if (ctx->save_stdin != STDIN_FILENO)
		close(ctx->save_stdin);
	if (ctx->save_stdout != STDOUT_FILENO)
		close(ctx->save_stdout);
}

void	*free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	return (NULL);
}

void	free_command_list(t_command *cmd)
{
	t_command	*current;
	t_command	*next;

	current = cmd;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}

void	free_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (i < cmd->arg_count)
		{
			free(cmd->args[i]);
			cmd->args[i++] = NULL;
		}
		free(cmd->args);
		cmd->args = NULL;
	}
	if (cmd->path)
	{
		free(cmd->path);
		cmd->path = NULL;
	}
	if (cmd->had_spaces)
	{
		free(cmd->had_spaces);
		cmd->had_spaces = NULL;
	}
	if (cmd->redirs)
	{
		i = 0;
		while (cmd->redirs[i].type)
		{
			if (cmd->redirs[i].file)
				free(cmd->redirs[i].file);
			cmd->redirs[i++].file = NULL;
		}
		free(cmd->redirs);
		cmd->redirs = NULL;
	}
	if (cmd->in_fd > 2)
		close(cmd->in_fd);
	if (cmd->out_fd > 2)
		close(cmd->out_fd);
	if (cmd->pfd[0] > 2)
		close(cmd->pfd[0]);
	if (cmd->pfd[1] > 2)
		close(cmd->pfd[1]);
	free(cmd);
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens)
		return ;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		if (current->content && current->content != current->value)
		{
			free(current->content);
			current->content = NULL;
		}
		free(current);
		current = next;
	}
}

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
}

void	free_pipeline_pipes(int **pipes, int num_commands)
{
	int	i;

	i = 0;
	if (!pipes || num_commands <= 1)
		return ;
	while (i < num_commands - 1)
	{
		if (pipes[i])
		{
			free(pipes[i]);
			pipes[i] = NULL;
		}
		++i;
	}
	free(pipes);
	pipes = NULL;
}