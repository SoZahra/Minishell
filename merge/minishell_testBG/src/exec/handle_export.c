/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:40:11 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/26 12:41:39 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int clear_exit(t_ctx *ctx)
{
    write(1, "exit\n", 5);
    cleanup_shell(ctx);
    rl_clear_history();
    free_command_list(get_ctx()->current_command);
    return(0);
}

int handle_exit_builtin(const char *input, t_ctx *ctx)
{
    char **arg_array;
    int i;
    int should_exit;

    while (*input == ' ')
        input++;
    arg_array = ft_split(input, ' ');
    if (!arg_array)
    {
        ctx->exit_status = 1;
        clear_exit(ctx);
        free((void*)input);
        exit(1);
    }
    should_exit = process_exit_arg(arg_array, ctx);
    i = 0;
    while (arg_array[i])
        free(arg_array[i++]);
    free(arg_array);
    if(should_exit)
    {
        clear_exit(ctx);
        free((void*)input);
        exit(ctx->exit_status);
    }
    return (1);
}

int handle_echo_builtin(const char *args, t_ctx *ctx)
{
    while (*args == ' ')
        args++;
    if (args[0] == '-' && 
        (ft_strchr(args, 'n') == args + 1 || 
         (ft_strspn(args + 1, "n") == ft_strlen(args + 1))))
    {
        return handle_echo_builtin_n(args, ctx);
    }
    write(STDOUT_FILENO, args, ft_strlen(args));
    write(STDOUT_FILENO, "\n", 1);
    ctx->exit_status = 0;
    return 0;
}

int handle_echo_builtin_n(const char *args, t_ctx *ctx)
{
    t_token *tokens;
    int no_newline;
    t_token *current;
    int first;

    tokens = tokenize_input((char *)args);
    no_newline = 0;
    current = tokens;
    while (current && is_valid_n(current))
    {
        no_newline = 1;
        current = current->next;
    }
    first = 1;
    while (current)
    {
        if (!first)
            write(STDOUT_FILENO, " ", 1);
        write(STDOUT_FILENO, current->value, ft_strlen(current->value));
        first = 0;
        current = current->next;
    }
    if (!no_newline)
        write(STDOUT_FILENO, "\n", 1);
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
int export_single_var(const char *arg, t_ctx *ctx)
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

void	print_export(t_ctx *ctx)
{
	t_env_var	*current;

	current = ctx->env_vars;
	while (current)
	{
		if (current->value)
			printf("%s=%s\n", current->name, current->value);
		else
			printf("%s\n", current->name);
		current = current->next;
	}
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

int handle_single_arg(char *args, t_ctx *ctx)
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


// int handle_multiple_args(const char *args, t_ctx *ctx)
// {
//     char **arg_array;
//     int result;
//     int i;

//     result = 0;
//     arg_array = ft_split(args, ' ');
//     if (!arg_array)
//         return 1;
//     i = 0;
//     while(arg_array[i])
//     {
//         result |= export_single_var(arg_array[i], ctx);
//         i++;
//     }
//     while(arg_array[i])
//         free(arg_array[i++]);
//     free(arg_array);
//     return result;
// }

int handle_multiple_args(const char *args, t_ctx *ctx)
{
    char **arg_array;
    int result;
    int i;

    result = 0;
    arg_array = ft_split(args, ' ');
    if (!arg_array)
        return 1;
    i = 0;
    while(arg_array[i])
    {
        result |= export_single_var(arg_array[i], ctx);
        free(arg_array[i]);  // Libérer chaque élément après l'avoir utilisé
        i++;
    }
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
        if (ft_strcmp(split_args[i], "=") == 0 || ft_isdigit(split_args[i][0]))
        {
            ft_fprintf(2, "MiniBG: export: `%s': not a valid identifier\n", 
                      split_args[i]);
            ctx->exit_status = 1;
            continue;
        }
        if (ft_strchr(split_args[i], '-'))
        {
            ft_fprintf(2, "MiniBG: export: `%s': not a valid identifier\n", 
                      split_args[i]);
            ctx->exit_status = 1;
            continue;
        }
        if (ft_strchr(split_args[i], '='))
            handle_single_arg(split_args[i], ctx);
        else
            handle_multiple_args(split_args[i], ctx);
    }
    free_array(split_args);
    return ctx->exit_status;
}

