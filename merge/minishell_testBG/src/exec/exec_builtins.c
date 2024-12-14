/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:05:20 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 16:01:44 by llarrey          ###   ########.fr       */
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
    // const char *args = input + 3;
    while (*input == ' ')
        input++;

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

/* int handle_pwd_builtin(char **args, t_ctx *ctx)
{
    if (args[1])  // Vérifie les arguments supplémentaires
    {
        fprintf(stderr, "pwd: too many arguments\n");
        ctx->exit_status = 1;
        return 1;
    }

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
} */

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
    // const char *args = input + 3;
    while (*input == ' ')
        input++;
    // Check if there are any arguments (env shouldn't have arguments in basic implementation)
    if (*input != '\0')
    {
        fprintf(stderr, "env: too many arguments\n");
        ctx->exit_status = 1;
        return 1;
    }
	print_env(get_ctx());
    ctx->exit_status = 0;
    return 0;
}

// int handle_env_builtin(char **args, t_ctx *ctx)
// {
//     if (args[1])  // `env` ne doit pas avoir d'arguments
//     {
//         fprintf(stderr, "env: too many arguments\n");
//         ctx->exit_status = 1;
//         return 1;
//     }
//     // Affiche les variables d'environnement
//     print_env(ctx);
//     ctx->exit_status = 0;
//     return 0;
// }


// int execute_builtin(const char *input, t_ctx *ctx)
// {
//     // printf("Debug: Executing builtin with input: '%s'\n", input);
//     int builtin_type = is_builtin(input);

//     if (!builtin_type)
//         return 0;

//     switch (builtin_type)
//     {
//         case 1:  // echo
//             return handle_echo_builtin(input, ctx);
// 		case 2:
// 			return (handle_cd_builtin(input, ctx));
// 		case 3:
// 			return (handle_pwd_builtin(input, ctx));
//         case 4:  // export - index 3 + 1
//             return handle_export_builtin(input, ctx);
//         case 6:  // env - index 5 + 1
//             return handle_env_builtin(input, ctx);
// 		case 7:
// 			return (handle_exit_builtin(input, ctx));
//         default:
//             // printf("Debug: Unknown builtin type: %d\n", builtin_type);
//             return 1;
//     }
// }

// int execute_builtin(char **args, t_ctx *ctx)
// {
//     if (!args || !args[0])
//     {
//         fprintf(stderr, "Debug: execute_builtin called with NULL or empty args\n");
//         return 1;
//     }

//     printf("Debug: Inside execute_builtin\n");
//     printf("Debug: args[0]: '%s'\n", args[0]);

//     if (ft_strcmp(args[0], "echo") == 0)
//     {
//         printf("Debug: Detected 'echo' builtin\n");
//         return handle_echo_builtin(args, ctx);
//     }
//     if (ft_strcmp(args[0], "cd") == 0)
//     {
//         printf("Debug: Detected 'cd' builtin\n");
//         return handle_cd_builtin(args, ctx);
//     }
//     if (ft_strcmp(args[0], "pwd") == 0)
//     {
//         printf("Debug: Detected 'pwd' builtin\n");
//         return handle_pwd_builtin(args, ctx);
//     }
//     if (ft_strcmp(args[0], "export") == 0)
//     {
//         printf("Debug: Detected 'export' builtin\n");
//         return handle_export_builtin(args, ctx);
//     }
//     if (ft_strcmp(args[0], "env") == 0)
//     {
//         printf("Debug: Detected 'env' builtin\n");
//         return handle_env_builtin(args, ctx);
//     }
//     if (ft_strcmp(args[0], "exit") == 0)
//     {
//         printf("Debug: Detected 'exit' builtin\n");
//         return handle_exit_builtin(args, ctx);
//     }

//     printf("Debug: No matching builtin for '%s'\n", args[0]);
//     return 1;
// }

int execute_builtin(const char *cmd_line, t_ctx *ctx)
{
    char *cmd = ft_strdup(cmd_line);
    char *space = strchr(cmd, ' ');
    if (space)
        *space = '\0';

    // Trouver le début des arguments (après la commande)
    const char *args = cmd_line + ft_strlen(cmd);
    while (*args == ' ')
        args++;


    // Exécuter le bon builtin
    int result = 0;
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
    else if (ft_strcmp(cmd, "exit") == 0)
        result = handle_exit_builtin(args, ctx);
    // ... autres builtins ...

    free(cmd);
    return result;
}


// int execute_builtin(char **args, t_ctx *ctx)
// {
//     // int result;
//     // char **args;  // Pour les commandes simples

//     // **args = ft_split(input, ' ');
//     // if (!args)
//     //     return 1;
//     // result = 0;
//     if(!args || !args[0])
//         return (1);

//     if(ft_strcmp(args[0], "echo") == 0)
//             return handle_echo_builtin(args, ctx);
// 	if(ft_strcmp(args[0], "cd") == 0)
// 			return (handle_cd_builtin(args, ctx));
// 	if(ft_strcmp(args[0], "pwd") == 0)
// 			return (handle_pwd_builtin(args, ctx));
//     if(ft_strcmp(args[0], "export") == 0)
//             return handle_export_builtin(args, ctx);
//      if(ft_strcmp(args[0], "env") == 0)
//             return handle_env_builtin(args, ctx);
// 	if(ft_strcmp(args[0], "exit") == 0)
// 			return (handle_exit_builtin(args, ctx));
//     return 1;
// }
