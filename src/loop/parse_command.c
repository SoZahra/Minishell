/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   parse_command.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: ymanchon <ymanchon@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/12/26 16:29:25 by ymanchon		  #+#	#+#			 */
/*   Updated: 2024/12/27 11:30:55 by ymanchon		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../include/minishell.h"

char	*handle_exit_status(char *result, t_ctx *ctx, int *i)
{
	char	*exit_str;
	char	*temp;

	exit_str = ft_itoa(ctx->exit_status);
	temp = ft_strjoin(result, exit_str);
	free(exit_str);
	free(result);
	*i += 2;
	return (temp);
}

char	*handle_regular_char(char *result, char c)
{
	char	str[2];
	char	*temp;

	str[0] = c;
	str[1] = '\0';
	temp = ft_strjoin(result, str);
	free(result);
	return (temp);
}

int	tokenizer(t_token **tokens, char *input)
{
	int	i;
	int	result;

	i = 0;
	*tokens = NULL;
	while (input[i])
	{
		if (input[i] != ' ' && input[i] != '\t')
		{
			if (is_token(input[i], TOKENS))
				result = token_proc(tokens, input, &i);
			else
				result = word_proc(tokens, input, &i);
			if (result != 0)
			{
				*tokens = NULL;
				return (-1);
			}
		}
		else if (input[i])
			i++;
	}
	_join_limiter(tokens);
	return (0);
}
