/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:05:20 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/26 12:49:55 by fzayani          ###   ########.fr       */
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
		return (free(var),free(value), 1);
	new_var->name = var;
	new_var->value = value;
	new_var->next = NULL;
	add_env_var_to_list(&ctx->env_vars, new_var);
	return (0);
}

// int handle_pwd_builtin(const char *input, t_ctx *ctx)
// {
//     char *cwd;

//     while (*input == ' ')
//         input++;
//     cwd = getcwd(NULL, 0);
//     if (!cwd)
//     {
//         perror("pwd");
//         ctx->exit_status = 1;
//         return 1;
//     }
//     printf("%s\n", cwd);
//     free(cwd);
//     ctx->exit_status = 0;
//     return 0;
// }

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
    t_env_var *current;

    current = ctx->env_vars;
    if (!current)
        return;
    while (current)
    {
        if (current->name)
        {
            if (current->value)
                printf("%s=%s\n", current->name, current->value);
            else
                printf("%s=\n", current->name);
        }
        current = current->next;
    }
}

int handle_env_builtin(const char *input, t_ctx *ctx)
{
    while (*input == ' ')
        input++;
    if (*input != '\0')
    {
        ft_fprintf(2, "env: too many arguments\n");
        ctx->exit_status = 1;
        return 1;
    }
	print_env(get_ctx());
    ctx->exit_status = 0;
    return 0;
}

int execute_builtin(char *cmd_line, t_ctx *ctx)
{
    char *cmd;
    char *space;
    const char *args;
    int result;
    char *args_dup = NULL;

    cmd = ft_strdup(cmd_line);
    if(!cmd)
        return (1);
    space = ft_strchr(cmd, ' ');
    if (space)
        *space = '\0';
    args = cmd_line + ft_strlen(cmd);
    while (*args == ' ')
        args++;
    result = 0;
    if (ft_strcmp(cmd, "exit") == 0)
    {
        args_dup = ft_strdup(args);
        if (!args_dup)
        {
            free(cmd);
            return (1);
        }
        free(cmd);
        free(cmd_line);
        result = handle_exit_builtin(args_dup, ctx);
        free(args_dup);  // On libère args_dup une fois qu'on a le résultat
        return (result);
        // return (handle_exit_builtin(args_dup, ctx));
    }
    if (ft_strcmp(cmd, "echo") == 0)
        result = handle_echo_builtin(args, ctx);
    else if (ft_strcmp(cmd, "cd") == 0)
        result = handle_cd_builtin(args, ctx);
    else if (ft_strcmp(cmd, "pwd") == 0)
        result = handle_pwd_builtin(args, ctx);
    else if (ft_strcmp(cmd, "export") == 0)
        result = handle_export_builtin(args, ctx);
    else if (ft_strcmp(cmd, "env") == 0)
        result = handle_env_builtin(args, ctx);
    // else if (ft_strcmp(cmd, "exit") == 0)
    //     result = handle_exit_builtin(args, ctx);
    else if (ft_strcmp(cmd, "unset") == 0)
        result = handle_unset_builtin(args, ctx);
    return (free(cmd), result);
}

// int handle_unset_builtin(const char *input, t_ctx *ctx)
// {
//     char **split_args;
//     int i;

//     while (*input == ' ')
//         input++;
//     if (!*input)
//         return 0;
//     split_args = ft_split(input, ' ');
//     if (!split_args)
//         return (perror("ft_split"), 1);
//     i = 0;
//     while(split_args[i])
//     {
//         if (!is_valid_var_name(split_args[i]))
//         {
//             ft_fprintf(2, "MiniBG: unset: `%s': not a valid identifier\n", split_args[i]);
//             ctx->exit_status = 1;
//             continue;
//         }
//         if (unset_v(&ctx->env_vars, split_args[i]) != 0)
//             ctx->exit_status = 0;
//         i++;
//     }
//     return (free_array(split_args), 0);
// }

// int unset_v(t_env_var **env_vars, const char *var)
// {
//     t_env_var *current;
//     t_env_var *prev;

//     current = *env_vars;
//     prev = NULL;
//     while (current)
//     {
//         if (ft_strcmp(current->name, var) == 0)
//         {
//             if (prev == NULL)
//                 *env_vars = current->next;
//             else
//                 prev->next = current->next;
//             free(current->name);
//             free(current->value);
//             free(current);
//             return 0;
//         }
//         prev = current;
//         current = current->next;
//     }
//     return 1;
// }

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
