/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:40:11 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/12 18:24:00 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int handle_exit_builtin(const char *input, t_ctx *ctx)
{
    // Skip "exit" and any leading spaces
    // const char *args = input + 4;
    while (*input == ' ')
        input++;

    // Split the arguments
    char **arg_array = ft_split(input, ' ');
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

// int handle_exit_builtin(char **args, t_ctx *ctx)
// {
//     // Skip "exit"
//     if (!args[1])  // Pas d'arguments supplémentaires
//     {
//         write(1, "exit\n", 5);
//         exit(ctx->exit_status);
//     }

//     // Processus pour le premier argument
//     process_exit_arg(args + 1, ctx);

//     write(1, "exit\n", 5);
//     exit(ctx->exit_status);
//     return 0;  // Ne sera jamais atteint
// }

// int handle_echo_builtin(const char *input, t_ctx *ctx)
// {
//     (void)ctx;
//     printf("Debug: echo args len: %zu\n", strlen(input));
//     printf("Debug: echo args chars: ");
//     for (int i = 0; input[i]; i++)
//         printf("'%c' ", input[i]);
//     printf("\n");

//     // Écrire exactement ce qu'on reçoit
//     write(STDOUT_FILENO, input, ft_strlen(input));
//     write(STDOUT_FILENO, "\n", 1);

//     return 0;
// }

int handle_echo_builtin(const char *args, t_ctx *ctx)
{
    (void)ctx;
    // printf("Debug: echo: traitement des single quotes correct, pas d'expansion\n");
    write(STDOUT_FILENO, args, ft_strlen(args));
    write(STDOUT_FILENO, "\n", 1);
    ctx->exit_status = 0;
    return 0;
}

// int handle_echo_builtin(const char *input, t_ctx *ctx)
// {
//     printf("Debug: handle_echo with args: '%s'\n", input);  // À ce stade args est "hello$PWD"
//     const char *str = input;
//     int n_option = 0;

//     str += 5;  // Skip "echo "
//     while (*str == ' ')
//         str++;
//     if (str[0] == '-' && str[1] == 'n')
//     {
//         n_option = 1;
//         str += 2;
//         while (*str == ' ')
//             str++;
//     }
//     write(STDOUT_FILENO, str, ft_strlen(str));
//     if (!n_option)
//         write(STDOUT_FILENO, "\n", 1);
//     ctx->exit_status = 0;
//     return 0;
// }

// int handle_echo_builtin(char **args, t_ctx *ctx)
// {
//     printf("Debug: Inside handle_echo_builtin\n");
//     for (int j = 0; args[j]; j++)
//         printf("Debug: args[%d]: '%s'\n", j, args[j]);

//     int i = 1;  // Skip "echo"
//     int n_option = 0;

//     // Vérifier l'option -n
//     if (args[i] && strcmp(args[i], "-n") == 0)
//     {
//         n_option = 1;
//         i++;
//     }
//     // Écrire les arguments
//     while (args[i])
//     {
//         write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
//         if (args[i + 1])  // Vérifie s'il reste d'autres arguments
//             write(STDOUT_FILENO, " ", 1);
//         i++;
//     }
//     if (!n_option)
//         write(STDOUT_FILENO, "\n", 1);

//     ctx->exit_status = 0;
//     return 0;
// }



int create_var_with_value(const char *name, const char *value, t_ctx *ctx)
{
    // printf("Debug: eeeeeeeeeeeeeeeeeeeeeeee  create_var_with_value: '%s'\n", value);

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
    // printf("Debug: zzzzzzzzzzzzzzzzzzzzzzzz export_single_var: '%s'\n", arg);

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
    // printf("Debug:-++++++++++++++++ handle_no_args");
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
    // printf("Debug:................. entree handle_single_arg: '%s'\n", args);

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
    // printf("Debug:xxxxxxxxxxxxxxxxxxxxxxx handle_multiple_args: '%s'\n", args);
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
    // printf("Debug:-++++++++++++++++ entree dans handle_export: '%s'\n", input);

    // const char *args = input + 6;  // Sauter "export" + un espace
    while (*input == ' ')  // Sauter les espaces
        input++;
    // printf("Debug: Extracted args: '%s'\n", input);  // Debug supplémentaire
    if (!*input)  // Si pas d'arguments
        return handle_no_args(ctx);  // Traiter sans arguments
    // printf("Debug: Arguments after trimming spaces: '%s'\n", input);
    if (ft_strchr(input, '='))
        return handle_single_arg(input, ctx);
    return handle_multiple_args(input, ctx);
}


// int handle_export_builtin(char **args, t_ctx *ctx)
// {
//     if (!args[1])  // Aucun argument
//         return handle_no_args(ctx);

//     int i = 1;
//     while (args[i])
//     {
//         // Vérifie si un argument contient un '='
//         if (strchr(args[i], '='))
//         {
//             handle_single_arg(args[i], ctx);  // Ajoute ou met à jour une variable
//         }
//         else
//         {
//             fprintf(stderr, "export: `%s': not a valid identifier\n", args[i]);
//             ctx->exit_status = 1;
//         }
//         i++;
//     }

//     return 0;
// }
