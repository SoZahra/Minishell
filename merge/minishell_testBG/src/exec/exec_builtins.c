/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:05:20 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/09 23:49:23 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_invalid_identifier(char *arg, char *var, char *value)
{
	if (var)
		fprintf(stderr, "export: `%s`: not a valid identifier\n", var);
	else
		fprintf(stderr, "export: `%s`: not a valid identifier\n", arg);
	free(var);
	free(value);
	return (1);
}

int	create_and_add_var(t_ctx *ctx, char *var, char *value)
{
	t_env_var	*new_var;

	new_var = malloc(sizeof(t_env_var));
	if (!new_var)
	{
		free(var);
		free(value);
		return (1);
	}
	new_var->name = var;
	new_var->value = value;
	new_var->next = NULL;
	// printf("test add var : %s\n", var);
	add_env_var_to_list(&ctx->env_vars, new_var);
	return (0);
}

int	handle_pwd_builtin(char **args, t_ctx *ctx)
{
	char	cwd[1024];

	if (args[1])
		;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		ctx->exit_status = 0;
	}
	else
	{
		perror("pwd");
		ctx->exit_status = 1;
	}
	return (1);
}

int	handle_env_builtin(t_ctx *ctx)
{
	(void)ctx;
	// printf("inside env\n");
	print_env(get_ctx());
	return (1);
}

int execute_builtin(t_ctx *ctx, t_token *tokens)
{
    if (!tokens || !tokens->value)
        return 0;

    // if (ft_strcmp(tokens->value, "exit") == 0)
    //     return handle_exit_builtin(tokens, ctx);
    if (ft_strcmp(tokens->value, "echo") == 0)
        return handle_echo_builtin(tokens->next, ctx); // Ignore le mot-clé "echo"
    // if (ft_strcmp(tokens->value, "export") == 0)
    //     return handle_export_builtin(tokens->next, ctx);
    // if (ft_strcmp(tokens->value, "pwd") == 0)
    //     return handle_pwd_builtin(ctx);
    // if (ft_strcmp(tokens->value, "cd") == 0)
    //     return handle_cd_builtin(tokens->next, ctx);
    // if (ft_strcmp(tokens->value, "env") == 0)
    //     return handle_env_builtin(ctx);

    return 0;
}

// int	exec_builtin_cmd(char **args, char **env, t_ctx *ctx)
// {
// 	(void)env;
// 	if (ft_strcmp(args[0], "exit") == 0)
// 		return (handle_exit_builtin(args, ctx));
// 	if (ft_strcmp(args[0], "echo") == 0)
// 		return (handle_echo_builtin(args, ctx));
// 	if (ft_strcmp(args[0], "export") == 0)
// 		return (handle_export_builtin(args, ctx));
// 	if (ft_strcmp(args[0], "pwd") == 0)
// 		return (handle_pwd_builtin(args, ctx));
// 	if (ft_strcmp(args[0], "cd") == 0)
// 		return (handle_cd_builtin(args, ctx));
// 	if (ft_strcmp(args[0], "env") == 0)
// 		return (handle_env_builtin(ctx));
// 	return (0);
// }
