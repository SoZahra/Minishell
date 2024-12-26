/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 19:40:17 by llarrey           #+#    #+#             */
/*   Updated: 2024/12/26 15:21:37 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// static const char	*parse_command(const char *cmd_line, char **cmd)
// {
// 	char		*space;
// 	const char	*args;

// 	*cmd = ft_strdup(cmd_line);
// 	space = ft_strchr(*cmd, ' ');
// 	if (space)
// 		*space = '\0';
// 	args = cmd_line + ft_strlen(*cmd);
// 	while (*args == ' ')
// 		args++;
// 	return (args);
// }

// int	execute_builtin(const char *cmd_line, t_ctx *ctx)
// {
// 	char		*cmd;
// 	const char	*args;
// 	int			result;

// 	args = parse_command(cmd_line, &cmd);
// 	result = 0;
// 	if (ft_strcmp(cmd, "echo") == 0)
// 		result = handle_echo_builtin(args, ctx);
// 	else if (ft_strcmp(cmd, "cd") == 0)
// 		result = handle_cd_builtin(args, ctx);
// 	else if (ft_strcmp(cmd, "pwd") == 0)
// 		result = handle_pwd_builtin(args, ctx);
// 	else if (ft_strcmp(cmd, "export") == 0)
// 		result = handle_export_builtin(args, ctx);
// 	else if (ft_strcmp(cmd, "env") == 0)
// 		result = handle_env_builtin(args, ctx);
// 	else if (ft_strcmp(cmd, "exit") == 0)
// 		result = handle_exit_builtin(args, ctx);
// 	else if (ft_strcmp(cmd, "unset") == 0)
// 		result = handle_unset_builtin(args, ctx);
// 	return (free(cmd), result);
// }

int	handle_unset_builtin(const char *input, t_ctx *ctx)
{
	char	**split_args;
	int		i;

	while (*input == ' ')
		input++;
	if (!*input)
		return (0);
	split_args = ft_split(input, ' ');
	if (!split_args)
		return (perror("ft_split"), 1);
	i = 0;
	while (split_args[i])
	{
		if (!is_valid_var_name(split_args[i]))
		{
			ft_fprintf(2, "MiniBG: unset: `%s': not a valid identifier\n",
				split_args[i]);
			ctx->exit_status = 1;
			continue ;
		}
		if (unset_v(&ctx->env_vars, split_args[i]) != 0)
			ctx->exit_status = 0;
		i++;
	}
	return (free_array(split_args), 0);
}

int	unset_v(t_env_var **env_vars, const char *var)
{
	t_env_var	*current;
	t_env_var	*prev;

	current = *env_vars;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, var) == 0)
		{
			if (prev == NULL)
				*env_vars = current->next;
			else
				prev->next = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}
