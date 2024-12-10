/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:40:11 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/10 15:13:50 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int handle_exit_builtin(char **args, t_ctx *ctx)
// {
//     process_exit_arg(args, ctx);
//     write(1, "exit\n", 5);
//     exit(ctx->exit_status);
//     return (1); // Ne sera jamais atteint
// }

// int handle_echo_builtin(char **args, t_ctx *ctx)
// {
//     t_token *token_list;

//     token_list = create_token_list(args + 1);
//     if (!token_list)
//     {
//         ctx->exit_status = 1;
//         return (1);
//     }
//     handle_echo(token_list, ctx);
//     return (1);
// }

int handle_exit_builtin(const char *input, t_ctx *ctx)
{
    // Skip "exit" and any leading spaces
    const char *args = input + 4;
    while (*args == ' ')
        args++;

    // Split the arguments
    char **arg_array = ft_split(args, ' ');
    if (!arg_array)
    {
        ctx->exit_status = 1;
        write(1, "exit\n", 5);
        exit(1);
    }
    process_exit_arg(arg_array, ctx);
    // Cleanup avant de sortir
    int i = 0;
    while (arg_array[i])
        free(arg_array[i++]);
    free(arg_array);

    write(1, "exit\n", 5);
    exit(ctx->exit_status);
    return 1; // Ne sera jamais atteint
}

int handle_echo_builtin(const char *input, t_ctx *ctx)
{
    const char *str = input;
    int n_option = 0;

    str += 5;  // Skip "echo "
    while (*str == ' ')
        str++;
    if (str[0] == '-' && str[1] == 'n')
    {
        n_option = 1;
        str += 2;
        while (*str == ' ')
            str++;
    }
    write(STDOUT_FILENO, str, ft_strlen(str));
    if (!n_option)
        write(STDOUT_FILENO, "\n", 1);
    ctx->exit_status = 0;
    return 0;
}

int create_var_with_value(const char *name, const char *value, t_ctx *ctx)
{
    char *var_name;
    char *var_value;
    int result;

    var_name = ft_strdup(name);
    var_value = ft_strdup(value);
    if (!var_name || !var_value)
    {
        free(var_name);
        free(var_value);
        return 1;
    }
    result = create_and_add_var(ctx, var_name, var_value);
    ctx->exit_status = result;
    return result;
}

int handle_no_equal(const char *arg, t_ctx *ctx)
{
    if (!is_valid_var_name(arg))
        return handle_error(arg, ctx);
    return (create_and_add_var(ctx, ft_strdup(arg), NULL));
}
static int export_single_var(const char *arg, t_ctx *ctx)
{
    int result;
    char *equal_sign;
    char *temp_equal;

    if (ft_strcmp(arg, "=") == 0)
        return handle_error(arg, ctx);
    equal_sign = ft_strchr(arg, '=');
    if (!equal_sign)
        return handle_no_equal(arg, ctx);
    char *temp = ft_strdup(arg);// Créer une copie temporaire pour la modification
    if (!temp)
        return 1;
    temp_equal = temp + (equal_sign - arg);// Pointer vers le = dans notre copie
    *temp_equal = '\0';  // Couper temporairement au =
    if (!is_valid_var_name(temp))// Vérifier si le nom est valide
        return (free(temp), handle_error(arg, ctx));
    result = create_var_with_value(temp, equal_sign + 1, ctx);
    free(temp);
    return result;
}

int handle_no_args(t_ctx *ctx)
{
    print_export(ctx);
    ctx->exit_status = 0;
    return 0;
}

int handle_error(const char *arg, t_ctx *ctx)
{
    fprintf(stderr, "MiniBG: export: `%s': not a valid identifier\n", arg);
    ctx->exit_status = 1;
    return 1;
}

int handle_no_equal_sign(char *arg, t_ctx *ctx)
{
    int result;
    if (!is_valid_var_name(arg))
        return handle_error(arg, ctx);
    result = create_and_add_var(ctx, ft_strdup(arg), NULL);
    free(arg);
    ctx->exit_status = result;
    return result;
}

int handle_with_equal(const char *arg, char *equal_sign, t_ctx *ctx)
{
    char *temp = ft_strdup(arg);
    char *temp_equal = temp + (equal_sign - arg);
    int result;

    *temp_equal = '\0';
    if (!is_valid_var_name(temp))
        return (free(temp), handle_error(arg, ctx));
    result = create_var_with_value(temp, equal_sign + 1, ctx);
    free(temp);
    return result;
}

int handle_single_arg(const char *args, t_ctx *ctx)
{
    int result;
    char *arg;

    arg = ft_strdup(args);
    if (!arg)
        return 1;
    result = export_single_var(arg, ctx);
    free(arg);
    return result;
}


int handle_multiple_args(const char *args, t_ctx *ctx)
{
    char **arg_array;

    arg_array = ft_split(args, ' ');
    if (!arg_array)
        return 1;

    int result = 0;
    for (int i = 0; arg_array[i]; i++)
        result |= export_single_var(arg_array[i], ctx);
    // Nettoyage
    for (int i = 0; arg_array[i]; i++)
        free(arg_array[i]);
    free(arg_array);
    return result;
}

int handle_export_builtin(const char *input, t_ctx *ctx)
{
    const char *args;

    args = input + 6;
    while (*args == ' ')
        args++;
    if (!*args)
        return handle_no_args(ctx);
    // Si on a un '=', traiter comme un seul argument
    // pour préserver les espaces dans la valeur
    if (ft_strchr(args, '='))
        return handle_single_arg(args, ctx);
    // Sinon, traiter comme des arguments multiples
    return handle_multiple_args(args, ctx);
}

