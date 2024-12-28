/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   handle_export.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: ymanchon <ymanchon@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/12/02 18:40:11 by fzayani		   #+#	#+#			 */
/*   Updated: 2024/12/26 16:09:02 by ymanchon		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_var_with_value(const char *name, const char *value, t_ctx *ctx)
{
	char	*var_name;
	char	*var_value;
	int		result;

	var_name = ft_strdup(name);
	var_value = ft_strdup(value);
	if (!var_name || !var_value)
		return (free(var_name), free(var_value), 1);
	result = create_and_add_var(ctx, var_name, var_value);
	ctx->exit_status = result;
	return (result);
}

int	export_single_var(const char *arg, t_ctx *ctx)
{
	int		result;
	char	*equal_sign;
	char	*temp_equal;
	char	*temp;

	if (ft_strcmp(arg, "=") == 0)
		return (handle_error(arg, ctx));
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (handle_no_equal(arg, ctx));
	temp = ft_strdup(arg);
	if (!temp)
		return (1);
	temp_equal = temp + (equal_sign - arg);
	*temp_equal = '\0';
	if (!is_valid_var_name(temp))
		return (free(temp), handle_error(arg, ctx));
	result = create_var_with_value(temp, equal_sign + 1, ctx);
	free(temp);
	return (result);
}

/*int	handle_with_equal(const char *arg, char *equal_sign, t_ctx *ctx)
{
	char	*temp;
	char	*temp_equal;
	int		result;

	temp = ft_strdup(arg);
	temp_equal = temp + (equal_sign - arg);
	*temp_equal = '\0';
	if (!is_valid_var_name(temp))
		return (free(temp), handle_error(arg, ctx));
	result = create_var_with_value(temp, equal_sign + 1, ctx);
	free(temp);
	return (result);
}*/

int	handle_multiple_args(const char *args, t_ctx *ctx)
{
	char	**arg_array;
	int		result;
	int		i;

	result = 0;
	arg_array = ft_split(args, ' ');
	if (!arg_array)
		return (1);
	i = 0;
	while (arg_array[i])
	{
		result |= export_single_var(arg_array[i], ctx);
		free(arg_array[i]);
		i++;
	}
	free(arg_array);
	return (result);
}

int	handle_export_builtin(const char *input, t_ctx *ctx)
{
	char	**split_args;
	int		i;

	while (*input == ' ')
		input++;
	if (!*input)
		return (handle_no_args(ctx));
	split_args = ft_split(input, ' ');
	if (!split_args)
		return (perror("ft_split"), 1);
	i = 0;
	while (split_args[i])
	{
		if (ft_strcmp(split_args[i], "=") == 0 || ft_isdigit(split_args[i][0]))
		{
			ft_fprintf(2, "MiniBG: export: `%s': not a valid identifier\n",
				split_args[i]);
			ctx->exit_status = 1;
			continue ;
		}
		if (ft_strchr(split_args[i], '-'))
		{
			ft_fprintf(2, "MiniBG: export: `%s': not a valid identifier\n",
				split_args[i]);
			ctx->exit_status = 1;
			continue ;
		}
		if (ft_strchr(split_args[i], '='))
			handle_single_arg(split_args[i], ctx);
		else
			handle_multiple_args(split_args[i], ctx);
		++i;
	}
	free_array(split_args);
	return (ctx->exit_status);
}

