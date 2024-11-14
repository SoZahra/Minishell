/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:03:19 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/14 17:26:57 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**get_environment(char **envp)
{
	int		i;
	int		count;
	char	**env_copy;

	i = 0;
	count = 0;
	while (envp[count])
		count++;
	env_copy = (char **)malloc((count + 1) * sizeof(char *));
	if (!env_copy)
		return (perror("malloc failed"), NULL);
	while (i < count)
	{
		env_copy[i] = strdup(envp[i]);
		if (!env_copy[i])
		{
			perror("strdup failed");
			while (--i >= 0)
				free(env_copy[i]);
			return (free(env_copy), NULL);
		}
		i++;
	}
	return (env_copy[i] = NULL, env_copy);
}

int export_v(char ***env_copy, const char *var, const char *value)
{
    int i;
    size_t len;
    char *new_var;

    i = 0;
    len = strlen(var);

    // Recherche si la variable existe déjà
    while ((*env_copy)[i]) {
        if (ft_strncmp_export((*env_copy)[i], var, len) == 0 && (*env_copy)[i][len] == '=') {
            // Libère l'ancienne valeur et crée une nouvelle variable
            free((*env_copy)[i]);
            new_var = ft_strjoin(var, "="); // Crée une nouvelle chaîne de base
            char *value_joined = ft_strjoin(new_var, value); // Concatène la valeur

            // Met à jour la variable dans l'environnement
            (*env_copy)[i] = value_joined;
            free(new_var); // Libère la mémoire de la chaîne intermédiaire
            return 0;
        }
        i++;
    }

    // Si la variable n'existe pas, ajoute-la
    new_var = ft_strjoin(var, "=");
    char *new_entry = ft_strjoin(new_var, value);
    free(new_var); // Libère la mémoire de la chaîne intermédiaire

    // Réalloue l'espace pour ajouter une nouvelle variable
    size_t current_size = i; // Nombre actuel d'éléments
    *env_copy = realloc(*env_copy, (current_size + 2) * sizeof(char *));
    if (!*env_copy) {
        perror("realloc failed");
        return -1; // Erreur de réallocation
    }

    (*env_copy)[current_size] = new_entry; // Ajoute la nouvelle entrée
    (*env_copy)[current_size + 1] = NULL; // Termine avec NULL
    return 0;
}


int unset_v(char ***env_copy, const char *var)
{
	int i;
	size_t len;

	i = 0;
	len = ft_strlen(var);

	while((*env_copy)[i++])
	{
		if(ft_strncmp_export((*env_copy)[i], var, len) == 0 && (*env_copy)[i][len] == '=')
		{
			free((*env_copy)[i]);
			while((*env_copy)[i + 1])
			{
				(*env_copy)[i] = (*env_copy)[i + 1];
				i++;
			}
			(*env_copy)[i] = NULL;
			return (0);
		}
		//i++;
	}
	return(-1);
}


char *expand_env_var(char *token, int *exit_status)
{
    if (!token) return NULL;

    char *result = malloc(1);
    if (!result) return NULL;
    result[0] = '\0';

    int i = 0;
    while (token[i] != '\0')
    {
        if (token[i] == '$')
        {
            if (token[i + 1] == '?')  // Gérer l'expansion de `$?`
            {
                char *num = ft_itoa(*exit_status);
                char *tmp = result;
                result = ft_strjoin(result, num);
                free(tmp);
                free(num);
                i += 2;  // Passer `$?`
            }
            else  // Gérer l'expansion d'une variable d'environnement `$VAR`
            {
                int j = i + 1;
                while (token[j] && (ft_isalnum(token[j]) || token[j] == '_'))
                    j++;
                char *var_name = ft_strndup(token + i + 1, j - i - 1);
                char *env_value = getenv(var_name);
                free(var_name);
                if (!env_value) env_value = "";

                char *tmp = result;
                result = ft_strjoin(result, env_value);
                free(tmp);
                i = j;
            }
        }
        else
        {
            int start = i;
            while (token[i] != '\0' && token[i] != '$')
                i++;

            char *text = ft_strndup(token + start, i - start);
            char *tmp = result;
            result = ft_strjoin(result, text);
            free(tmp);
            free(text);
        }
    }
    return result;
}

// Fonction pour obtenir la partie avant le caractère `$`
char *ps_get_before_env(char *str, char *found)
{
    return (ft_strndup(str, found - str)); // Copie jusqu'au `$`
}

// Fonction pour obtenir la valeur de la variable d'environnement ou une chaîne vide si elle n'existe pas
char *ps_get_env_var(char *var_name, t_ctx *ctx)
{
    if (strcmp(var_name, "?") == 0) {
        return ft_itoa(ctx->exit_status);
    }
    char *env_value = getenv(var_name);
    return env_value ? ft_strdup(env_value) : ft_strdup("");
}

// Fonction pour obtenir la partie après le nom de la variable
char *ps_get_after_env(char *found)
{
    int i = 0;
    while (found[i] && (ft_isalnum(found[i]) || found[i] == '_')) i++;
    return ft_strdup(found + i);
}

// Convertit une chaîne contenant une variable d'environnement en une chaîne avec la valeur étendue
char *ps_convert_to_env(char *str, char *found, t_ctx *ctx)
{
    char *before_env = ps_get_before_env(str, found);
    char *env_var = ps_get_env_var(found + 1, ctx); // `+1` pour ignorer le `$`
    char *after_env = ps_get_after_env(found + 1);

    if (!before_env || !env_var || !after_env)
    {
        if (before_env)// Libérer tous les pointeurs s'ils sont non nuls
            free(before_env);
        if (env_var)
            free(env_var);
        if (after_env)
            free(after_env);
        return NULL;
    }
    char *new = ft_strjoin(before_env, env_var);// Libération de before_env et env_var après l'utilisation
    free(before_env);
    free(env_var);
    if (!new)
        return (free(after_env), NULL);
    char *final_result = ft_strjoin(new, after_env);
    free(after_env);
    free(new); // Libération de new pour éviter une fuite
    return final_result; // Renvoie le résultat final
}

// Compte les occurrences de `$` dans une chaîne
int ft_count_exp(char *str)
{
    int i = 0, count = 0;
    while (str[i])
    {
        if (str[i] == '$') count++;
        i++;
    }
    return count;
}

// Gère l'expansion des variables d'environnement dans un token
int ps_handle_env(t_token *token, t_ctx *ctx)
{
    char *tmp, *found, *new;
    int nb = ft_count_exp(token->value);
    while (token->value && nb > 0) {
        found = ft_strchr(token->value, '$');
        if (!found) break;
        tmp = token->value;
        new = ps_convert_to_env(token->value, found, ctx);
        token->value = new && new[0] == '\0' ? NULL : new;
        free(tmp);
        nb--;
    }
    return 0;
}

void ps_expand_env(t_token *tokens, t_ctx *ctx)
{
    (void)ctx;

    while (tokens) {
        char *token = tokens->value;
        char *result = NULL;

        for (int i = 0; token[i] != '\0'; i++)
        {
            if (token[i] == '$') {
                // Si le prochain caractère est un guillemet, on ajoute `$` comme une chaîne et on ignore l'expansion
                if (token[i + 1] == '"') {
                    char single_char[2] = {'$', '\0'};
                    char *tmp = result;
                    result = ft_strjoin(result ? result : "", single_char);
                    free(tmp);
                    continue; // Passez au caractère suivant
                }
                char *var_start = &token[i + 1];
                int j = 0;
                while (ft_isalpha(var_start[j]) || var_start[j] == '_')
                    j++;
                if (j > 0)
                {
                    char var_name[j + 1];
                    strncpy(var_name, var_start, j);
                    var_name[j] = '\0';
                    char *env_value = getenv(var_name);
                    if (env_value)
                    {
                        char *tmp = result;
                        result = ft_strjoin(result ? result : "", env_value);
                        free(tmp);
                    }
                    i += j;
                }
            }
            else
            {
                // Ajoutez les autres caractères directement dans la chaîne de résultat
                char single_char[2] = {token[i], '\0'};
                char *tmp = result;
                result = ft_strjoin(result ? result : "", single_char);
                free(tmp);
            }
        }
        free(tokens->value);
        tokens->value = result;
        tokens = tokens->next;
    }
}


int	ft_strncmp_export(const char *s1, const char *s2, unsigned int n)
{
	unsigned int	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] && s2[i] && n - 1 > i)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
int	is_valid_id(const char *var)
{
	int	i;

	i = 1;
	if (!var || !*var)
		return (0);
	if (!ft_isalpha(var[0]) && var[0] != '_')
		return (0);
	while (var[i] != '\0')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}


void	write_echo_content(t_token *token_list, int n_option)
{
	t_token	*current = token_list;
	int		first_arg = 1;

	while (current)
	{
		if (current->value && current->value[0] != '\0') // Ignore les arguments vides
		{
			if (!first_arg)
				write(STDOUT_FILENO, " ", 1); // Ajoute un espace seulement entre arguments non-vides
			write(STDOUT_FILENO, current->value, ft_strlen(current->value));
			first_arg = 0;
		}
		current = current->next;
	}
	if (!n_option)
		write(STDOUT_FILENO, "\n", 1);
}

void	handle_echo(t_token *token_list)
{
	int		n_option = 0;

	// Vérifie si le premier argument est "-n" pour ne pas ajouter de saut de ligne
	if (token_list && ft_strcmp(token_list->value, "-n") == 0)
	{
		n_option = 1;
		token_list = token_list->next; // Passe au token suivant
	}
	write_echo_content(token_list, n_option);
}

int	handle_quotes(char **line, int *in_quotes, char *quote_char, char *buffer,
		int *i, t_token **token_list)
{
	if (**line == '"' || **line == '\'')
	{
		if (*in_quotes && **line == *quote_char)
		{
			*in_quotes = 0;
			*quote_char = 0;
			if (*i > 0)  // Vérifie que le buffer contient des caractères avant d'ajouter le token
			{
				buffer[*i] = '\0';
				add_token(token_list, TOKEN_ARGUMENT, buffer);
				*i = 0;
			}
		}
		else if (!*in_quotes)
		{
			*in_quotes = 1;
			*quote_char = **line;
		}
		(*line)++;
		return (1);
	}
	return (0);
}

int	handle_whitespace(char **line, char *buffer, int *i, t_token **token_list,
		int in_quotes)
{
	if (is_whitespace(**line) && !in_quotes)
	{
		if (*i > 0)
		{
			buffer[*i] = '\0';
			add_token(token_list, TOKEN_ARGUMENT, buffer);
			*i = 0;
		}
		(*line)++;
		return (1);
	}
	return (0);
}

int	handle_special_chars(char **line, char *buffer, int *i,
		t_token **token_list, int in_quotes)
{
	char	special[3];

	ft_bzero(special, sizeof(special));
	if ((**line == '|' || **line == '<' || **line == '>') && !in_quotes)
	{
		if (*i > 0)
		{
			buffer[*i] = '\0';
			add_token(token_list, TOKEN_ARGUMENT, buffer);
			*i = 0;
		}
		special[0] = **line;
		(*line)++;
		if ((special[0] == '>' && **line == '>') || (special[0] == '<'
				&& **line == '<'))
			special[1] = *(*line)++;
		add_token(token_list, get_token_type(special), special);
		return (1);
	}
	return (0);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	handle_space(char **ptr)
{
	if (**ptr == ' ')
	{
		(*ptr)++;
		return (1);
	}
	return (0);
}

t_token	*create_token_from_pipe(t_token **head, t_token **tail)
{
	return (add_pipe_token(head, tail));
}

t_token	*parse_command_line(char *line)
{
	t_token	*token_list = NULL;
	char	buffer[1024];
	int		i = 0;
	int		in_single_quotes = 0;
	int		in_double_quotes = 0;

	while (*line)
	{
		printf("EEEEE: %d, %d\n", in_single_quotes, !in_single_quotes);
		// Gestion des simples quotes (on ne traite pas $ comme une variable)
		if (*line == '\'' && !in_double_quotes)
		{
			printf("AAAAAPP: %d, %d\n", in_single_quotes, !in_single_quotes);
			in_single_quotes = !in_single_quotes;
			line++;
			continue;
		}
		// Gestion des doubles quotes (on traite $ comme une variable)
		if (*line == '"' && !in_single_quotes)
		{
			printf("AAAAAQQ: %d, %d\n", in_single_quotes, !in_single_quotes);
			in_double_quotes = !in_double_quotes;
			line++;
			continue;
		}
		// Expansion de variable : $ seulement en dehors des simples quotes
		if (*line == '$' && !in_single_quotes)
		{
			printf("AAAAASS: %d, %d\n", in_single_quotes, !in_single_quotes);
			if (handle_env_var(&line, &token_list))
				continue;
		}
		// Gestion des espaces en dehors des quotes
		if (is_whitespace(*line) && !in_single_quotes && !in_double_quotes)
		{
			if (i > 0) // Crée un token seulement si le buffer n'est pas vide
			{
				buffer[i] = '\0';
				add_token(&token_list, TOKEN_ARGUMENT, buffer);
				i = 0;
			}
			line++;
			continue;
		}
		// Ajout des caractères normaux au buffer
		buffer[i++] = *line++;
	}
	// Crée le dernier token si le buffer contient du texte
	if (i > 0)
	{
		buffer[i] = '\0';
		add_token(&token_list, TOKEN_ARGUMENT, buffer);
	}

	return token_list;
}


// t_token	*parse_command_line(char *line)
// {
// 	t_token	*token_list = NULL;
// 	char	buffer[1024];
// 	int		i = 0;
// 	int		in_quotes = 0;
// 	char	quote_char = 0;

// 	while (*line)
// 	{
// 		// Détecte l'ouverture ou la fermeture des quotes
// 		if ((*line == '"' || *line == '\'') && (!in_quotes || *line == quote_char))
// 		{
// 			if (in_quotes && *line == quote_char) // Ferme la quote actuelle
// 			{
// 				in_quotes = 0;
// 				quote_char = 0;
// 			}
// 			else if (!in_quotes) // Ouvre une nouvelle quote
// 			{
// 				in_quotes = 1;
// 				quote_char = *line;
// 			}
// 			line++;
// 			continue; // Ignore les quotes elles-mêmes
// 		}
// 		if ((*line == '"' && *(line + 1) == '"') || (*line == '\'' && *(line + 1) == '\''))	// Ignore les quotes vides
// 		{
// 			line += 2;
// 			continue;
// 		}
// 		if (is_whitespace(*line) && !in_quotes)// Gestion des espaces en dehors des quotes
// 		{
// 			if (i > 0) // Crée un token seulement si le buffer n'est pas vide
// 			{
// 				buffer[i] = '\0';
// 				add_token(&token_list, TOKEN_ARGUMENT, buffer);
// 				i = 0;
// 			}
// 			line++;
// 			continue;
// 		}
// 		buffer[i++] = *line++;// Ajout des caractères normaux au buffer
// 	}

// 	// Crée le dernier token si le buffer contient du texte
// 	if (i > 0)
// 	{
// 		buffer[i] = '\0';
// 		add_token(&token_list, TOKEN_ARGUMENT, buffer);
// 	}

// 	return token_list;
// }

t_token	*add_pipe_token(t_token **head, t_token **tail)
{
	t_token	*n_token;

	n_token = create_token(TOKEN_PIPE, "|");
	if (n_token)
	{
		if (!*head)
			*head = n_token;
		else
			(*tail)->next = n_token;
		*tail = n_token;
	}
	return (n_token);
}

int	process_token(t_token **head, t_token **tail, char *start, char *ptr,
		int first_token)
{
	char	*token_str;

	(void)tail;
	token_str = ft_strndup(start, ptr - start);
	if (!token_str)
		return (0);
	if (first_token)
		add_token(head, TOKEN_COMMAND, token_str);
	else
		add_token(head, TOKEN_ARGUMENT, token_str);
	free(token_str);
	return (0);
}

void	handle_token(t_token **head, t_token **tail, char **ptr,
		int *first_token)
{
	char	*start;

	start = *ptr;
	while (**ptr && **ptr != ' ' && **ptr != '|')
		(*ptr)++;
	process_token(head, tail, start, *ptr, *first_token);
	*first_token = 0;
}

int	handle_env_var(char **line, t_token **token_list)
{
	char	*start = *line;
	int		len = 1;

	// Vérifie si le caractère après `$` est valide pour une variable d'environnement
	if (!isalpha(start[1]) && start[1] != '_')
		return (0);

	// Avance après le caractère `$`
	(*line)++;
	while (isalnum(**line) || **line == '_')
	{
		(*line)++;
		len++;
	}

	// Extraire le nom de la variable sans `$`
	char *var_name = ft_strndup(start + 1, len - 1);
	if (!var_name)
		return (-1);

	// Cherche la valeur de la variable d'environnement
	char *value = getenv(var_name);
	if (value)
		add_token(token_list, TOKEN_ARGUMENT, value);

	free(var_name);
	return (1); // Indique qu'un token a été traité
}

// int	handle_env_var(char **ptr, t_token **head, t_token **tail, int *first_token)
// {
// 	(void)tail;
// 	char	*start = *ptr;
// 	int		len = 1;

// 	// Vérifier si le prochain caractère est une lettre ou un underscore
// 	if (!ft_isalpha(start[1]) && start[1] != '_')
// 		return (0); // Si ce n'est pas un nom valide de variable, on retourne 0
// 	(*ptr)++; // Avance au caractère suivant après le '$'
// 	while (ft_isalnum(**ptr) || **ptr == '_')
// 	{
// 		(*ptr)++;
// 		len++;
// 	}// Extraire le nom de la variable (sans le '$')
// 	char *var_name = ft_strndup(start + 1, len - 1);
// 	if (!var_name)
// 		return (-1);
// 	if (*first_token)// Ajouter un token pour cette variable d'environnement
// 		add_token(head, TOKEN_COMMAND, var_name);
// 	else
// 		add_token(head, TOKEN_ARGUMENT, var_name);
// 	free(var_name);
// 	*first_token = 0;
// 	return (1); // Retourne 1 pour indiquer qu'on a traité un token de variable
// }

t_token	*lexer(const char *input)
{
	t_token	*head = NULL;
	t_token	*tail = NULL;
	char	*ptr = ft_strdup(input);
	int		first_token = 1;
	int		in_single_quotes = 0;
	int		in_double_quotes = 0;

	if (!ptr)
		return (NULL);

	while (*ptr)
	{
		// Gestion des simples quotes
		if (*ptr == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			ptr++;
			continue;
		}

		// Gestion des doubles quotes
		if (*ptr == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			ptr++;
			continue;
		}

		// // Appelle `handle_env_var` seulement si dans double quotes ou en dehors de toute quote
		// if (*ptr == '$' && !in_single_quotes)
		// {
		// 	if (handle_env_var(&ptr, &head))
		// 		continue;
		// }

		// Gestion des autres tokens
		handle_token(&head, &tail, &ptr, &first_token);
	}

	free(ptr);
	return head;
}


// t_token	*lexer(const char *input)
// {
// 	t_token	*head = NULL;
// 	t_token	*tail = NULL;
// 	char	*ptr = ft_strdup(input);
// 	int		first_token = 1;

// 	if (!ptr)
// 		return (NULL);

// 	while (*ptr)
// 	{
// 		if (handle_space(&ptr))
// 			continue;
// 		if (*ptr == '|')
// 		{
// 			if (!create_token_from_pipe(&head, &tail))
// 				return (free(ptr), NULL);
// 			first_token = 1;
// 			ptr++;
// 			continue;
// 		}
// 		if (*ptr == '$' && handle_env_var(&ptr, &head, &tail, &first_token))
// 			continue;
// 		handle_token(&head, &tail, &ptr, &first_token);
// 	}
// 	return (free(ptr), head);
// }


t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		perror("malloc failed");
		return (NULL);
	}
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	return (new_token);
}

int	add_token(t_token **head, t_token_type type, const char *value)
{
	t_token	*new_token;
	t_token	*current;

	new_token = create_token(type, value);
	if (!new_token)
		return (0);
	if (!*head)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
	return (0);
}

t_token_type	get_token_type(const char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(str, "<") == 0)
		return (TOKEN_REDIRECT_INPUT);
	if (ft_strcmp(str, ">") == 0)
		return (TOKEN_REDIRECT_OUTPUT);
	if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_REDIRECT_APPEND);
	if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	return (TOKEN_ARGUMENT);
}

int	finalize_tokens(int in_quotes, char quote_char, char *buffer, int *i,
		t_token **token_list)
{
	if (in_quotes)
	{
		fprintf(stderr, "Syntax error : unmatched %c\n", quote_char);
		return (-1);
	}
	if (*i > 0)
	{
		buffer[*i] = '\0';
		add_token(token_list, TOKEN_ARGUMENT, buffer);
	}
	return (0);
}


void	handle_child(t_token *cmd_tokens, int fd_in, int pipe_fd[2],
		int last_cmd)
{
	if (fd_in != 0)
		dup2(fd_in, STDIN_FILENO);
	if (!last_cmd)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	exec_cmd(cmd_tokens, fd_in, pipe_fd, last_cmd);
}

void	handle_parent(int pipe_fd[2], int *fd_in, pid_t pid)
{
	close(pipe_fd[1]);
	waitpid(pid, NULL, 0);
	if (*fd_in != 0)
		close(*fd_in);
	*fd_in = pipe_fd[0];
}


int handle_line(char *line, char **env, t_ctx *ctx)
{
    t_token *tokens;

    add_history(line);
    tokens = parse_command_line(line);
    if (tokens)
    {
        if (validate_pipes(tokens) == -1)
        {
            free_tokens(tokens);
            return 0;
        }
        // print_tokens(tokens);
        if (contains_pipe(tokens))
            process_pline(tokens, env);
        else
            exec_simple_cmd(tokens, env, ctx); // Passer le contexte ici
        free_tokens(tokens);
    }
    return 0;
}

int	ft_cd(char **args)
{
	static char	*oldpwd = NULL;

	if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
		ft_cd_home();
	else if (ft_strcmp(args[1], "-") == 0)
		ft_cd_oldpwd(&oldpwd);
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("cd");
			return (0);
		}
	}
	ft_update_pwd(&oldpwd);
	return (0);
}

int	ft_cd_home(void)
{
	char	*home;

	home = getenv("HOME");
	if (home == NULL)
	{
		fprintf(stderr, "cd: HOME not set\n");
		return (0);
	}
	if (chdir(home) != 0)
		perror("cd");
	return (0);
}

int	ft_cd_oldpwd(char **oldpwd)
{
	if (*oldpwd == NULL)
	{
		fprintf(stderr, "cd: OLDPWD not set\n");
		return (0);
	}
	if (chdir(*oldpwd) != 0)
	{
		perror("cd");
		return (0);
	}
	printf("%s\n", *oldpwd);
	return(0);
}

int	ft_update_pwd(char **oldpwd)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (*oldpwd)
		free(*oldpwd);
	if (cwd != NULL)
	{
		*oldpwd = strdup(cwd);
		printf("Current directory: %s\n", cwd);
		free(cwd);
	}
	else
	{
		perror("getcwd() error");
		*oldpwd = NULL;
	}
	return(0);
}

void	print_all_cmds(t_token *cmd_tokens, int num_pipes)
{
	int		cmd_index = 0;
	t_token	*current;

	printf("Number of pipes: %d\n", num_pipes);

	while (cmd_index <= num_pipes)
	{
		printf(" Command %d: ", cmd_index);
		current = cmd_tokens;

		// Parcours les tokens pour chaque commande jusqu'à un pipe ou la fin
		while (current && current->type != TOKEN_PIPE)
		{
			printf("%s ", current->value);
			current = current->next;
		}
		printf("\n");

		// Passe au prochain token après le pipe
		if (current && current->type == TOKEN_PIPE)
			current = current->next;

		cmd_tokens = current;
		cmd_index++;
	}
}


void	create_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("Echec de pipe");
		exit(EXIT_FAILURE);
	}
}

char	**prepare_print_args(t_token *cmd)
{
	char	**args;
	int		k;
	int exit_status = 0;

	k = 0;
	args = prepare_args(cmd, &exit_status);
	if (args == NULL || args[0] == NULL)
	{
		fprintf(stderr, "Erreur lors de la preparation des args\n");
		exit(EXIT_FAILURE);
	}
	printf("Execution de la commande : %s\n", args[0]);
	while (args[k] != NULL)
	{
		printf("Arguments %d : %s\n", k, args[k]);
		k++;
	}
	return (args);
}


void	exec_cmd(t_token *cmd, int fd_in, int pipe_fd[2], int last_cmd)
{
	char	**args;

	args = prepare_print_args(cmd);
	if (fd_in != 0)
		dup2(fd_in, STDIN_FILENO);
	if (!last_cmd)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	if (execvp(args[0], args) == -1)
	{
		perror("Échec de exec");
		free(args);
		exit(EXIT_FAILURE);
	}
}

void	process_tokens(t_token *cmd_tokens, int num_pipes)
{
	int		pipe_fd[2];
	int		fd_in;
	int		j;
	pid_t	pid;

	fd_in = 0;
	j = 0;
	print_all_cmds(cmd_tokens, num_pipes);
	while (j <= num_pipes)
	{
		if (j < num_pipes)
			create_pipe(pipe_fd);
		pid = fork();
		if (pid == -1)
		{
			perror("Echec de fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
			handle_child(&cmd_tokens[j], fd_in, pipe_fd, j == num_pipes);
		else
			handle_parent(pipe_fd, &fd_in, pid);
		j++;
	}
}


int count_tokens(t_token *tokens)
{
    int count = 0;

	while(tokens)
	{
		count++;
		tokens = tokens->next;
	}
    return (count);
}

int exec_simple_cmd(t_token *tokens, char **env, t_ctx *ctx)
{
    char **args;
    pid_t pid;

    // 1. Étendre les variables d'environnement dans chaque token

	//ICI
    // ps_expand_env(tokens, ctx); // Passer les tokens à ps_expand_env pour traiter l'expansion
    // 2. Préparer les arguments après l'expansion
    args = prepare_args(tokens, &ctx->exit_status); // Passer les tokens ici, pas une chaîne de caractères
    if (!args) {
        perror("Erreur d'allocation de mémoire pour les arguments");
        return 0;
    }
    // 3. Vérifier si la commande est une commande interne (builtin)
    if (exec_builtin_cmd(args, env)) {
        free(args);
        return 0;
    }
    // 4. Fork pour exécuter la commande externe
    pid = fork();
    if (pid == -1) {
        perror("Echec fork");
        free(args);
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
	{
        // Processus enfant: exécuter la commande
        exec(tokens, env); // Assurez-vous que cette fonction exécute correctement `execve`
        free(args); // Libérer les arguments dans le processus enfant aussi
        exit(0);
    }
    // 5. Processus parent: attendre la fin de la commande
    waitpid(pid, NULL, 0);
    free(args); // Libérer les arguments après exécution
	return 0;
}


void split_env_v(const char *input, char **var, char **value)
{
	const char *equal_sign;

	equal_sign = ft_strchr(input, '=');
	if(equal_sign)
	{
		size_t	var_len = equal_sign - input;
		*var = ft_strndup(input, var_len);
		*value = ft_strdup(equal_sign + 1); //apres le =
	}
	else
	{
		*var = ft_strdup(input);
		*value = NULL;
	}
}

int exec_builtin_cmd(char **args, char **env)
{
	t_ctx *ctx = NULL;

	if(ft_strcmp(args[0], "export") == 0 && args[1])
	{
		char *var = NULL;
		char *value = NULL;
		split_env_v(args[1], &var, &value);
		if(var && is_valid_id(var))
		{
			if(value)
				export_v(&env, var, value);
			else
				export_v(&env, var, "");
		}
		else
			fprintf(stderr, "not good caracter %s\n", args[1]);
		ctx->exit_status = 0;
		return (free(var), free(value), 1);
	}
	if(ft_strcmp(args[0], "unset") == 0 && args[1])
		return (unset_v(&env, args[1]), 1);
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args), 1);
	return (0);
}


int read_and_exec(char **env)
{
    char *line;
    t_ctx ctx;
    ctx.exit_status = 0;

    line = readline(PROMPT);
    if (line == NULL) {
        write(1, "exit\n", 5);
        exit(0);
    }

    if (ft_strncmp(line, "exit", 4) == 0 && (line[4] == '\0' || line[4] == ' ')) {
        write(1, "exit\n", 5);
        exit(ctx.exit_status);
    }

    if (*line)
        handle_line(line, env, &ctx);
    free(line);
    return ctx.exit_status;
}


int	loop(char **env)
{
	// int exit_status = 0;

	while (1)
		read_and_exec(env);
	return (0);
}

int	valide_pipes(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (!current || current->type == TOKEN_PIPE)
		return (fprintf(stderr,
				"minibg : syntax error near unexpected token `|'\n"), -1);
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next || current->next->type == TOKEN_PIPE)
				return (fprintf(stderr,
						"minibg : syntax error near unexpected token `|'\n"),
					-1);
		}
		current = current->next;
	}
	return (0);
}

int	exc_error(t_token *tokens)
{
	int		pipefd[2];
	pid_t	pid1;

	(void)tokens;
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return 0;
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork failed");
		return 0;
	}
	return 0;
}

int	exc_pipe(t_token *tokens)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;
	char	*grep_args;
	t_token	*current;

	grep_args = NULL;
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return (0);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork failed");
		return (0);
	}
	if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		char *args[] = {"/bin/ls", NULL};
		execve(args[0], args, NULL);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork failed");
		return (0);
	}
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		current = tokens;
		while (current)
		{
			if (strcmp(current->value, "grep") == 0 && current->next)
			{
				grep_args = current->next->value;
				break ;
			}
			current = current->next;
		}
		if (grep_args)
		{
			char *args[] = {"/bin/grep", grep_args, NULL};
			execve(args[0], args, NULL);
		}
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	return(0);
}


t_token	*extract_command(t_token *tokens)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new_t;

	head = 0;
	tail = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		// t_token *new_t = malloc(sizeof(t_token));
		// new_t->type = tokens->type;
		// new_t->value = strdup(tokens->value);
		// new_t->next = NULL;
		new_t = malloc(sizeof(t_token));
		if (!new_t)
		{
			// Libérer la mémoire déjà allouée dans cette fonction si nécessaire
			perror("malloc failed");
			exit(EXIT_FAILURE);
		}
		new_t->value = strdup(tokens->value);
		if (!new_t->value)
		{
			free(new_t);
			perror("strdup failed");
			exit(EXIT_FAILURE);
		}
		if (!head)
		{
			head = new_t;
			tail = new_t;
		}
		else
		{
			tail->next = new_t;
			tail = new_t;
		}
		tokens = tokens->next;
	}
	return (head); // liste de tokens avant le pipe
}

t_token	*extract_command_after(t_token *tokens)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new_t;

	head = NULL;
	tail = NULL;
	while (tokens && tokens->type != TOKEN_PIPE)
		tokens = tokens->next;
	if (tokens && tokens->type == TOKEN_PIPE)
		tokens = tokens->next;
	while (tokens) // apres pipe
	{
		// t_token *new_t = create_token(tokens->type, tokens->value);
		new_t = malloc(sizeof(t_token));
		new_t->type = tokens->type;
		new_t->value = strdup(tokens->value);
		new_t->next = NULL;
		if (!head)
		{
			head = new_t;
			tail = new_t;
		}
		else
		{
			tail->next = new_t;
			tail = new_t;
		}
		tokens = tokens->next;
	}
	return (head); // liste de tokens avant le pipe
}

int	process_pline(t_token *tokens, char **env)
{
	int		pipe_fd[2];
	int		status;
	t_token	*first_cmd_t;
	t_token	*second_cmd_t;

	pid_t pid1, pid2;
	if (pipe(pipe_fd) == -1)
		exit_error();
	pid1 = fork();
	if (pid1 == -1)
		exit_error();
	if (pid1 == 0) // Processus enfant 1 (avant le pipe)
	{
		first_cmd_t = extract_command(tokens);
		child(first_cmd_t, pipe_fd, env);
	}
	pid2 = fork();
	if (pid2 == -1)
		exit_error();
	if (pid2 == 0) // Processus enfant 2 (après le pipe)
	{
		second_cmd_t = extract_command_after(tokens);
		parent(second_cmd_t, pipe_fd, env);
	}
	// Fermer les deux extrémités du pipe dans le processus parent
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	// Attendre la fin des deux processus enfants
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	return (0);
}


void	exit_error(void)
{
	perror("Error");
	exit(EXIT_FAILURE);
}

void	*free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	return (NULL);
}

char	*find_in_env(char *name, char **env)
{
	int	i;
	int	len;

	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*join_path_cmd(char *path, char *cmd)
{

    char *full_path;
    char *path_with_slash = malloc(strlen(path) + 2);

    if (!path_with_slash)
        return (NULL);
    sprintf(path_with_slash, "%s/", path);
    full_path = malloc(strlen(path_with_slash) + strlen(cmd) + 1);

    if (!full_path)
    {
        free(path_with_slash);
        return (NULL);
    }

    sprintf(full_path, "%s%s", path_with_slash, cmd);
    free(path_with_slash);
    return (full_path);
}

char *get_path(char *cmd, char **env)
{
    char **paths;
    char *full_path;
    int i;
    // printf("%s\n", cmd);
    // Extraire uniquement la commande sans les arguments
    char **split_cmd = ft_split(cmd, ' ');  // Séparer les mots de la commande
    char *command_only = split_cmd[0];      // Prendre seulement la commande

    paths = ft_split(find_in_env("PATH", env), ':');
    i = 0;
    while (paths[i])
    {
        full_path = join_path_cmd(paths[i], command_only);  // Chercher uniquement la commande
        // printf("Checking path: %s\n", full_path);  // Debug pour vérifier les chemins testés
        if (access(full_path, F_OK | X_OK) == 0)
        {
            free_tab(paths);
            free_tab(split_cmd);
            return full_path;  // Retourner le chemin complet si la commande est trouvée
        }
        free(full_path);
        i++;
    }
    free_tab(paths);
    free_tab(split_cmd);
    return command_only;  // Si aucun chemin n'est trouvé, retourner la commande brute
}


char **prepare_args(t_token *tokens, int *exit_status)
{
	(void)exit_status;
    int count = 0;
    t_token *current = tokens;
    char **args;
    int i = 0;

    // Compter les arguments jusqu'au prochain pipe
    while (current && current->type != TOKEN_PIPE)
    {
        count++;
        current = current->next;
    }

    args = malloc((count + 1) * sizeof(char *));
    if (!args)
    {
        perror("malloc failed");
        return NULL;
    }

    current = tokens;
    i = 0;
    while (current && current->type != TOKEN_PIPE)
    {
        args[i] = strdup(current->value); // Dupliquer la valeur après expansion
        if (!args[i])
        {
            perror("Duplication de mémoire échouée");
            for (int j = 0; j < i; j++)
                free(args[j]);
            free(args);
            return NULL;
        }
        i++;
        current = current->next;
    }
    args[i] = NULL;  // Ajouter NULL à la fin pour `execve`
    return args;
}


int	exec(t_token *cmd_tokens, char **env)
{
	char	**option_cmd;
	char	*path;
	int exit_status = 0;

	option_cmd = prepare_args(cmd_tokens, &exit_status);
	if (!option_cmd[0])
	{
		fprintf(stderr, "Error: Command is empty\n");
		free_tab(option_cmd);
		exit(EXIT_FAILURE);
	}
	if (strcmp(option_cmd[0], "cd") == 0)
	{
		ft_cd(option_cmd);
		free_tab(option_cmd);
		return (0);
	}
	path = get_path(option_cmd[0], env);
	if (execve(path, option_cmd, env) == -1)
	{
		perror("exec command");
		free_tab(option_cmd);
		exit(EXIT_FAILURE);
	}
	free_tab(option_cmd);
	return (0);
}

void	child(t_token *tokens, int *pipe_fd, char **env)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	exec(tokens, env);
}

void	parent(t_token *tokens, int *pipe_fd, char **env)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	exec(tokens, env);
}

int	validate_pipes(t_token *tokens)
{
	t_token	*current = tokens;

	// Vérifie si le premier token est un pipe ou si la liste est vide
	if (!current || current->type == TOKEN_PIPE)
	{
		fprintf(stderr, "Error: syntax error near unexpected token `|`\n");
		return (-1);
	}

	while (current)
	{
		// Vérifie les pipes consécutifs et les commandes manquantes
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next || current->next->type == TOKEN_PIPE)
			{
				fprintf(stderr, "Error: syntax error near unexpected token `|`\n");
				return (-1);
			}
		}
		current = current->next;
	}
	return (0); // Aucun problème trouvé
}


int	contains_pipe(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}


int		g_var_global = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_var_global = 1;
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	init_sig(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

int	main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
		char **envp)
{
	char	**env_copy;

	init_sig();
	env_copy = get_environment(envp);
	if (!env_copy)
		return (fprintf(stderr, "Failed to copy environment\n"), 1);
	loop(env_copy);
	free_tab(env_copy);
	return (0);
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: '%s'\n", tokens->value);
		tokens = tokens->next;
	}
}