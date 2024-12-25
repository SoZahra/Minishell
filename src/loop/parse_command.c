/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   parse_command.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: ymanchon <ymanchon@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/12/02 14:58:29 by fzayani		   #+#	#+#			 */
/*   Updated: 2024/12/21 19:10:56 by ymanchon		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_full_string(const char *str, char quote_type, t_ctx *ctx)
{
	char	*result;
	char	*temp;
	char	*var_name;
	char	*var_value;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				char *exit_str = ft_itoa(ctx->exit_status);
				temp = ft_strjoin(result, exit_str);
				free(exit_str);
				free(result);
				result = temp;
				i += 2;
			}
			else if (str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			{
				int j = i + 1;
				while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
					j++;
				var_name = ft_substr(str, i + 1, j - (i + 1));
				var_value = ps_get_env_var(var_name, ctx);
				free(var_name);
				if (var_value)
				{
					temp = ft_strjoin(result, var_value);
					free(result);
					result = temp;
				}
				i = j;
			}
			else
			{
				if (quote_type != '\'')
				{
					temp = ft_strjoin(result, "$");
					free(result);
					result = temp;
				}
				i++;
			}
		}
		else
		{
			char c[2] = {str[i], '\0'};
			temp = ft_strjoin(result, c);
			free(result);
			result = temp;
			i++;
		}
	}
	return result;
}

char *expand_variable_(const char *str, char quote_type, t_ctx *ctx)
{
	if (!str)
		return NULL;
	// Si c'est entre single quotes, pas d'expansion
	if (quote_type == '\'')
		return ft_strdup(str);
	if (quote_type == '"' || ft_strchr(str, '$'))	// Si c'est entre double quotes ou sans quotes, expansion complète
		return expand_full_string(str, quote_type, ctx);
	return ft_strdup(str);	// Sinon, retourner tel quel
}

int expand_str(t_token *token, t_ctx *ctx)
{
	char *expanded;

	expanded = NULL;
	if (!token || !token->value)
		return (1);
	expanded = expand_full_string(token->value, token->type, ctx);
	if (!expanded)
		return (-1);
	free(token->value);
	token->value = expanded;
	return (0);
}

int	expand_proc(t_token **tokens, t_ctx *ctx)
{
	t_token	*token;

	if (!tokens || !*tokens)
		return (0);
	token = *tokens;
	jokeroverride(tokens, ctx);
	while (token)
	{
		if ((token->type == 'S' || token->type == '"')
			&& ft_strchr(token->value, '$'))
		{
			if (expand_str(token, ctx) == -1)
				return (-1);
		}
		token = token->next;
	}
	return (0);
}

int join_str(t_token *token, bool limiter)
{
	t_token *prev;

	if (!token || !token->prev)
		return 0;
	prev = token->prev;
	if (limiter)
	{
		if (!prev->prev || prev->prev->type != 'H')
			return 0;
		prev = prev->prev;
	}
	if (join_tokens(prev, token) == -1)
		return -1;
	token_del(token);
	return 0;
}

void token_del(t_token *token)
{
	if (!token)
		return;
	if (token->prev)
		token->prev->next = token->next;
	if (token->next)
		token->next->prev = token->prev;
	free(token->value);
	free(token);
}

int join_tokens(t_token *prev, t_token *current)
{
	char *tmp;
	char *joined;

	if (!prev || !current || !prev->value || !current->value)
		return -1;

	// Sauvegarder la valeur précédente
	tmp = prev->value;

	// Joindre les valeurs
	joined = ft_strjoin(tmp, current->value);
	if (!joined)
		return -1;

	// Mettre à jour les valeurs
	prev->value = joined;
	free(tmp);

	// Hériter du type si c'est une quote
	if (current->type == '"' || current->type == '\'')
		prev->type = current->type;

	return 0;
}


int join_proc(t_token **tokens, bool limiter)
{
	t_token *token;
	t_token *next;

	if (!tokens || !*tokens)
		return 0;

	token = *tokens;
	while (token)
	{
		next = token->next; // Sauvegarder le next avant modification
		if (token->had_space)
		{
			if (join_str(token, limiter) == -1)
				return -1;
		}
		token = next;
	}
	return 0;
}


char *tokens_to_string(t_token *tokens)
{
	if(!tokens)
		return (NULL);
	t_token *current;
	char *result;
	char *temp;

	current = tokens;
	result = ft_strdup("");
	if(!result)
		return (NULL);
	while (current)
	{
		
		if (*result && !current->had_space)
		{
			temp = ft_strjoin(result, " ");
			if (!temp)
				return (free(result),NULL);
			free(result);
			result = temp;
		}
		temp = ft_strjoin(result, current->value);
		if (!temp)
			return (free(result), NULL);
		free(result);
		result = temp;
		current = current->next;
	}
	return result;
}

// char *tokens_to_string(t_token *tokens)
// {
//	 t_token *token;
//	 char *str;
//	 char *tmp;

//	 if(!tokens)
//		 return (NULL);
//	 token = tokens;
//	 str = ft_strdup(token->value);
//	 if (!str)
//		 return NULL;
//	 token = token->next;
//	 while (token)
//	 {
//		 tmp = str;
//		 str = ft_strjoin(str, token->value);
//		 if (!str)
//			 return (free(tmp), NULL);
//		 free(tmp);
//		 token = token->next;
//	 }
//	 return str;
// }

bool	is_token(char c, char *str)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == c)
			return (true);
	}
	return (false);
}

t_token *get_last_token(t_token *token_list)
{
	t_token *current;

	if (!token_list)
		return (NULL);
	current = token_list;
	while (current->next)
		current = current->next;
	return (current);
}

int quotes_proc(t_token **tokens, char *input, int *i)
{
	char *value;
	char type;
	int j;

	type = input[(*i)++];
	j = (*i);
	while (input[(*i)] && input[(*i)] != type)
		(*i)++;
	if (input[(*i)++] != type)
		return (printf("error: syntax\n"), -1);
	value = ft_substr(input, j, *i - j - 1);
	if (!value)
		return -1;
	// add_token(tokens, type, value);
	if (add_token(tokens, type, value) != 0)  // ajout
		return (free(value) ,-1); //ajout
	if (j > 1 && tokens && !ft_isspace(input[j - 2]) && !is_token(input[j - 2], UNJOIN))
		get_last_token(*tokens)->had_space = 1;
	free(value); //ajout
	return 0;
}
// > < |

void clear(pid_t *pids, t_command *cmds, int exit_code)
{
	(void)pids;
	if (cmds)
		free_args(cmds->args); 
	exit(exit_code);
}

int operators_proc(t_token **tokens, char *input, int *i, int n)
{
	char type;
	int x;
	int start;
	char *value;

	if (*tokens && is_token(get_last_token(*tokens)->type, OPERATORS))
		return (printf("error: syntax\n"), -1);
	start = *i;
	x = 1;
	type = input[(*i)++];
	// while (input[*i] && input[*i] == type && x++ < n)
	//	 (*i)++;
	while (input[*i] && input[*i] == type)
	{
		x++;
		(*i)++;
		if (x > n) // Si on dépasse la limite autorisée (1 pour |, 2 pour > et <)
			return (printf("error: syntax\n"), -1);
	}
	value = ft_substr(input, start, *i - start);
	if (!value)
		return -1;
	if (type == '<' && x > 1)
		type = 'H';
	else if (type == '>' && x > 1)
		type = 'A';
	int ret = add_token(tokens, type, value);
	free(value);
	if(ret != 0)
		return 1;
	return 0;
}

// x"y"'z'"a"'b'c>|>><"x"<<

int token_proc(t_token **tokens, char *input, int *i)
{
	if (input[*i] == '<' || input[*i] == '>' )
	{
		if (operators_proc(tokens, input, i, 2))
			return -1;
	}
	else if (input[*i] == '|')
	{
		if (operators_proc(tokens, input, i, 1))
			return -1;
	}
	else if (input[*i] == '"' || input[*i] == '\'')
	{
		if (quotes_proc(tokens, input, i))
			return -1;
	}
	return 0;
}

int word_proc(t_token **tokens, char *input, int *i)
{
	char *value;
	int j;
	int result;

	j = *i;
	while (input[(*i)] && !ft_isspace(input[(*i)]) && !is_token(input[*i], TOKENS))
		(*i)++;
	if (j == *i)
		return 0;
	value = ft_substr(input, j, *i - j);
	if (!value)
		return -1;
	result = add_token(tokens, 'S', value);
	free(value);
	if (result != 0)
		return (-1);
	if (j > 0 && input[j - 1] && !ft_isspace(input[j - 1]) && !is_token(input[j - 1], UNJOIN))
		get_last_token(*tokens)->had_space = 1;
	return 0;
}

t_token *join_it(t_token *token)
{
	char *tmp;
	t_token *prev;
	(void)tmp;
	(void)prev;
	
	if (!token)
		return NULL;
	if (token->prev && token->prev->prev && token->prev->prev->type == 'H')
	{
		tmp = token->prev->value;
		token->prev->value = ft_strjoin(tmp, token->value);
		if (!token->prev->value)
		{
			token->prev->value = tmp;
			return NULL;
		}
		free(tmp);
		token->prev->next = token->next;
		if (token->next)
			token->next->prev = token->prev;
		free(token->value);
		prev = token->prev;
		free(token);
		return prev;
	}
	return NULL;
}

int _join_limiter(t_token **tokens)
{
	t_token *tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->had_space)
		{
			tmp = join_it(tmp);
			if (!tmp)
				return -1;
		}
		tmp = tmp->next;
	}
	return 0;
}


int tokenizer(t_token **tokens, char *input)
{
	int i;
	int result;

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
				// free_tokens(*tokens);
				*tokens = NULL;
				return -1;
			}
		}
		else if (input[i])
			i++;
	}
	_join_limiter(tokens);
	return 0;
}
