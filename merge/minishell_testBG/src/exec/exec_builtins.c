/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:05:20 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/10 11:55:42 by fzayani          ###   ########.fr       */
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

int handle_pwd_builtin(const char *input, t_ctx *ctx)
{
    // Skip "pwd" at the beginning
    const char *args = input + 3;
    while (*args == ' ')
        args++;

    // pwd n'a pas d'options, mais on tolère les arguments supplémentaires
    char *cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("pwd");
        ctx->exit_status = 1;
        return 1;
    }

    printf("%s\n", cwd);
    free(cwd);
    ctx->exit_status = 0;
    return 0;
}

// int	handle_pwd_builtin(char **args, t_ctx *ctx)
// {
// 	char	cwd[1024];

// 	if (args[1])
// 		;
// 	if (getcwd(cwd, sizeof(cwd)) != NULL)
// 	{
// 		printf("%s\n", cwd);
// 		ctx->exit_status = 0;
// 	}
// 	else
// 	{
// 		perror("pwd");
// 		ctx->exit_status = 1;
// 	}
// 	return (1);
// }

// int	handle_env_builtin(t_ctx *ctx)
// {
// 	(void)ctx;
// 	// printf("inside env\n");
// 	print_env(get_ctx());
// 	return (1);
// }
void print_env(t_ctx *ctx)
{
    // printf("Debug: Starting env print\n");
    t_env_var *current = ctx->env_vars;
    if (!current)
		return ;
        // printf("Debug: env list is empty\n");
    // int count = 0;
    while (current)
    {
        // printf("Debug: Found var[%d] '%s'='%s'\n", count++, current->name, current->value);
        if (current->value)
            printf("%s=%s\n", current->name, current->value);
        current = current->next;
    }
}

int handle_env_builtin(const char *input, t_ctx *ctx)
{
    // Skip "env" at the beginning and any spaces
    const char *args = input + 3;
    while (*args == ' ')
        args++;
    // Check if there are any arguments (env shouldn't have arguments in basic implementation)
    if (*args != '\0')
    {
        fprintf(stderr, "env: too many arguments\n");
        ctx->exit_status = 1;
        return 1;
    }
    // Print environment variables
    // t_env_var *current = ctx->env_vars;
    // while (current)
    // {
    //     if (current->value)  // Only print if the variable has a value
    //         printf("%s=%s\n", current->name, current->value);
    //     current = current->next;
    // }
	print_env(get_ctx());
    ctx->exit_status = 0;
    return 0;
}

int execute_builtin(const char *input, t_ctx *ctx)
{
    // printf("Debug: Executing builtin with input: '%s'\n", input);
    int builtin_type = is_builtin(input);

    if (!builtin_type)
        return 0;

    switch (builtin_type)
    {
        case 1:  // echo
            return handle_echo_builtin(input, ctx);
		case 2:
			return (handle_cd_builtin(input, ctx));
		case 3:
			return (handle_pwd_builtin(input, ctx));
        case 4:  // export - index 3 + 1
            return handle_export_builtin(input, ctx);
        case 6:  // env - index 5 + 1
            return handle_env_builtin(input, ctx);
		case 7:
			return (handle_exit_builtin(input, ctx));
        default:
            // printf("Debug: Unknown builtin type: %d\n", builtin_type);
            return 1;
    }
}

// int execute_builtin(t_ctx *ctx, t_token *tokens)
// {
//     if (!tokens || !tokens->value)
//         return 0;

//     // if (ft_strcmp(tokens->value, "exit") == 0)
//     //     return handle_exit_builtin(tokens, ctx);
//     if (ft_strcmp(tokens->value, "echo") == 0)
//         return handle_echo_builtin(tokens->next, ctx); // Ignore le mot-clé "echo"
//     // if (ft_strcmp(tokens->value, "export") == 0)
//     //     return handle_export_builtin(tokens->next, ctx);
//     // if (ft_strcmp(tokens->value, "pwd") == 0)
//     //     return handle_pwd_builtin(ctx);
//     // if (ft_strcmp(tokens->value, "cd") == 0)
//     //     return handle_cd_builtin(tokens->next, ctx);
//     // if (ft_strcmp(tokens->value, "env") == 0)
//     //     return handle_env_builtin(ctx);

//     return 0;
// }

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
