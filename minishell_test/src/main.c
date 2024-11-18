/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:03:19 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/19 00:18:08 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env_var *create_env_var(const char *env_entry)
{
    t_env_var *new_var = (t_env_var *)malloc(sizeof(t_env_var));
    if (!new_var)
        return (perror("malloc failed"), NULL);

    char *sep = ft_strchr(env_entry, '=');
    if (!sep)
    {
        new_var->name = strdup(env_entry);
        new_var->value = NULL;
    }
    else
    {
        new_var->name = strndup(env_entry, sep - env_entry); // Nom avant le '='
        new_var->value = strdup(sep + 1); // Valeur après le '='
    }
    if (!new_var->name || (sep && !new_var->value))
    {
        free(new_var->name);
        free(new_var->value);
        free(new_var);
        return (perror("malloc failed"), NULL);
    }
    new_var->next = NULL;
    return new_var;
}

t_env_var *get_environment(char **envp)
{
    if (!envp)
        return NULL;

    t_env_var *head = NULL;
    t_env_var *current = NULL;

    for (int i = 0; envp[i]; i++)
    {
        t_env_var *new_var = create_env_var(envp[i]);
        if (!new_var)
        {
            while (head)
            {
                t_env_var *temp = head;
                head = head->next;
                free(temp->name);
                free(temp->value);
                free(temp);
            }
            return NULL;
        }

        if (!head)
            head = new_var;
        else
            current->next = new_var;
        current = new_var;
    }
    return head;
}

char **copy_envp(char **envp)
{
    int i = 0;
    while (envp[i])
        i++;
    char **env_copy = malloc((i + 1) * sizeof(char *));
    if (!env_copy)
        return NULL;
    for (int j = 0; j < i; j++)
        env_copy[j] = ft_strdup(envp[j]);
    env_copy[i] = NULL;
    return env_copy;
}



// char	**get_environment(char **envp)
// {
// 	int		i;
// 	int		count;
// 	char	**env_copy;

// 	i = 0;
// 	count = 0;
// 	while (envp[count])
// 		count++;
// 	env_copy = (char **)malloc((count + 1) * sizeof(char *));
// 	if (!env_copy)
// 		return (perror("malloc failed"), NULL);
// 	while (i < count)
// 	{
// 		env_copy[i] = strdup(envp[i]);
// 		if (!env_copy[i])
// 		{
// 			perror("strdup failed");
// 			while (--i >= 0)
// 				free(env_copy[i]);
// 			return (free(env_copy), NULL);
// 		}
// 		i++;
// 	}
// 	return (env_copy[i] = NULL, env_copy);
// }

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
            char *value_joined = value ? ft_strjoin(new_var, value) : strdup(new_var); // Gérer valeur NULL

            // Met à jour la variable dans l'environnement
            (*env_copy)[i] = value_joined;
            free(new_var); // Libère la mémoire de la chaîne intermédiaire
            return 0;
        }
        i++;
    }
    // Si la variable n'existe pas, ajoute-la
    new_var = ft_strjoin(var, "=");
    char *new_entry = value ? ft_strjoin(new_var, value) : strdup(new_var);
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


// int export_v(char ***env_copy, const char *var, const char *value)
// {
//     int i;
//     size_t len;
//     char *new_var;

//     i = 0;
//     len = strlen(var);

//     // Recherche si la variable existe déjà
//     while ((*env_copy)[i]) {
//         if (ft_strncmp_export((*env_copy)[i], var, len) == 0 && (*env_copy)[i][len] == '=') {
//             // Libère l'ancienne valeur et crée une nouvelle variable
//             free((*env_copy)[i]);
//             new_var = ft_strjoin(var, "="); // Crée une nouvelle chaîne de base
//             char *value_joined = ft_strjoin(new_var, value); // Concatène la valeur
//             // Met à jour la variable dans l'environnement
//             (*env_copy)[i] = value_joined;
//             free(new_var); // Libère la mémoire de la chaîne intermédiaire
//             return 0;
//         }
//         i++;
//     }
//     // Si la variable n'existe pas, ajoute-la
//     new_var = ft_strjoin(var, "=");
//     char *new_entry = ft_strjoin(new_var, value);
//     free(new_var); // Libère la mémoire de la chaîne intermédiaire

//     // Réalloue l'espace pour ajouter une nouvelle variable
//     size_t current_size = i; // Nombre actuel d'éléments
//     *env_copy = realloc(*env_copy, (current_size + 2) * sizeof(char *));
//     if (!*env_copy) {
//         perror("realloc failed");
//         return -1; // Erreur de réallocation
//     }

//     (*env_copy)[current_size] = new_entry; // Ajoute la nouvelle entrée
//     (*env_copy)[current_size + 1] = NULL; // Termine avec NULL
//     return 0;
// }


int unset_v(t_env_var **env_vars, const char *var)
{
    t_env_var *current = *env_vars;
    t_env_var *prev = NULL;

    if (!env_vars || !var)
        return -1;

    while (current)
    {
        if (ft_strcmp(current->name, var) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env_vars = current->next;

            free(current->name);
            free(current->value);
            free(current);
            return 0; // Successfully unset
        }
        prev = current;
        current = current->next;
    }
    return -1; // Variable not found
}





// int unset_v(t_env_var **env_vars, const char *var)
// {
//     t_env_var *current = *env_vars;
//     t_env_var *prev = NULL;

//     if (!env_vars || !var)
//     {
//         fprintf(stderr, "unset_v: NULL pointer\n");
//         return -1;
//     }
//     while (current)
//     {
//         if (strcmp(current->name, var) == 0)
//         {
//             if (prev)
//                 prev->next = current->next;
//             else
//                 *env_vars = current->next;
//             free(current->name);
//             free(current->value);
//             free(current);
//             return 0; // Successfully unset
//         }
//         prev = current;
//         current = current->next;
//     }
//     return -1; // Variable not found
// }




// int unset_v(char ***env_copy, const char *var)
// {
//     int i, j;
//     size_t len;

//     if (!env_copy || !*env_copy || !var)
//     {
//         // ctx->exit_status = 1; // Erreur sur les paramètres
// 		fprintf(stderr, "unset_v: NULL pointer\n");
//         return (-1);
//     }
//     len = ft_strlen(var);
//     for (i = 0; (*env_copy)[i]; i++)
//     {
//         if (ft_strncmp_export((*env_copy)[i], var, len) == 0 && (*env_copy)[i][len] == '=')
//         {
//             free((*env_copy)[i]); // Libérer la variable trouvée
//             for (j = i; (*env_copy)[j]; j++)
//                 (*env_copy)[j] = (*env_copy)[j + 1]; // Décaler les éléments suivants
//             // ctx->exit_status = 0; // Succès
//             return (0);
//         }
//     }
//     return (0);
// }


// int unset_v(char ***env_copy, const char *var)
// {
// 	int i;
// 	size_t len;

// 	i = 0;
// 	len = ft_strlen(var);

// 	while((*env_copy)[i++])
// 	{
// 		if(ft_strncmp_export((*env_copy)[i], var, len) == 0 && (*env_copy)[i][len] == '=')
// 		{
// 			free((*env_copy)[i]);
// 			while((*env_copy)[i + 1])
// 			{
// 				(*env_copy)[i] = (*env_copy)[i + 1];
// 				i++;
// 			}
// 			(*env_copy)[i] = NULL;
// 			return (0);
// 		}
// 		//i++;
// 	}
// 	return(0);
// }

// Fonction pour obtenir la partie avant le caractère `$`
char *ps_get_before_env(char *str, char *found)
{
    return (ft_strndup(str, found - str)); // Copie jusqu'au `$`
}

// Fonction pour obtenir la valeur de la variable d'environnement ou une chaîne vide si elle n'existe pas
char *ps_get_env_var(char *var_name, t_ctx *ctx)
{
    if (strcmp(var_name, "?") == 0)
        return ft_itoa(ctx->exit_status);

    char *env_value = find_env_value(var_name, ctx->env_vars);
    return env_value ? strdup(env_value) : strdup("");
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

// int ps_handle_env(t_token *token, t_ctx *ctx)
// {
//     char *tmp;
//     int nb;
// 	char *line;
// 	char *new;

// 	nb = ft_count_exp(token->value);
//     while (token->value && nb > 0)
// 	{
//         tmp = token->value;
//         line = token->value; // La ligne commence au début de la valeur actuelle
//         t_token *token_list = NULL;
//         if (handle_env_var(&line, &token_list, ctx) == -1)
//             return (free(tmp), -1);
//         new = NULL;
//         if (token_list)
// 		{
//             new = ft_strdup(token_list->value); // Copier la valeur du token généré
//             free_tokens(token_list);           // Libérer la liste temporaire
//         }
//         token->value = new && new[0] == '\0' ? NULL : new; // Assigner la nouvelle valeur
//         free(tmp);
//         nb--;
//     }
//     return 0;
// }

char *find_env_value(const char *name, t_env_var *env_vars)
{
    while (env_vars)
    {
        if (strcmp(env_vars->name, name) == 0)
            return env_vars->value;
        env_vars = env_vars->next;
    }
    return NULL; // Retourne NULL si aucune correspondance n'est trouvée
}


// void ps_expand_env(t_token *tokens, t_ctx *ctx)
// {
//     (void)ctx;

//     while (tokens) {
//         char *token = tokens->value;
//         char *result = NULL;

//         for (int i = 0; token[i] != '\0'; i++)
//         {
//             if (token[i] == '$') 
// 			{
//                 // Si le prochain caractère est un guillemet, on ajoute `$` comme une chaîne et on ignore l'expansion
//                 if (token[i + 1] == '"') {
//                     char single_char[2] = {'$', '\0'};
//                     char *tmp = result;
//                     result = ft_strjoin(result ? result : "", single_char);
//                     free(tmp);
//                     continue; // Passez au caractère suivant
//                 }
//                 char *var_start = &token[i + 1];
//                 int j = 0;
//                 while (ft_isalpha(var_start[j]) || var_start[j] == '_')
//                     j++;
//                 if (j > 0)
//                 {
//                     char var_name[j + 1];
//                     strncpy(var_name, var_start, j);
//                     var_name[j] = '\0';
//                     // char *env_value = getenv(var_name);
// 					char *env_value = find_env_value(var_name, ctx->env_vars);
//                     if (env_value)
//                     {
//                         char *tmp = result;
//                         result = ft_strjoin(result ? result : "", env_value);
//                         free(tmp);
//                     }
//                     i += j;
//                 }
//             }
//             else
//             {
//                 // Ajoutez les autres caractères directement dans la chaîne de résultat
//                 char single_char[2] = {token[i], '\0'};
//                 char *tmp = result;
//                 result = ft_strjoin(result ? result : "", single_char);
//                 free(tmp);
//             }
//         }
//         free(tokens->value);
//         tokens->value = result;
//         tokens = tokens->next;
//     }
// }


int	ft_strncmp_export(const char *s1, const char *s2, unsigned int n)
{
	unsigned int	i;

	if(!s1 || !s2)
		return (-1);
	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] && s2[i] && n - 1 > i)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

// int	is_valid_id(const char *var)
// {
// 	int	i;

// 	i = 1;
// 	if (!var || !*var)
// 		return (0);
// 	if (!ft_isalpha(var[0]) && var[0] != '_')
// 		return (0);
// 	while (var[i] != '\0')
// 	{
// 		if (!ft_isalnum(var[i]) && var[i] != '_')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

int is_valid_id(const char *var)
{
    if (!var || (!ft_isalpha(var[0]) && var[0] != '_'))
        return 0;

    for (int i = 1; var[i]; i++)
    {
        if (!ft_isalnum(var[i]) && var[i] != '_')
            return 0;
    }
    return 1;
}

char *expand_variables(const char *str, t_ctx *ctx, t_token_type token_type)
{
    if (token_type == SINGLE_QUOTE)
        return ft_strdup(str); // Return as-is for single-quoted strings

    char *expanded = ft_strdup("");
    char *temp;

    while (*str)
    {
        if (*str == '$')
        {
            str++; // Skip the '$'
            if (*str == '\0') // Standalone '$'
            {
                temp = ft_strjoin(expanded, "$");
                free(expanded);
                expanded = temp;
            }
            else if (*str == '?') // Special case for `$?`
            {
                char *exit_code = ft_itoa(ctx->exit_status);
                temp = ft_strjoin(expanded, exit_code);
                free(expanded);
                expanded = temp;
                free(exit_code);
                str++;
            }
            else if (ft_isalnum(*str) || *str == '_') // Valid variable name
            {
                char buffer[1024];
                int i = 0;

                while (ft_isalnum(*str) || *str == '_')
                    buffer[i++] = *str++;
                buffer[i] = '\0';

                char *value = find_env_value(buffer, ctx->env_vars);
                temp = ft_strjoin(expanded, value ? value : "");
                free(expanded);
                expanded = temp;
            }
            else // Invalid variable, treat as literal
            {
                temp = ft_strjoin(expanded, "$");
                free(expanded);
                expanded = temp;
            }
        }
        else
        {
            char char_as_str[2] = {*str, '\0'};
            temp = ft_strjoin(expanded, char_as_str);
            free(expanded);
            expanded = temp;
            str++;
        }
    }

    return expanded;
}


// char *expand_variables(const char *str, t_ctx *ctx, t_token_type token_type)
// {
//     if (token_type == SINGLE_QUOTE)
//         return ft_strdup(str); // No expansion for single-quoted strings

//     char *expanded = ft_strdup("");
//     char *temp;

//     while (*str)
//     {
//         if (*str == '$')
//         {
//             str++; // Skip the '$'
//             if (*str == '\0') // Standalone '$'
//             {
//                 temp = ft_strjoin(expanded, "$");
//                 free(expanded);
//                 expanded = temp;
//             }
//             else if (*str == '?') // Special case for `$?`
//             {
//                 char *exit_code = ft_itoa(ctx->exit_status);
//                 temp = ft_strjoin(expanded, exit_code);
//                 free(expanded);
//                 expanded = temp;
//                 free(exit_code);
//                 str++;
//             }
//             else if (ft_isalnum(*str) || *str == '_') // Valid variable name
//             {
//                 char buffer[1024];
//                 int i = 0;

//                 while (ft_isalnum(*str) || *str == '_')
//                     buffer[i++] = *str++;
//                 buffer[i] = '\0';

//                 char *value = find_env_value(buffer, ctx->env_vars);
//                 temp = ft_strjoin(expanded, value ? value : "");
//                 free(expanded);
//                 expanded = temp;
//             }
//             else // Invalid variable, treat as literal
//             {
//                 temp = ft_strjoin(expanded, "$");
//                 free(expanded);
//                 expanded = temp;
//             }
//         }
//         else
//         {
//             char char_as_str[2] = {*str, '\0'};
//             temp = ft_strjoin(expanded, char_as_str);
//             free(expanded);
//             expanded = temp;
//             str++;
//         }
//     }

//     return expanded;
// }


void write_echo_content(t_token *token_list, int n_option, t_ctx *ctx)
{
    t_token *current = token_list;
    int first_arg = 1;

    while (current)
    {
        if (current->value && current->value[0] != '\0') // Ignore empty arguments
        {
            char *output = NULL;

            if (current->type == SINGLE_QUOTE)
            {
                output = ft_strdup(current->value); // Treat single quotes as literal
            }
            else if (current->type == DOUBLEQUOTE)
            {
                output = expand_variables(current->value, ctx, DOUBLEQUOTE); // Expand inside double quotes
            }
            else
            {
                output = expand_variables(current->value, ctx, STRING); // Expand normally
            }

            if (!output)
                output = ft_strdup(""); // Fallback if expansion fails

            if (!first_arg)
                write(STDOUT_FILENO, " ", 1); // Add space between arguments
            write(STDOUT_FILENO, output, ft_strlen(output));
            free(output);
            first_arg = 0;
        }
        current = current->next;
    }

    if (!n_option)
        write(STDOUT_FILENO, "\n", 1); // Add newline unless -n is specified
}


// void write_echo_content(t_token *token_list, int n_option, t_ctx *ctx)
// {
//     t_token *current = token_list;
//     int first_arg = 1;

//     while (current)
//     {
//         if (current->value && current->value[0] != '\0') // Ignore les arguments vides
//         {
//             char *output = NULL;

//             if (current->type == SINGLE_QUOTE)
//             {
//                 output = ft_strdup(current->value); // Aucune expansion pour les quotes simples
//             }
//             else if (current->type == DOUBLEQUOTE)
//             {
//                 output = expand_variables(current->value, ctx, DOUBLEQUOTE); // Expansion pour les quotes doubles
//             }
//             else
//             {
//                 output = expand_variables(current->value, ctx, STRING); // Expansion normale
//             }

//             if (!output)
//                 output = ft_strdup(""); // Fallback si l'expansion échoue
//             if (!first_arg)
//                 write(STDOUT_FILENO, " ", 1);
//             write(STDOUT_FILENO, output, ft_strlen(output));
//             free(output);
//             first_arg = 0;
//         }
//         current = current->next;
//     }
//     if (!n_option)
//         write(STDOUT_FILENO, "\n", 1); // Ajoute un saut de ligne sauf si "-n"
// }

void handle_echo(t_token *token_list, t_ctx *ctx)
{
    int n_option = 0;

    // Vérifie si le premier argument est "-n"
    if (token_list && ft_strcmp(token_list->value, "-n") == 0)
    {
        n_option = 1;
        token_list = token_list->next; 
    }
    write_echo_content(token_list, n_option, ctx);
	ctx->exit_status = 0;
}


// int	handle_quotes(char **line, int *in_quotes, char *quote_char, char *buffer,
// 		int *i, t_token **token_list)
// {
// 	if (**line == '"' || **line == '\'')
// 	{
// 		if (*in_quotes && **line == *quote_char)
// 		{
// 			*in_quotes = 0;
// 			*quote_char = 0;
// 			if (*i > 0)  // Vérifie que le buffer contient des caractères avant d'ajouter le token
// 			{
// 				buffer[*i] = '\0';
// 				add_token(token_list, TOKEN_ARGUMENT, buffer);
// 				*i = 0;
// 			}
// 		}
// 		else if (!*in_quotes)
// 		{
// 			*in_quotes = 1;
// 			*quote_char = **line;
// 		}
// 		(*line)++;
// 		return (1);
// 	}
// 	return (0);
// }

// int	handle_whitespace(char **line, char *buffer, int *i, t_token **token_list,
// 		int in_quotes)
// {
// 	if (is_whitespace(**line) && !in_quotes)
// 	{
// 		if (*i > 0)
// 		{
// 			buffer[*i] = '\0';
// 			add_token(token_list, TOKEN_ARGUMENT, buffer);
// 			*i = 0;
// 		}
// 		(*line)++;
// 		return (1);
// 	}
// 	return (0);
// }

// int	handle_special_chars(char **line, char *buffer, int *i,
// 		t_token **token_list, int in_quotes)
// {
// 	char	special[3];

// 	ft_bzero(special, sizeof(special));
// 	if ((**line == '|' || **line == '<' || **line == '>') && !in_quotes)
// 	{
// 		if (*i > 0)
// 		{
// 			buffer[*i] = '\0';
// 			add_token(token_list, TOKEN_ARGUMENT, buffer);
// 			*i = 0;
// 		}
// 		special[0] = **line;
// 		(*line)++;
// 		if ((special[0] == '>' && **line == '>') || (special[0] == '<'
// 				&& **line == '<'))
// 			special[1] = *(*line)++;
// 		add_token(token_list, get_token_type(special), special);
// 		return (1);
// 	}
// 	return (0);
// }

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

// int	handle_space(char **ptr)
// {
// 	if (**ptr == ' ')
// 	{
// 		(*ptr)++;
// 		return (1);
// 	}
// 	return (0);
// }

t_token	*create_token_from_pipe(t_token **head, t_token **tail)
{
	return (add_pipe_token(head, tail));
}

// t_token *parse_command_line(char *line, t_ctx *ctx)
// {
//     t_token *token_list = NULL;
//     char buffer[1024];
//     int i = 0;
//     int in_single_quotes = 0;
//     int in_double_quotes = 0;

//     while (*line)
//     {
//         // Gestion des quotes simples
//         if (*line == '\'' && !in_double_quotes)
//         {
//             in_single_quotes = !in_single_quotes;
//             line++;
//             continue;
//         }

//         // Gestion des quotes doubles
//         if (*line == '"' && !in_single_quotes)
//         {
//             in_double_quotes = !in_double_quotes;
//             line++;
//             continue;
//         }

//         // Expansion des variables uniquement en dehors des quotes simples
//         if (*line == '$' && !in_single_quotes)
//         {
//             char *start = line;
//             if (handle_env_var(&line, &token_list, ctx))
//                 continue; // Continue si l'expansion a été traitée
//             else
//                 line = start; // Si aucune expansion n'est trouvée, continuez normalement
//         }

//         // Gestion des espaces en dehors des quotes
//         if (is_whitespace(*line) && !in_single_quotes && !in_double_quotes)
//         {
//             if (i > 0)
//             {
//                 buffer[i] = '\0';
//                 add_token(&token_list, TOKEN_ARGUMENT, buffer);
//                 i = 0;
//             }
//             line++;
//             continue;
//         }

//         // Ajout des caractères normaux au buffer
//         buffer[i++] = *line++;
//     }

//     // Ajout du dernier token si nécessaire
//     if (i > 0)
//     {
//         buffer[i] = '\0';
//         add_token(&token_list, TOKEN_ARGUMENT, buffer);
//     }

//     return token_list;
// }


t_token *parse_command_line(char *line, t_ctx *ctx)
{
	(void)ctx;
    t_token *token_list = NULL;
    char buffer[1024];
    int i = 0;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    while (*line)
    {
        // Handle single quotes
        if (*line == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            line++;
            continue;
        }
        // Handle double quotes
        if (*line == '"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            line++;
            continue;
        }
        // Handle whitespace
        if (is_whitespace(*line) && !in_single_quotes && !in_double_quotes)
        {
            if (i > 0)
            {
                buffer[i] = '\0';
                t_token *new_token = add_token(&token_list, TOKEN_ARGUMENT, buffer);
                new_token->type = in_single_quotes ? SINGLE_QUOTE : (in_double_quotes ? DOUBLEQUOTE : STRING);
                i = 0;
            }
            line++;
            continue;
        }
        // Normal characters
        buffer[i++] = *line++;
    }
    // Add last token
    if (i > 0)
    {
        buffer[i] = '\0';
        t_token *new_token = add_token(&token_list, TOKEN_ARGUMENT, buffer);
        new_token->type = in_single_quotes ? SINGLE_QUOTE : (in_double_quotes ? DOUBLEQUOTE : STRING);
    }
    return token_list;
}



// t_token *parse_command_line(char *line, t_ctx *ctx)
// {
// 	(void)ctx;
//     t_token *token_list = NULL;
//     char buffer[1024];
//     int i = 0;
//     int in_single_quotes = 0;
//     int in_double_quotes = 0;

//     while (*line)
//     {
//         // Gestion des quotes simples
//         if (*line == '\'' && !in_double_quotes)
//         {
//             in_single_quotes = !in_single_quotes;
//             line++;
//             continue;
//         }
//         if (*line == '"' && !in_single_quotes)
//         {
//             in_double_quotes = !in_double_quotes;
//             line++;
//             continue;
//         }
//         if (is_whitespace(*line) && !in_single_quotes && !in_double_quotes)
//         {
//             if (i > 0)
//             {
//                 buffer[i] = '\0';
//                 add_token(&token_list, TOKEN_ARGUMENT, buffer);
//                 i = 0;
//             }
//             line++;
//             continue;
//         }
//         buffer[i++] = *line++;
//     }
//     if (i > 0)
//     {
//         buffer[i] = '\0';
//         add_token(&token_list, TOKEN_ARGUMENT, buffer);
//     }
//     return token_list;
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

// int handle_env_var(char **line, t_token **token_list, t_ctx *ctx)
// {
//     char *start = *line;
//     char *exit_code_str;
//     char *var_name;
//     char *value;
//     int len = 0;

//     (*line)++; // Avance après le caractère `$`

//     // Gérer le cas spécial de `$?` (code de sortie)
//     if (**line == '?')
//     {
//         exit_code_str = ft_itoa(ctx->exit_status);
//         if (!exit_code_str)
//             return -1;
//         add_token(token_list, TOKEN_ARGUMENT, exit_code_str);
//         free(exit_code_str);
//         (*line)++;
//         return 1;
//     }
//     // Gérer le nom de variable
//     while (ft_isalnum(**line) || **line == '_')
//     {
//         (*line)++;
//         len++;
//     }
//     if (len == 0) // Si aucun caractère valide après `$`, ne rien faire
//         return 0;

//     var_name = ft_strndup(start + 1, len);
//     if (!var_name)
//         return -1;
//     // value = getenv(var_name); // Récupérer la valeur de la variable
// 	value = find_env_value(var_name, ctx->env_vars); // Récupérer la valeur de la variable
//     if (value)
//         add_token(token_list, TOKEN_ARGUMENT, value);
//     free(var_name);
//     return 1;
// }

int handle_env_var(char **line, t_token **token_list, t_ctx *ctx)
{
    char *start = *line;
    char *exit_code_str;
    char *var_name;
    char *value;
    int len = 0;

    (*line)++; // Avance après le caractère `$`

    // Gérer le cas spécial de `$?` (code de sortie)
    if (**line == '?')
    {
        exit_code_str = ft_itoa(ctx->exit_status);
        if (!exit_code_str)
            return -1;
        add_token(token_list, TOKEN_ARGUMENT, exit_code_str);
        free(exit_code_str);
        (*line)++;
        return 1;
    }

    // Gérer les noms de variables
    while (ft_isalnum(**line) || **line == '_')
    {
        (*line)++;
        len++;
    }

    if (len == 0) // Si aucun caractère valide après `$`, ne rien faire
        return 0;

    var_name = ft_strndup(start + 1, len);
    if (!var_name)
        return -1;

    value = find_env_value(var_name, ctx->env_vars); // Récupérer la valeur de la variable
    if (value)
        add_token(token_list, TOKEN_ARGUMENT, value);
    free(var_name);
    return 1;
}



// int	handle_env_var(char **line, t_token **token_list, t_ctx *ctx)
// {
// 	char	*start = *line;
// 	char	*exit_code_str;
// 	int		len = 1;

// 	// Vérifie si le caractère après `$` est valide pour une variable d'environnement
// 	if (!ft_isalpha(start[1]) && start[1] != '_' && start[1] != '?')
// 		return (0);
// 	(*line)++;	// Avance après le caractère `$`
// 	while (ft_isalnum(**line) || **line == '_')
// 	{
// 		(*line)++;
// 		len++;
// 	}
// 	if(**line == '?')
// 	{
// 		exit_code_str = ft_itoa(ctx->exit_status);
// 		if (!exit_code_str)
// 			return (-1);
// 		// Nouveau code : Vérifier les caractères qui suivent immédiatement `?`
// 		(*line)++;
// 		char *suffix = NULL;
// 		if (ft_isalnum(**line) || **line == '_') // Si des caractères suivent `?`
// 		{
// 			char *start = *line;
// 			while (ft_isalnum(**line) || **line == '_')
// 				(*line)++;
// 			suffix = ft_strndup(start, *line - start); // Extraire la suite
// 		}
// 		// Concaténer l'expansion et la suite (s'il y en a)
// 		char *expanded_value = suffix ? ft_strjoin(exit_code_str, suffix) : ft_strdup(exit_code_str);
// 		if (!expanded_value)
// 			return (free(exit_code_str), free(suffix), -1);
// 		add_token(token_list, TOKEN_ARGUMENT, expanded_value);
// 		free(exit_code_str);
// 		free(suffix);
// 		free(expanded_value);
// 		return (1);
// 	}// Extraire le nom de la variable sans `$`
// 	char *var_name = ft_strndup(start + 1, len - 1);
// 	if (!var_name)
// 		return (-1);
// 	char *value = getenv(var_name);
// 	if (value)
// 		add_token(token_list, TOKEN_ARGUMENT, value);
// 	free(var_name);
// 	return (1); // Indique qu'un token a été traité
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
		handle_token(&head, &tail, &ptr, &first_token);
	}
	free(ptr);
	return head;
}

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

// int	add_token(t_token **head, t_token_type type, const char *value)
// {
// 	t_token	*new_token;
// 	t_token	*current;

// 	new_token = create_token(type, value);
// 	if (!new_token)
// 		return (0);
// 	if (!*head)
// 		*head = new_token;
// 	else
// 	{
// 		current = *head;
// 		while (current->next)
// 			current = current->next;
// 		current->next = new_token;
// 	}
// 	return (0);
// }

t_token *add_token(t_token **token_list, t_token_type type, const char *value)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        perror("malloc failed");
        return NULL;
    }
    new_token->value = ft_strdup(value); // Duplicate the string
    if (!new_token->value)
    {
        free(new_token);
        perror("ft_strdup failed");
        return NULL;
    }
    new_token->type = type;
    new_token->next = NULL;

    if (!*token_list)
    {
        *token_list = new_token; // Set the head if the list is empty
    }
    else
    {
        t_token *current = *token_list;
        while (current->next)
            current = current->next;
        current->next = new_token; // Append to the end of the list
    }

    return new_token; // Return the newly created token
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
		t_ctx *ctx)
{
	int last_cmd = 1;

	if (fd_in != 0)
		dup2(fd_in, STDIN_FILENO);
	if (!last_cmd)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	exec_cmd(cmd_tokens, fd_in, pipe_fd, ctx);
}

// void	handle_child(t_token *cmd_tokens, int fd_in, int pipe_fd[2],
// 		int last_cmd)
// {
// 	if (fd_in != 0)
// 		dup2(fd_in, STDIN_FILENO);
// 	if (!last_cmd)
// 		dup2(pipe_fd[1], STDOUT_FILENO);
// 	close(pipe_fd[1]);
// 	close(pipe_fd[0]);
// 	exec_cmd(cmd_tokens, fd_in, pipe_fd, last_cmd);
// }

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

    tokens = parse_command_line(line, ctx);
    if (tokens) 
    {
        if (validate_pipes(tokens) == -1)
        {
            free_tokens(tokens);
            return 0;
        }
        if (contains_pipe(tokens))
            process_pline(tokens, env, ctx);
        else
            exec_simple_cmd(tokens, env, ctx); // Passer le contexte ici
        free_tokens(tokens);
    }
    return 0;
}

int	ft_cd(char **args, t_ctx *ctx)
{
	static char	*oldpwd = NULL;

	if (args[1] && args[2])
        ;
	if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
	{
		if(ft_cd_home(ctx) != 0)
			return(1);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		if(ft_cd_oldpwd(&oldpwd) != 0)
			return (1);
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("cd");
			return (1);
		}
	}
	ft_update_pwd(&oldpwd);
	return (0);
}

int	ft_cd_home(t_ctx *ctx)
{
	char	*home;

	// home = getenv("HOME");
	// home = get_environment("HOME");
	home = find_env_value("HOME", ctx->env_vars);
	if (home == NULL)
		return (fprintf(stderr, "cd: HOME not set\n"), 1);
	if (chdir(home) != 0)
		return(perror("cd"), 1);
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
		// printf("Current directory: %s\n", cwd);
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

char	**prepare_print_args(t_token *cmd, t_ctx *ctx)
{
	char	**args;
	int		k;

	k = 0;
	args = prepare_args(cmd, ctx);
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

void	exec_cmd(t_token *cmd, int fd_in, int pipe_fd[2], t_ctx *ctx)
{
	char	**args;
	int last_cmd;

	last_cmd = 1;
	args = prepare_print_args(cmd, ctx);
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

// void	exec_cmd(t_token *cmd, int fd_in, int pipe_fd[2], int last_cmd)
// {
// 	char	**args;
// 	int last_cmd;

// 	last_cmd = 1;
// 	args = prepare_print_args(cmd);
// 	if (fd_in != 0)
// 		dup2(fd_in, STDIN_FILENO);
// 	if (!last_cmd)
// 		dup2(pipe_fd[1], STDOUT_FILENO);
// 	close(pipe_fd[1]);
// 	close(pipe_fd[0]);
// 	if (execvp(args[0], args) == -1)
// 	{
// 		perror("Échec de exec");
// 		free(args);
// 		exit(EXIT_FAILURE);
// 	}
// }

void	process_tokens(t_token *cmd_tokens, int num_pipes, t_ctx *ctx)
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
		{
			ctx->num_pipes = j;
			handle_child(&cmd_tokens[j], fd_in, pipe_fd, ctx);
		}
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
	int status;

    args = prepare_args(tokens, ctx); // Passer les tokens ici, pas une chaîne de caractères
    if (!args)
	{
        perror("Erreur d'allocation de mémoire pour les arguments");
		ctx->exit_status = 1;
        return 0;
    }
    if (exec_builtin_cmd(args, env, ctx))
	{
        free(args);
		// ctx->exit_status = 0;
        return 0;
    }
    pid = fork();
    if (pid == -1) {
        perror("Echec fork");
		ctx->exit_status = 1;
        free(args);
        return (-1);
    }
    if (pid == 0)
	{   // Processus enfant: exécuter la commande
        exec(tokens, env, ctx); // Assurez-vous que cette fonction exécute correctement `execve`
        free(args); // Libérer les arguments dans le processus enfant aussi
        exit(127);
    }
    waitpid(pid, &status, 0);
	if(WIFEXITED(status))
		ctx->exit_status = WEXITSTATUS(status);
	else
		ctx->exit_status = 1;
    free(args); // Libérer les arguments après exécution
	return 0;
}


// void split_env_v(const char *input, char **var, char **value)
// {
// 	const char *equal_sign;

// 	equal_sign = ft_strchr(input, '=');
// 	if(equal_sign)
// 	{
// 		size_t	var_len = equal_sign - input;
// 		*var = ft_strndup(input, var_len);
// 		*value = ft_strdup(equal_sign + 1); //apres le =
// 	}
// 	else
// 	{
// 		*var = ft_strdup(input);
// 		*value = NULL;
// 	}
// }

int split_env_v(char *arg, char **var, char **value)
{
    char *equal_sign = strchr(arg, '=');

    if (!equal_sign)
    {
        *var = strdup(arg);
        *value = NULL;
        return *var != NULL; // Échec si allocation échoue
    }

    *var = strndup(arg, equal_sign - arg);
    *value = strdup(equal_sign + 1);

    if (!*var || !*value)
    {
        free(*var);
        free(*value);
        return 0; // Échec d'allocation
    }

    return 1; // Succès
}


// int split_env_v(char *arg, char **var, char **value)
// {
//     char *equal_sign = strchr(arg, '=');

//     if (!equal_sign) {
//         *var = strdup(arg);
//         *value = NULL;
//         return 1; // Succès, mais pas de valeur associée
//     }

//     *var = strndup(arg, equal_sign - arg);
//     *value = strdup(equal_sign + 1);

//     if (!*var || !*value) {
//         free(*var);
//         free(*value);
//         return 0; // Échec d'allocation
//     }

//     return 1; // Succès
// }

int process_exit_arg(char **args, t_ctx *ctx)
{
    char *cleaned_arg;
    long exit_code;

    if (args[1])
	{
        cleaned_arg = strip_quotes(args[1]); // Supprimer les quotes
        if (!is_numeric_argument(cleaned_arg))
		{
            fprintf(stderr, "minishell: exit: %s: numeric argument required\n", cleaned_arg);
            free(cleaned_arg);
            ctx->exit_status = 255;
            return 1; // Indique une sortie immédiate avec code 2
        }
        exit_code = ft_atoi(cleaned_arg);
        free(cleaned_arg);
        if (args[2])
		{
            fprintf(stderr, "minishell: exit: too many arguments\n");
            ctx->exit_status = 1;
            return 0; // Ne pas quitter immédiatement
        }
        ctx->exit_status = (exit_code % 256 + 256) % 256; // Calculer modulo 256
    }
	else 
		ctx->exit_status = 0;
    write(1, "exit\n", 5);
    exit(ctx->exit_status);
    return 1; // Pour respecter les conventions de retour (même si unreachable ici)
}

t_token *create_token_list(char **args)
{
    t_token *head = NULL;
    t_token *current = NULL;

    for (int i = 0; args[i]; i++)
    {
        t_token *new_token = create_token(TOKEN_ARGUMENT, args[i]);
        if (!new_token)
        {
            perror("create_token failed");
            free_tokens(head); // Libère la liste existante en cas d'erreur
            return NULL;
        }
        if (!head) // Premier élément
            head = new_token;
        else
            current->next = new_token;
        current = new_token;
    }
    return head;
}


// t_token *create_token_list(char **args)
// {
//     t_token *head = NULL;
//     t_token *current = NULL;

//     for (int i = 0; args[i]; i++)
//     {
//         t_token *new_token = create_token(TOKEN_ARGUMENT, args[i]); // TOKEN_ARG peut être un type défini pour vos arguments
//         if (!new_token)
//         {
//             perror("create_token failed");
//             free_tokens(head); // Libérer la liste existante en cas d'erreur
//             return (NULL);
//         }
//         if (!head) // Premier élément
//             head = new_token;
//         else
//             current->next = new_token;
//         current = new_token;
//     }
//     return (head);
// }

int ft_export(t_ctx *ctx, char *key, char *value)
{
    t_env_var *current = ctx->env_vars;

    // Vérifie si la variable existe déjà
    while (current)
    {
        if (strcmp(current->name, key) == 0)
        {
            free(current->value);
            current->value = value ? strdup(value) : NULL; // Met à jour la valeur
            return 0;
        }
        if (!current->next)
            break;
        current = current->next;
    }

    // Ajoute une nouvelle variable si elle n'existe pas
    t_env_var *new_var = create_env_var(key);
    if (!new_var)
        return -1;

    new_var->value = value ? strdup(value) : NULL;
    if (current)
        current->next = new_var;
    else
        ctx->env_vars = new_var;

    return 0;
}


int exec_builtin_cmd(char **args, char **env, t_ctx *ctx)
{
    (void)env;
    if (ft_strcmp(args[0], "exit") == 0)
    {
        if (process_exit_arg(args, ctx))
            return 1;
        return 1;
    }

    if (ft_strcmp(args[0], "echo") == 0)
    {
        t_token *token_list = create_token_list(args + 1);
        if (token_list)
        {
            handle_echo(token_list, ctx);
            free_tokens(token_list);
        }
        ctx->exit_status = 0;
        return 1;
    }

    if (ft_strcmp(args[0], "export") == 0)
    {
        if (!args[1])
        {
            t_env_var *current = ctx->env_vars;
            while (current)
            {
                printf("declare -x %s=\"%s\"\n", current->name, current->value ? current->value : "");
                current = current->next;
            }
            ctx->exit_status = 0;
            return 1;
        }

        for (int i = 1; args[i]; i++)
        {
            char *var = NULL;
            char *value = NULL;

            if (!split_env_v(args[i], &var, &value))
            {
                fprintf(stderr, "export: `%s`: not a valid identifier\n", args[i]);
                ctx->exit_status = 1;
                free(var);
                free(value);
                continue;
            }
            if (!is_valid_id(var))
            {
                fprintf(stderr, "export: `%s`: not a valid identifier\n", var);
                free(var);
                free(value);
                ctx->exit_status = 1;
                continue;
            }

            if (ft_export(ctx, var, value) == -1)
                ctx->exit_status = 1;

            free(var);
            free(value);
        }
        return 1;
    }

    if (ft_strcmp(args[0], "unset") == 0)
    {
        if (!args[1])
        {
            ctx->exit_status = 0;
            return 1;
        }
		for (int i = 1; args[i]; i++)
        {
            if (!is_valid_id(args[i]))
            {
                fprintf(stderr, "unset: `%s`: not a valid identifier\n", args[i]);
				ctx->exit_status = 1;
                continue;
            }
            unset_v(&(ctx->env_vars), args[i]);
        }
		if(ctx->exit_status == 0)
        	ctx->exit_status = 0;
        return 1;
    }
	if (ft_strcmp(args[0], "pwd") == 0)
	{
		if (args[1]) // Check for extra arguments
			;
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			printf("%s\n", cwd);
			ctx->exit_status = 0; // Success
		}
		else
		{
			perror("pwd");
			ctx->exit_status = 1; // Failure
		}
		return 1;
	}
    if (ft_strcmp(args[0], "cd") == 0)
    {
        // if (ft_cd(args, ctx) == 0)
        //     ctx->exit_status = 0;
        // else
        //     ctx->exit_status = 1;
        // return 1;
			char **expanded_args = malloc((sizeof(char *) * (count_args(args) + 1)));
		if (!expanded_args)
		{
			perror("malloc failed for expanded_args");
			ctx->exit_status = 1;
			return 1;
		}
		// Expansion des arguments
		for (int i = 0; args[i]; i++)
			expanded_args[i] = expand_variables(args[i], ctx, STRING); // Expansion normale
		expanded_args[count_args(args)] = NULL;
		// Exécute la commande cd
		if (ft_cd(expanded_args, ctx) == 0)
			ctx->exit_status = 0;
		else
			ctx->exit_status = 1;
		free_args(expanded_args); // Libérer les arguments expandis
		return 1;
	}
    return 0; // Not a built-in command
}

int count_args(char **args)
{
    int count = 0;
    while (args[count])
        count++;
    return count;
}

void free_args(char **args)
{
    if (!args)
        return;
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
}




// int exec_builtin_cmd(char **args, char **env, t_ctx *ctx)
// {
// 	if(ft_strcmp(args[0], "exit") == 0)
// 	{
// 		if(process_exit_arg(args, ctx))
// 			return (1);
// 		return (1);
// 	}
// 	if (ft_strcmp(args[0], "echo") == 0)
// 	{
// 		t_token *token_list = create_token_list(args + 1);
// 		if(token_list)
// 		{
// 			handle_echo(token_list, ctx);
// 			free_tokens(token_list);
// 		}
// 		ctx->exit_status = 0;
// 		return (1);
// 	}
//     if (ft_strcmp(args[0], "export") == 0)
// 	{
//         if (!args[1])
// 		{
//             // Cas: `export` sans argument - afficher les variables
//             char **env_ptr = env;
//             while (*env_ptr) 
// 			{
//                 printf("declare -x %s\n", *env_ptr);
//                 env_ptr++;
//             }
//             ctx->exit_status = 0; // Success
//             return 1;
//         }// Cas: `export` avec des arguments
//         for (int i = 1; args[i]; i++)
// 		{
//             char *var = NULL;
//             char *value = NULL;
//             // Séparer la variable et la valeur
//             if (!split_env_v(args[i], &var, &value))
// 			{
//                 // fprintf(stderr, "export: `%s`: not a valid identifier\n", args[i]);
//                 // ctx->exit_status = 1; // Échec sur cet argument
//                 free(var);
//                 free(value);
// 				ctx->exit_status = 1;
//                 continue;
//             }// Ajouter ou mettre à jour la variable
// 			if (!is_valid_id(var))
// 			{
// 				fprintf(stderr, "export: `%s`: not a valid identifier\n", var);
// 				free(var);
// 				free(value);
// 				ctx->exit_status = 1; // Échec pour cet argument
// 				continue;
// 			}// Ajouter ou mettre à jour la variable dans l'environnement
// 			if (export_v(&env, var, value) == -1)
// 			{
// 				perror("export failed");
// 				ctx->exit_status = 1; // Échec de la mise à jour
// 			}
// 			// if(!value)
// 			// 	return (0);
//             // if (export_v(&env, var, value) == -1)
// 			// {
//             //     perror("export failed");
//             //     ctx->exit_status = 1; // Échec de la mise à jour
//             // }
//             free(var);
//             free(value);
//         }
//         return 1;
//     }
//     if (ft_strcmp(args[0], "unset") == 0)
// 	{
// 		if (!args[1])
// 		{
// 			ctx->exit_status = 0; // Succès : aucun argument, rien à faire
// 			return (1);
// 		}

// 		int error_occurred = 0; // Indique si une erreur s'est produite
// 		for (int i = 1; args[i]; i++)
// 		{
// 			if (!is_valid_id(args[i]))
// 			{
// 				fprintf(stderr, "unset: `%s`: not a valid identifier\n", args[i]);
// 				error_occurred = 1; // Une erreur est survenue
// 				continue;
// 			}
// 			if (unset_v(&env, args[i]) == -1)
// 			{
// 				fprintf(stderr, "unset: error while unsetting `%s`\n", args[i]);
//             	error_occurred = 1;
// 			}
// 		}// Ajuster le code de sortie en fonction des erreurs
// 		if (error_occurred)
// 			ctx->exit_status = 1;
// 		else
// 			ctx->exit_status = 0;
// 		return (1);
//         // if (!args[1])
// 		// {            // Cas: `unset` sans arguments
//         //     ctx->exit_status = 0; // Succès
//         //     return (1);
//         // }
//         // // Cas: `unset` avec des arguments
//         // for (int i = 1; args[i]; i++) {
//         //     if (!is_valid_id(args[i])) {
//         //         fprintf(stderr, "unset: `%s`: not a valid identifier\n", args[i]);
//         //         ctx->exit_status = 1; // Échec sur cet argument
//         //         continue;
//         //     }
// 		// 	if(unset_v(&env, args[i]) == -1)
// 		// 		ctx->exit_status = 1;
//         //     // Supprimer la variable si elle existe
//         //     unset_v(&env, args[i]);
//         // }
//         // ctx->exit_status = 0; // Succès global si aucune erreur majeure
//         // return 1;
//     }
//     if (ft_strcmp(args[0], "cd") == 0) {
//         if (ft_cd(args, ctx) == 0)
//             ctx->exit_status = 0; // Succès
//         else
//             ctx->exit_status = 1; // Échec
//         return 1;
//     }
//     return 0; // Pas un builtin
// }

char *strip_quotes(char *arg)
{
    if ((*arg == '"' || *arg == '\'') && arg[strlen(arg) - 1] == *arg) {
        return ft_strndup(arg + 1, strlen(arg) - 2); // Supprimer les quotes
    }
    return ft_strdup(arg);
}

int is_numeric_argument(const char *arg)
{
    if (!arg || (*arg != '+' && *arg != '-' && !ft_isdigit(*arg)))
        return 0; // L'argument doit commencer par +, -, ou un chiffre
    if (*arg == '+' || *arg == '-')
        arg++; // Ignorer le signe initial
    while (*arg) 
	{
        if (!ft_isdigit(*arg))
            return 0; // Tous les caractères restants doivent être numériques
        arg++;
    }
    return 1;
}

int read_and_exec(char **env, t_ctx *ctx)
{
    char *line;

    ctx->exit_status = 0;

    // Construire les variables d'environnement
    ctx->env_vars = get_environment(env);
    if (!ctx->env_vars)
        return (perror("Failed to initialize environment variables"), 1);
    while (1)
    {
        line = readline(PROMPT);
        if (line == NULL)
        {
            write(1, "exit\n", 5);
            free_env_vars(ctx->env_vars);
            exit(ctx->exit_status); 
        }
        if (*line)
        {
            add_history(line);
            handle_line(line, env, ctx); // Passer `ctx` pour le contexte global
        }
        free(line);
    }
    // free_env_vars(ctx->env_vars);
    return (ctx->exit_status);
}

// int read_and_exec(char **env)
// {
//     char *line;
//     t_ctx ctx;
//     ctx.exit_status = 0;

//     while (1)
// 	{
//         line = readline(PROMPT);
//         if (line == NULL) {
//             write(1, "exit\n", 5);
//             exit(ctx.exit_status); // Utiliser le dernier exit_status
//         }
//         if (*line)
// 		{
// 			add_history(line);
//             handle_line(line, env, &ctx);
// 		}
//         free(line);
//     }
//     return ctx.exit_status; // Retourner le dernier code de sortie
// }

int	loop(char **env, t_ctx *ctx)
{
	while (1)
		read_and_exec(env, ctx);
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

int	process_pline(t_token *tokens, char **env, t_ctx *ctx)
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
		child(first_cmd_t, pipe_fd, env, ctx);
	}
	pid2 = fork();
	if (pid2 == -1)
		exit_error();
	if (pid2 == 0) // Processus enfant 2 (après le pipe)
	{
		second_cmd_t = extract_command_after(tokens);
		parent(second_cmd_t, pipe_fd, env, ctx);
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
    full_path = malloc(ft_strlen(path_with_slash) + ft_strlen(cmd) + 1);
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


char **prepare_args(t_token *tokens, t_ctx *ctx)
{
	(void)ctx;
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


int	exec(t_token *cmd_tokens, char **env, t_ctx *ctx)
{
	char	**option_cmd;
	char	*path;
	// int exit_status = 0;

	option_cmd = prepare_args(cmd_tokens, ctx);
	if (!option_cmd[0])
	{
		fprintf(stderr, "Error: Command is empty\n");
		free_tab(option_cmd);
		exit(EXIT_FAILURE);
	}
	if (strcmp(option_cmd[0], "cd") == 0)
	{
		ft_cd(option_cmd, ctx);
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

void	child(t_token *tokens, int *pipe_fd, char **env, t_ctx *ctx)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	exec(tokens, env, ctx);
}

void	parent(t_token *tokens, int *pipe_fd, char **env, t_ctx *ctx)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	exec(tokens, env, ctx);
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


int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    char **env_copy = copy_envp(envp);
    if (!env_copy)
    {
        perror("Failed to copy environment");
        return (1);
    }

    t_ctx ctx;
    ctx.env_vars = get_environment(env_copy);
    if (!ctx.env_vars)
    {
        perror("Failed to initialize environment variables");
        free_env_copy(env_copy); 
        return (1);
    }

    ctx.exit_status = 0;
    // read_and_exec(env_copy, &ctx);
	loop(env_copy, &ctx);
    free_env_copy(env_copy);
    free_env_vars(ctx.env_vars);
    return ctx.exit_status;
}


// int main(int argc, char **argv, char **envp)
// {
//     (void)argc;
//     (void)argv;

//     char **env_copy = copy_envp(envp);
//     if (!env_copy)
//     {
//         perror("Failed to copy environment");
//         return (1);
//     }
//     t_ctx ctx;
//     ctx.env_vars = get_environment(env_copy);
//     if (!ctx.env_vars)
//     {
//         perror("Failed to initialize environment variables");
//         free_env_copy(env_copy); 
//         return (1);
//     }
//     ctx.exit_status = 0;
//     read_and_exec(env_copy);
//     free_env_copy(env_copy);
//     free_env_vars(ctx.env_vars); 
//     return ctx.exit_status;
// }

// Libère un tableau d'environnement
void free_env_copy(char **env_copy)
{
    if (!env_copy)
        return;
    for (int i = 0; env_copy[i]; i++)
        free(env_copy[i]);
    free(env_copy);
}

// Libère une liste chaînée de variables d'environnement
void free_env_vars(t_env_var *env_vars)
{
    while (env_vars)
    {
        t_env_var *to_free = env_vars;
        env_vars = env_vars->next;
        free(to_free->name);
        free(to_free->value);
        free(to_free);
    }
}


// int	main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
// 		char **envp)
// {
// 	char	**env_copy;

// 	init_sig();
// 	env_copy = get_environment(envp);
// 	if (!env_copy)
// 		return (fprintf(stderr, "Failed to copy environment\n"), 1);
// 	loop(env_copy);
// 	free_tab(env_copy);
// 	return (0);
// }

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: '%s'\n", tokens->value);
		tokens = tokens->next;
	}
}