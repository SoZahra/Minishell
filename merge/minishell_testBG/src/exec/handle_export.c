/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:40:11 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 23:12:14 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int handle_exit_builtin(const char *input, t_ctx *ctx)
{
    char **arg_array;
    int i;

    while (*input == ' ')
        input++;
    arg_array = ft_split(input, ' ');
    if (!arg_array)
    {
        ctx->exit_status = 1;
        write(1, "exit\n", 5);
        exit(1);
    }
    process_exit_arg(arg_array, ctx);
    i = 0;
    while (arg_array[i])
        free(arg_array[i++]);
    free(arg_array);
    write(1, "exit\n", 5);
    exit(ctx->exit_status);
    return 1;
}

// int handle_echo_builtin(const char *args, t_ctx *ctx)
// {
//     (void)ctx;
//     write(STDOUT_FILENO, args, ft_strlen(args));
//     write(STDOUT_FILENO, "\n", 1);
//     ctx->exit_status = 0;
//     return 0;
// }

// int handle_echo_builtin(const char *args, t_ctx *ctx)
// {
//     // Les tokens sont déjà créés et expansés avant d'arriver ici
//     t_token *tokens = tokenize_input((char *)args);
//     int no_newline = 0;

//     // Vérifier l'option -n
//     if (tokens && is_valid_n(tokens))
//     {
//         no_newline = 1;
//         // Supprimer les tokens -n
//         while (tokens && tokens->value[0] == '-' && 
//                ft_strchr(tokens->value, 'n'))
//         {
//             t_token *temp = tokens;
//             tokens = tokens->next;
//             free(temp->value);
//             free(temp);
//         }
//     }
//     t_token *current = tokens;
//     while (current)
//     {
//         write(STDOUT_FILENO, current->value, ft_strlen(current->value));
//         if (current->next)
//             write(STDOUT_FILENO, " ", 1);
//         current = current->next;
//     }
//     if (!no_newline)
//         write(STDOUT_FILENO, "\n", 1);
//     free_tokens(tokens);
//     ctx->exit_status = 0;
//     return 0;
// }

// Vérifie et supprime les tokens -n
// t_token *handle_n_option(t_token *tokens, int *no_newline)
// {
//     *no_newline = 0;

//     if (tokens && is_valid_n(tokens))
//     {
//         *no_newline = 1;
//         while (tokens && tokens->value[0] == '-' && 
//                ft_strchr(tokens->value, 'n'))
//         {
//             t_token *temp = tokens;
//             tokens = tokens->next;
//             free(temp->value);
//             free(temp);
//         }
//     }

//     return tokens;
// }

// // Écrire les tokens avec des espaces entre eux
// void write_echo_tokens(t_token *tokens)
// {
//     t_token *current = tokens;
//     while (current)
//     {
//         write(STDOUT_FILENO, current->value, ft_strlen(current->value));
//         if (current->next)
//             write(STDOUT_FILENO, " ", 1);
//         current = current->next;
//     }
// }

// int handle_echo_builtin(const char *args, t_ctx *ctx)
// {
//     t_token *tokens;
//     int no_newline = 0;

//     tokens = tokenize_input((char *)args);
//     tokens = handle_n_option(tokens, &no_newline);
//     write_echo_tokens(tokens);
//     if (!no_newline)
//         write(STDOUT_FILENO, "\n", 1);
//     free_tokens(tokens);
//     ctx->exit_status = 0;
//     return 0;
// }

int handle_echo_builtin(const char *args, t_ctx *ctx)
{
    t_token *tokens = tokenize_input((char *)args);
    int no_newline = 0;
    // int has_non_n_token = 0;

    t_token *current = tokens;
    while (current && is_valid_n(current))
    {
        no_newline = 1;
        current = current->next;
    }

    // Écrire les tokens
    int first = 1;
    while (current)
    {
        if (!first)
            write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, current->value, ft_strlen(current->value));
        first = 0;
        current = current->next;
    }

    // Ajouter un retour à la ligne sauf si -n
    if (!no_newline)
        write(STDOUT_FILENO, "\n", 1);

    // Libérer les tokens
    free_tokens(tokens);

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
        return (free(var_name), free(var_value), 1);
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
    char *temp = ft_strdup(arg);
    if (!temp)
        return 1;
    temp_equal = temp + (equal_sign - arg);
    *temp_equal = '\0';
    if (!is_valid_var_name(temp))
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
    ft_fprintf(2, "MiniBG: export: `%s': not a valid identifier\n", arg);
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
    char *temp;
    char *temp_equal;
    int result;

    temp = ft_strdup(arg);
    temp_equal = temp + (equal_sign - arg);
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
    int result;

    result = 0;
    arg_array = ft_split(args, ' ');
    if (!arg_array)
        return 1;
    for (int i = 0; arg_array[i]; i++)
        result |= export_single_var(arg_array[i], ctx);
    for (int i = 0; arg_array[i]; i++)
        free(arg_array[i]);
    free(arg_array);
    return result;
}

int handle_export_builtin(const char *input, t_ctx *ctx)
{
    char **split_args;

    while (*input == ' ')
        input++;
    if (!*input)
        return (handle_no_args(ctx));
    split_args = ft_split(input, ' ');
    if (!split_args)
        return (perror("ft_split"), 1);
    for (int i = 0; split_args[i]; i++)
    {
        if (ft_strchr(split_args[i], '='))
            handle_single_arg(split_args[i], ctx);
        else
            handle_multiple_args(split_args[i], ctx);
    }
    free_array(split_args);
    return 0;
}
