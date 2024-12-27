/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:17:12 by ymanchon          #+#    #+#             */
/*   Updated: 2024/12/27 14:56:25 by ymanchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*tokenize_input(char *line)
{
	t_token	*tokens;
	t_token	*tmp;

	if (!line)
		return (NULL);
	if (check_syntax_errors(line))
		return (NULL);
	tokens = NULL;
	if (tokenizer(&tokens, line) < 0)
		return (free_tokens(tokens), NULL);
	tmp = get_last_node(tokens);
	if (!tmp)
		return (free_tokens(tokens), NULL);
	if (is_token(tmp->type, REDIRS))
	{
		printf("syntax error\n");
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}

t_command	*parse_pipe_sequence(t_token *tokens)
{
	t_token		*current;
	t_command	*first_cmd;
	t_token		*cmd_end;
	t_command	*new_cmd;

	current = tokens;
	first_cmd = NULL;
	while (current)
	{
		cmd_end = current;
		while (cmd_end && cmd_end->type != '|')
			cmd_end = cmd_end->next;
		new_cmd = create_command_from_tokens_range(current, cmd_end);
		if (!new_cmd)
		{
			free_command_list(first_cmd);
			return (NULL);
		}
		first_cmd = link_commands(first_cmd, new_cmd);
		if (!first_cmd)
			return (free_command_list(new_cmd), NULL);
		current = find_next_command(current);
	}
	return (first_cmd);
}

int	handle_line_for_loop(char *line, t_ctx *ctx)
{
	t_command	*cmd;
	t_token		*tokens;

	if (!line || !*line)
		return (1);
	add_history(line);
	tokens = tokenize_input(line);
	if (!tokens)
		return (1);
	if (expand_proc(&tokens, ctx) == -1)
		return (free_tokens(tokens), 1);
	cmd = parse_pipe_sequence(tokens);
	free_tokens(tokens);
	if (!cmd)
		return (1);
	if (exec_loop(ctx, cmd))
		return (free_command_list(cmd), 1);
	free_command_list(cmd);
	return (0);
}

int	process(t_ctx *ctx)
{
	char	*line;

	while (1)
	{
		line = readline("MiniBG> ");
		if (line == NULL)
		{
			write(1, "exit\n", 5);
			rl_clear_history();
			return (ctx->exit_status);
		}
		handle_line_for_loop(line, ctx);
		free(line);
	}
	return (ctx->exit_status);
}

/*void	print_command_debug(t_command *command)
{
	t_command	*cmd;

	cmd = command;
	fprintf(stderr, "\n=== Command Debug ===\n");
	while (cmd)
	{
		fprintf(stderr, "Command:\n");
		fprintf(stderr, "  Args:\n");
		for (int i = 0; cmd->args && cmd->args[i]; i++)
		{
			fprintf(stderr, "	[%d]: '%s' (had_space: %d)\n", 
				i, cmd->args[i], cmd->had_spaces[i]);
		}

		fprintf(stderr, "  Redirections:\n");
		for (int i = 0; cmd->redirs && cmd->redirs[i].type != 0; i++)
		{
			fprintf(stderr, "	[%d]: type: '%c' file: '%s'\n",
				i, cmd->redirs[i].type, cmd->redirs[i].file);
		}

		fprintf(stderr, "  Links:\n");
		fprintf(stderr, "	Next: %s\n", cmd->next ? cmd->next->args[0] : "NULL");
		fprintf(stderr, "	Prev: %s\n", cmd->prev ? cmd->prev->args[0] : "NULL");
		
		fprintf(stderr, "  Path: %s\n", cmd->path ? cmd->path : "NULL");
		
		fprintf(stderr, "-----------------\n");
		cmd = cmd->next;
	}
	fprintf(stderr, "=== End Debug ===\n\n");
}*/
