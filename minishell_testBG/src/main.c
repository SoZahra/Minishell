/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:03:19 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/27 18:42:00 by fzayani          ###   ########.fr       */
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
char **get_environment(char **envp)
{
    int count = 0;
    while (envp[count])
        count++;

    char **env_copy = malloc((count + 1) * sizeof(char *));
    if (!env_copy)
    {
        perror("malloc failed");
        return NULL;
    }

    for (int i = 0; i < count; i++)
    {
        env_copy[i] = strdup(envp[i]);
        if (!env_copy[i])
        {
            perror("strdup failed");
            while (i-- > 0)
                free(env_copy[i]);
            free(env_copy);
            return NULL;
        }
    }
    env_copy[count] = NULL;
    return env_copy;
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

t_env_var *export_v(t_env_var *env_vars, const char *var, const char *value) {
    printf("DEBUG - export_v: updating '%s' to '%s'\n", var, value);

    t_env_var *current = env_vars;
    while (current) {
        if (ft_strcmp(current->name, var) == 0) {
            free(current->value);
            current->value = value ? strdup(value) : strdup("");
            printf("DEBUG - export_v: updated existing var '%s' to '%s'\n", var, current->value);
            return env_vars;
        }
        current = current->next;
    }

    t_env_var *new_var = malloc(sizeof(t_env_var));
    new_var->name = strdup(var);
    new_var->value = value ? strdup(value) : strdup("");
    new_var->next = env_vars;
    printf("DEBUG - export_v: added new var '%s' with value '%s'\n", new_var->name, new_var->value);
    setenv(var, value, 1);
    return new_var;
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
//             char *value_joined = value ? ft_strjoin(new_var, value) : strdup(new_var); // Gérer valeur NULL

//             // Met à jour la variable dans l'environnement
//             (*env_copy)[i] = value_joined;
//             free(new_var); // Libère la mémoire de la chaîne intermédiaire
//             return 0;
//         }
//         i++;
//     }
//     // Si la variable n'existe pas, ajoute-la
//     new_var = ft_strjoin(var, "=");
//     char *new_entry = value ? ft_strjoin(new_var, value) : strdup(new_var);
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
    return (0); // Variable not found
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

// char *expand_variables(const char *str, t_ctx *ctx, t_token_type token_type)
// {
//     if (token_type == SINGLE_QUOTE)
//         return ft_strdup(str); // Return as-is for single-quoted strings

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


char *expand_variables(const char *str, t_ctx *ctx, t_token_type token_type)
{
    // Pour les single quotes, retourner la chaîne telle quelle sans expansion
    if (token_type == SINGLE_QUOTE)
    {
        return ft_strdup(str);
    }

    char *expanded = ft_strdup("");
    char *temp;

    while (*str)
    {
        if (*str == '$' && token_type != SINGLE_QUOTE) // Ne jamais expandre dans les single quotes
        {
            str++; // Skip the '$'
            if (*str == '?') // Special case for `$?`
            {
                char *exit_code = ft_itoa(ctx->exit_status);
                temp = ft_strjoin(expanded, exit_code);
                free(expanded);
                free(exit_code);
                expanded = temp;
                str++;
            }
            else if (ft_isalnum(*str) || *str == '_') // Valid variable name
            {
                char buffer[1024];
                int i = 0;
                while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                {
                    buffer[i] = str[i];
                    i++;
                }
                buffer[i] = '\0';
                str += i;

                char *value = find_env_value(buffer, ctx->env_vars);
                temp = ft_strjoin(expanded, value ? value : "");
                free(expanded);
                expanded = temp;
            }
            else // Invalid variable, keep the $
            {
                temp = ft_strjoin(expanded, "$");
                free(expanded);
                expanded = temp;
                if (*str && *str != ' ') // Si ce n'est pas un espace
                {
                    char curr[2] = {*str, '\0'};
                    temp = ft_strjoin(expanded, curr);
                    free(expanded);
                    expanded = temp;
                    str++;
                }
            }
        }
        else
        {
            char curr[2] = {*str, '\0'};
            temp = ft_strjoin(expanded, curr);
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
//         return ft_strdup(str); // Return as-is for single-quoted strings

//     char *expanded = ft_strdup("");
//     char *temp;

//     while (*str)
//     {
//         if (*str == '$') // Variable detected
//         {
//             str++; // Skip the '$'
//             if (*str == '?') // Special case for `$?`
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

//                 // Extract variable name
//                 while (ft_isalnum(*str) || *str == '_')
//                     buffer[i++] = *str++;
//                 buffer[i] = '\0';

//                 // Fetch value from environment
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
//         else // Literal character
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

    void _print_tokens(t_token *token_list)
    {
        if (!token_list)
            return ;
        t_token *tmp = token_list;
        while (tmp)
        {
            printf("value: %s\ntype: %d\n", tmp->value, tmp->type);
            tmp = tmp->next;
        }
    }


void write_echo_content(t_token *token_list, int n_option, t_ctx *ctx)
{
    t_token *current = token_list;
    int first_arg = 1;
    int prev_was_quote = 0;  // Flag pour suivre si le token précédent était entre quotes

    // printf("\nDEBUG 5 - write_echo_content: Début du traitement echo\n");

    while (current)
    {
        // printf("DEBUG 6 - write_echo_content: Traitement token type='%c', valeur='%s'\n",
        //        (char)current->type, current->value);

        // On ajoute un espace seulement si :
        // - Ce n'est pas le premier argument
        // - Le token précédent n'était pas entre quotes OU le token actuel n'est pas entre quotes
        if (!first_arg && !(prev_was_quote && (current->type == '\'' || current->type == '"')))
            write(STDOUT_FILENO, " ", 1);

        if (current->type == '"')  // Double quote: On expand
        {
            char *expanded = expand_variables(current->value, ctx, current->type);
            if (expanded)
            {
                write(STDOUT_FILENO, expanded, ft_strlen(expanded));
                free(expanded);
            }
            prev_was_quote = 1;  // Marquer que c'était une quote
        }
        else if (current->type == '\'') // Single quote: pas d'expansion
        {
            write(STDOUT_FILENO, current->value, ft_strlen(current->value));
            prev_was_quote = 1;  // Marquer que c'était une quote
        }
        else // Texte normal
        {
            write(STDOUT_FILENO, current->value, ft_strlen(current->value));
            prev_was_quote = 0;  // Réinitialiser le flag
        }

        first_arg = 0;
        current = current->next;
    }

    if (!n_option)
        write(STDOUT_FILENO, "\n", 1);
}

// void write_echo_content(t_token *token_list, int n_option, t_ctx *ctx)
// {
//     t_token *current = token_list;
//     int first_arg = 1;
//     int prev_was_quote = 0;

//     // printf("\nDEBUG 5 - write_echo_content: Début du traitement echo\n");

//     while (current)
//     {
//         // printf("DEBUG 6 - write_echo_content: Traitement token type='%c', valeur='%s'\n",
//             //    (char)current->type, current->value);
//         if (!first_arg)
//             write(STDOUT_FILENO, " ", 1);

//         if (current->type == '"')  // Double quote: On expand
//         {
//             char *expanded = expand_variables(current->value, ctx, current->type);
//             // printf("DEBUG 7 - write_echo_content: Double quote, expansion: '%s' -> '%s'\n",
//                 //    current->value, expanded ? expanded : "NULL");
//             if (expanded)
//             {
//                 write(STDOUT_FILENO, expanded, ft_strlen(expanded));
//                 free(expanded);
//             }
//         }
//         else // Single quote ou texte normal: pas d'expansion
//         {
//             // printf("DEBUG 8 - write_echo_content: Sans expansion: '%s'\n", current->value);
//             write(STDOUT_FILENO, current->value, ft_strlen(current->value));
//         }

//         first_arg = 0;
//         current = current->next;
//     }
//     if (!n_option)
//         write(STDOUT_FILENO, "\n", 1);
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

// char *get_sub_quote(char *line, int *i)
// {
//     char *res;
//     int i;
//     int j;
//     int type;

//     type = line[*i];
//     j = (*i)++;
//     while (line[*i] && line[*i] != type)
//         (*i)++;
//     if (!line[*i])
//         return NULL;
//     res = ft_substr(line, j, *i - j + 1);
//     return res;
// }

// char *get_sub_quote(char *line, int *i)
// {
//     char *res;
//     int j;
//     char quote_type;

//     quote_type = line[*i];
//     j = ++(*i);
//     while (line[*i] && line[*i] != quote_type)
//         (*i)++;
//     if (!line[*i])
//         return NULL;
//     res = ft_substr(line, j, *i - j);
//     return res;
// }

int check_invalid_quotes(char *line)
{
    char quote_type = 0;  // Garde le type de quote qu'on cherche à fermer
    int i = 0;

    while (line[i])
    {
        if ((line[i] == '\'' || line[i] == '"') && !quote_type)
            quote_type = line[i];
        else if (line[i] == quote_type)
            quote_type = 0;
        else if ((line[i] == '\'' || line[i] == '"') && !quote_type)
            return 1;
        i++;
    }
    if (quote_type)  // Quote non fermée
        return 1;
    return 0;
}

t_token *parse_command_line(char *line, t_ctx *ctx)
{
    if (check_invalid_quotes(line))
    {
        fprintf(stderr, "Error: quote found without matching opening quote\n");
        return NULL;
    }

    t_token *token_list = NULL;
    char buffer[1024];
    int i = 0;
    int j = 0;
    char current_quote = 0;  // Pour suivre si on est dans des quotes
    char *temp = NULL;  // Pour joindre les tokens
    (void)ctx;

    // printf("DEBUG - Parsing: '%s'\n", line);

    // Premier mot (echo)
    while (line[i] && is_whitespace(line[i]))
        i++;
    while (line[i] && !is_whitespace(line[i]))
        buffer[j++] = line[i++];
    if (j > 0)
    {
        buffer[j] = '\0';
        add_token(&token_list, 'T', buffer);
        j = 0;
    }

    // Skip les espaces
    while (line[i] && is_whitespace(line[i]))
        i++;

    // Traitement des autres mots
    while (line[i])
    {
        if ((line[i] == '\'' || line[i] == '"') && !current_quote)
        {
            current_quote = line[i];  // Début des quotes
            i++;
            j = 0;  // Reset buffer

            // Capture jusqu'à la quote fermante
            while (line[i] && line[i] != current_quote)
                buffer[j++] = line[i++];

            if (!line[i])
            {
                fprintf(stderr, "Error: unclosed quote\n");
                free_tokens(token_list);
                return NULL;
            }

            buffer[j] = '\0';
            // printf("DEBUG - Found quoted content: '%s' with type '%c'\n", buffer, current_quote);

            // Si le prochain caractère est aussi une quote, on continue
            if (line[i + 1] && (line[i + 1] == '\'' || line[i + 1] == '"'))
            {
                if (!temp)
                    temp = ft_strdup(buffer);
                else
                {
                    char *new_temp = ft_strjoin(temp, buffer);
                    free(temp);
                    temp = new_temp;
                }
            }
            else
            {
                // Ajouter le token combiné ou simple
                if (temp)
                {
                    char *final = ft_strjoin(temp, buffer);
                    add_token(&token_list, current_quote, final);
                    // printf("DEBUG - Added combined token: '%s'\n", final);
                    free(final);
                    free(temp);
                    temp = NULL;
                }
                else
                {
                    add_token(&token_list, current_quote, buffer);
                    // printf("DEBUG - Added single token: '%s'\n", buffer);
                }
            }

            i++;  // Skip la quote fermante
            current_quote = 0;
            j = 0;
        }
        else if (is_whitespace(line[i]))
        {
            i++;
        }
        else
        {
            // Texte normal
            while (line[i] && !is_whitespace(line[i]) && line[i] != '\'' && line[i] != '"')
                buffer[j++] = line[i++];
            if (j > 0)
            {
                buffer[j] = '\0';
                add_token(&token_list, 'T', buffer);
                j = 0;
            }
        }
    }

    if (temp)
        free(temp);
    return token_list;
}

// t_token *parse_command_line(char *line, t_ctx *ctx)
// {
//     if (check_invalid_quotes(line))
//     {
//         fprintf(stderr, "Error: quote found without matching opening quote\n");
//         return NULL;
//     }

//     t_token *token_list = NULL;
//     char buffer[1024];
//     int i = 0;
//     int j = 0;
//     (void)ctx;

//     // Premier mot (echo)
//     while (line[i] && is_whitespace(line[i]))
//         i++;
//     while (line[i] && !is_whitespace(line[i]))
//         buffer[j++] = line[i++];
//     if (j > 0)
//     {
//         buffer[j] = '\0';
//         add_token(&token_list, 'T', buffer);
//         j = 0;
//     }

//     // Skip les espaces
//     while (line[i] && is_whitespace(line[i]))
//         i++;

//     // Traitement des autres mots
//     while (line[i])  // On continue tant qu'il y a des caractères
//     {
//         if (line[i] == '\'')
//         {
//             i++;
//             while (line[i] && line[i] != '\'')
//                 buffer[j++] = line[i++];
//             if (!line[i])
//             {
//                 fprintf(stderr, "Error: unclosed single quote\n");
//                 free_tokens(token_list);
//                 return NULL;
//             }
//             buffer[j] = '\0';
//             add_token(&token_list, '\'', buffer);
//             i++; // Skip la dernière quote
//             j = 0;  // Reset le buffer pour le prochain mot
//         }
//         else if (line[i] == '"')
//         {
//             i++;
//             while (line[i] && line[i] != '"')
//                 buffer[j++] = line[i++];
//             if (!line[i])
//             {
//                 fprintf(stderr, "Error: unclosed double quote\n");
//                 free_tokens(token_list);
//                 return NULL;
//             }
//             buffer[j] = '\0';
//             add_token(&token_list, '"', buffer);
//             i++;
//             j = 0;  // Reset le buffer pour le prochain mot
//         }
//         else if (is_whitespace(line[i]))
//         {
//             if (j > 0)
//             {
//                 buffer[j] = '\0';
//                 add_token(&token_list, 'T', buffer);
//                 j = 0;
//             }
//             i++;
//         }
//         else
//         {
//             buffer[j++] = line[i++];
//         }
//     }

//     // Dernier mot s'il y en a un
//     if (j > 0)
//     {
//         buffer[j] = '\0';
//         add_token(&token_list, 'T', buffer);
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
		add_token(head, STRING, token_str);
	else
		add_token(head, STRING, token_str);
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
        add_token(token_list, STRING, exit_code_str);
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
        add_token(token_list, STRING, value);
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
    new_token->content = ft_strdup(value); // Duplicate aussi pour content
    if (!new_token->value || !new_token->content)
    {
        if (new_token->value)
            free(new_token->value);
        if (new_token->content)
            free(new_token->content);
        free(new_token);
        perror("ft_strdup failed");
        return NULL;
    }
    new_token->type = type;
    new_token->next = NULL;
    if (!*token_list)
    {
        *token_list = new_token;
    }
    else
    {
        t_token *current = *token_list;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
    return new_token;
}


// t_token_type	get_token_type(const char *str)
// {
// 	if (ft_strcmp(str, "|") == 0)
// 		return (TOKEN_PIPE);
// 	if (ft_strcmp(str, "<") == 0)
// 		return (TOKEN_REDIRECT_INPUT);
// 	if (ft_strcmp(str, ">") == 0)
// 		return (TOKEN_REDIRECT_OUTPUT);
// 	if (ft_strcmp(str, ">>") == 0)
// 		return (TOKEN_REDIRECT_APPEND);
// 	if (ft_strcmp(str, "<<") == 0)
// 		return (TOKEN_HEREDOC);
// 	return (TOKEN_ARGUMENT);
// }

// int	finalize_tokens(int in_quotes, char quote_char, char *buffer, int *i,
// 		t_token **token_list)
// {
// 	if (in_quotes)
// 	{
// 		fprintf(stderr, "Syntax error : unmatched %c\n", quote_char);
// 		return (-1);
// 	}
// 	if (*i > 0)
// 	{
// 		buffer[*i] = '\0';
// 		add_token(token_list, TOKEN_ARGUMENT, buffer);
// 	}
// 	return (0);
// }

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


// int handle_line(char *line, char **env, t_ctx *ctx)
// {
//     t_token *tokens;

//     tokens = parse_command_line(line, ctx);
//     if (tokens)
//     {
//         if (validate_pipes(tokens) == -1)
//         {
//             free_tokens(tokens);
//             return 0;
//         }
//         if (contains_pipe(tokens))
//             process_pline(tokens, env, ctx);
//         else
//             exec_simple_cmd(tokens, env, ctx); // Passer le contexte ici
//         free_tokens(tokens);
//     }
//     return 0;
// }


int ft_cd(char **args, t_ctx *ctx)
{
    char *path;

    // Vérifier si trop d'arguments sont passés
    if (args[1] && args[2])
	{
        fprintf(stderr, "cd: too many arguments\n");
        ctx->exit_status = 1;
        return 1;
    }
    if (!args[1] || ft_strcmp(args[1], "~") == 0) {
        return ft_cd_home(ctx);
    } else if (ft_strcmp(args[1], "-") == 0) {
        return ft_cd_oldpwd(ctx);
    }

    // Expansion de la variable si nécessaire
	path = expand_variables(args[1], ctx, STRING);
	if (!path || ft_strlen(path) == 0) // Si l'expansion échoue ou retourne une chaîne vide
	{
		fprintf(stderr, "cd: %s: No such file or directoryxxxxxx\n", args[1]);
		free(path);
		ctx->exit_status = 1;
		return 1;
	}
	// Vérification de chdir
	if (chdir(path) != 0)
	{
		perror("cd");
		free(path);
		ctx->exit_status = 1;
		return 1;
	}
	free(path);
	return ft_update_pwd(ctx);
}


int ft_cd_home(t_ctx *ctx) {
    char *home = find_env_value("HOME", ctx->env_vars);
    if (!home) {
        fprintf(stderr, "cd: HOME not set\n");
        return 1;
    }
    if (chdir(home) != 0) {
        perror("cd");
        return 1;
    }
    return ft_update_pwd(ctx);
}


// int	ft_cd_home(t_ctx *ctx)
// {
// 	char	*home;

// 	// home = getenv("HOME");
// 	// home = get_environment("HOME");
// 	home = find_env_value("HOME", ctx->env_vars);
// 	if (home == NULL)
// 		return (fprintf(stderr, "cd: HOME not set\n"), 1);
// 	if (chdir(home) != 0)
// 		return(perror("cd"), 1);
// 	return (0);
// }

int	ft_cd_oldpwd(t_ctx *ctx)
{
	if (!(ctx->oldpwd == NULL))
	{
		fprintf(stderr, "cd: OLDPWD not set\n");
		return (0);
	}
	if (chdir(ctx->oldpwd) != 0)
	{
		perror("cd");
		return (0);
	}
	printf("%s\n", ctx->oldpwd);
	return(0);
}
int ft_update_pwd(t_ctx *ctx) {
    char *cwd = getcwd(NULL, 0); // Obtenir le répertoire actuel
    if (!cwd) {
        perror("getcwd");
        return 1;
    }
    if (ctx->oldpwd)
        free(ctx->oldpwd);   // Libérer l'ancien `OLDPWD`
    ctx->oldpwd = ctx->pwd;  // Mettre à jour `OLDPWD`
    ctx->pwd = cwd;          // Attribuer le nouveau chemin à `PWD`
    return 0;
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

void process_tokens(t_token *cmd_tokens, int num_pipes, t_ctx *ctx)
{
    int pipe_fd[2];
    int fd_in = 0; // Descripteur d'entrée pour les pipes
    pid_t pid;

    // Debug: Affichage des commandes et du nombre de pipes
    printf("Number of pipes: %d\n", num_pipes);
    for (int k = 0; k <= num_pipes; k++) {
        t_token *cmd = &cmd_tokens[k];
        printf("Command %d: ", k);
        while (cmd) {
            printf("%s ", cmd->value);
            cmd = cmd->next;
        }
        printf("\n");
    }
    for (int j = 0; j <= num_pipes; j++) {
        if (j < num_pipes) {
            if (pipe(pipe_fd) == -1) {
                perror("Échec de pipe");
                exit(EXIT_FAILURE);
            }
        }
        if ((pid = fork()) == -1) {
            perror("Échec de fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)// Processus enfant
        {
            if (fd_in != 0) {
                dup2(fd_in, STDIN_FILENO); // Rediriger l'entrée
            }
            if (j < num_pipes) {
                dup2(pipe_fd[1], STDOUT_FILENO); // Rediriger la sortie vers le pipe
            }
            close(pipe_fd[1]); // Fermer le descripteur d'écriture du pipe
            close(pipe_fd[0]); // Fermer le descripteur de lecture du pipe
            t_token *cmd = &cmd_tokens[j];// Préparer les arguments pour la commande actuelle
            char **args = prepare_args(cmd, ctx);
            if (args == NULL || args[0] == NULL) {
                fprintf(stderr, "Erreur lors de la préparation des arguments pour la commande\n");
                exit(EXIT_FAILURE);
            }
            printf("Exécution de la commande : %s\n", args[0]);// Debug: Afficher les arguments
            for (int k = 0; args[k] != NULL; k++) {
                printf("Argument %d : %s\n", k, args[k]);
            }
            if (execvp(args[0], args) == -1) {
                perror("Échec de exec");
                free(args); // Libérer la mémoire avant de sortir
                exit(EXIT_FAILURE);
            }
        }
        else// Processus parent
        {
            close(pipe_fd[1]); // Fermer le descripteur d'écriture du pipe dans le parent
            waitpid(pid, NULL, 0); // Attendre que l'enfant termine
            if (fd_in != 0) {
                close(fd_in); // Fermer l'ancien pipe d'entrée
            }
            fd_in = pipe_fd[0]; // Le prochain processus lira à partir de ce pipe
        }
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
char **convert_env_to_array(t_ctx *ctx)
{
    printf("DEBUG - convert_env_to_array start\n");
    if (!ctx || !ctx->env_vars)
    {
        printf("DEBUG - NULL context or env_vars\n");
        return NULL;
    }
    int count = 0;
    t_env_var *current = ctx->env_vars;
    char **env_array;

    while (current)
    {
        count++;
        current = current->next;
    }

    env_array = malloc((count + 1) * sizeof(char *));
    if (!env_array)
        return NULL;

    current = ctx->env_vars;
    for (int i = 0; i < count && current; i++)
    {
        int len = strlen(current->name) + (current->value ? ft_strlen(current->value) : 0) + 2;
        env_array[i] = malloc(len);
        if (!env_array[i])
        {
            free_tab(env_array);
            return NULL;
        }
        if (current->value)
            snprintf(env_array[i], len, "%s=%s", current->name, current->value);
        else
            snprintf(env_array[i], len, "%s=", current->name);
        current = current->next;
    }
    env_array[count] = NULL;
    return env_array;
}

int exec_simple_cmd(t_token *tokens, char **env, t_ctx *ctx)
{
    char **args;
    pid_t pid;
    int status;
	(void)env;

    // Préparer les arguments
    args = prepare_args(tokens, ctx);
    if (!args || !args[0])
    {
        perror("Erreur d'allocation de mémoire pour les arguments");
        free_tab(args);
        ctx->exit_status = 1;
        return 0;
    }

    // Vérifier si c'est une commande builtin
    if (is_builtin(args[0]))
    {
        // Convertir env_vars en tableau pour les builtins
        char **env_array = convert_env_to_array(ctx);
        if (!env_array)
        {
            free_tab(args);
            ctx->exit_status = 1;
            return 0;
        }

        int ret = exec_builtin_cmd(args, env_array, ctx);
        free_tab(args);
        free_tab(env_array);
        return ret;
    }

    // Si ce n'est pas un builtin, fork et exécuter
    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        free_tab(args);
        return -1;
    }

    if (pid == 0)
    {
        // Convertir env_vars en tableau pour execve
        char **env_array = convert_env_to_array(ctx);
        if (!env_array)
        {
            free_tab(args);
            exit(EXIT_FAILURE);
        }

        // Trouver le chemin de la commande
        char *path = get_path(args[0], env_array);
        if (!path)
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
            free_tab(args);
            free_tab(env_array);
            exit(127);
        }

        // Exécuter la commande
        execve(path, args, env_array);
        free(path);
        free_tab(args);
        free_tab(env_array);
        perror("execve");
        exit(127);
    }

    // Parent
    free_tab(args);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        ctx->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        ctx->exit_status = 128 + WTERMSIG(status);

    return 0;
}

// int exec_simple_cmd(t_token *tokens, char **env, t_ctx *ctx)
// {
//     char **args;
//     pid_t pid;
// 	int status;
//     char **env_array;
//     (void)env;

//     args = prepare_args(tokens, ctx); // Passer les tokens ici, pas une chaîne de caractères
//     if (!args)
// 	{
//         perror("Erreur d'allocation de mémoire pour les arguments");
// 		ctx->exit_status = 1;
//         return 0;
//     }
//     env_array = convert_env_to_array(ctx);
//     if(!env_array)
//     {
//         perror("Erreur de conversion de l'env");
//         free(args);
//         ctx->exit_status = 1;
//         return (0);
//     }
//     if (exec_builtin_cmd(args, env_array, ctx))
// 	{
//         free(args);
// 		free_tab(env_array);
//         return 0;
//     }
//     pid = fork();
//     if (pid == -1) {
//         perror("Echec fork");
// 		ctx->exit_status = 1;
//         free(args);
//         return (-1);
//     }
//     if (pid == 0)
// 	{   // Processus enfant: exécuter la commande
//         // ICI FAUT CHNGER WSH
//         exec(tokens, NULL, ctx); // Assurez-vous que cette fonction exécute correctement `execve`
//         free(args); // Libérer les arguments dans le processus enfant aussi
//         exit(127);
//     }
//     waitpid(pid, &status, 0);
// 	if(WIFEXITED(status))
// 		ctx->exit_status = WEXITSTATUS(status);
// 	else
// 		ctx->exit_status = 1;
//     free(args); // Libérer les arguments après exécution
// 	return 0;
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
            ctx->exit_status = 2;
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
        t_token *new_token = malloc(sizeof(t_token));
        if (!new_token)
        {
            perror("malloc failed");
            free_tokens(head);
            return NULL;
        }

        new_token->value = ft_strdup(args[i]);
        if (!new_token->value)
        {
            free(new_token);
            free_tokens(head);
            return NULL;
        }

        // Détermine le type de token en fonction des quotes
        if (args[i][0] == '\'' && args[i][ft_strlen(args[i]) - 1] == '\'')
        {
            new_token->type = SINGLE_QUOTE;
            // Enlever les quotes du début et de la fin
            char *unquoted = ft_strndup(args[i] + 1, ft_strlen(args[i]) - 2);
            free(new_token->value);
            new_token->value = unquoted;
        }
        else if (args[i][0] == '"' && args[i][ft_strlen(args[i]) - 1] == '"')
        {
            new_token->type = DOUBLEQUOTE;
            // Enlever les quotes du début et de la fin
            char *unquoted = ft_strndup(args[i] + 1, ft_strlen(args[i]) - 2);
            free(new_token->value);
            new_token->value = unquoted;
        }
        else
        {
            new_token->type = STRING;
        }

        new_token->next = NULL;

        if (!head)
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

int	check_consecutive_pipes(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE && (!current->next
				|| current->next->type == TOKEN_PIPE))
		{
			fprintf(stderr, "Error: Consecutive pipes or missing command\n");
			return (-1);
		}
		current = current->next;
	}
	return (0);
}

// int	contains_pipe(t_token *tokens)
// {
// 	while (tokens)
// 	{
// 		if (tokens->type == TOKEN_PIPE)
// 			return (1);
// 		tokens = tokens->next;
// 	}
// 	return (0);
// }

void print_env(char **env_array)
{
    for (int i = 0; env_array[i]; i++) {
        printf("%s\n", env_array[i]);
    }
}

int exec_builtin_cmd(char **args, char **env, t_ctx *ctx)
{
    int i;
    (void)env;

    if (!args || !args[0])
        return 0;
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
        i = 1;
        while (args[i])
        {
            char *var = NULL;
            char *value = NULL;
            split_env_v(args[i], &var, &value);

            if (var && is_valid_id(var))
            {
                if (value)
                {
                    printf("DEBUG - env_vars before export: %p\n", (void*)ctx->env_vars);
                    ctx->env_vars = export_v(ctx->env_vars, var, value);
                    printf("DEBUG - env_vars after export: %p\n", (void*)ctx->env_vars);
                }
                else
                    ctx->env_vars = export_v(ctx->env_vars, var, "");
            }
            free(var);
            free(value);
            i++;
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
    if (ft_strcmp(args[0], "env") == 0)
    {
        char **env_array = convert_env_to_array(ctx);
        print_env(env_array);
        free_tab(env_array);
        return 1;
    }
    return 0; // Not a built-in command
}

// int exec_builtin_cmd(char **args, char **env, t_ctx *ctx)
// {
//     (void)env;

//     if (!args || !args[0])
//         return 0;
//     if (ft_strcmp(args[0], "exit") == 0)
//     {
//         if (process_exit_arg(args, ctx))
//             return 1;
//         return 1;
//     }

//     if (ft_strcmp(args[0], "echo") == 0)
//     {
//         t_token *token_list = create_token_list(args + 1);
//         if (token_list)
//         {
//             handle_echo(token_list, ctx);
//             free_tokens(token_list);
//         }
//         ctx->exit_status = 0;
//         return 1;
//     }

//     if (ft_strcmp(args[0], "export") == 0)
//     {
//         if (!args[1])
//         {
//             t_env_var *current = ctx->env_vars;
//             while (current)
//             {
//                 printf("declare -x %s=\"%s\"\n", current->name, current->value ? current->value : "");
//                 current = current->next;
//             }
//             ctx->exit_status = 0;
//             return 1;
//         }

//         for (int i = 1; args[i]; i++)
//         {
//             char *var = NULL;
//             char *value = NULL;

//             if (!split_env_v(args[i], &var, &value))
//             {
//                 fprintf(stderr, "export: `%s`: not a valid identifier\n", args[i]);
//                 ctx->exit_status = 1;
//                 free(var);
//                 free(value);
//                 continue;
//             }
//             if (!is_valid_id(var))
//             {
//                 fprintf(stderr, "export: `%s`: not a valid identifier\n", var);
//                 free(var);
//                 free(value);
//                 ctx->exit_status = 1;
//                 continue;
//             }

//             if (ft_export(ctx, var, value) == -1)
//                 ctx->exit_status = 1;

//             free(var);
//             free(value);
//         }
//         return 1;
//     }

//     if (ft_strcmp(args[0], "unset") == 0)
//     {
//         if (!args[1])
//         {
//             ctx->exit_status = 0;
//             return 1;
//         }
// 		for (int i = 1; args[i]; i++)
//         {
//             if (!is_valid_id(args[i]))
//             {
//                 fprintf(stderr, "unset: `%s`: not a valid identifier\n", args[i]);
// 				ctx->exit_status = 1;
//                 continue;
//             }
//             unset_v(&(ctx->env_vars), args[i]);
//         }
//         ctx->exit_status = 0;
//         return 1;
//     }
// 	if (ft_strcmp(args[0], "pwd") == 0)
// 	{
// 		if (args[1]) // Check for extra arguments
// 			;
// 		char cwd[1024];
// 		if (getcwd(cwd, sizeof(cwd)) != NULL)
// 		{
// 			printf("%s\n", cwd);
// 			ctx->exit_status = 0; // Success
// 		}
// 		else
// 		{
// 			perror("pwd");
// 			ctx->exit_status = 1; // Failure
// 		}
// 		return 1;
// 	}
//     if (ft_strcmp(args[0], "cd") == 0)
//     {
//         // if (ft_cd(args, ctx) == 0)
//         //     ctx->exit_status = 0;
//         // else
//         //     ctx->exit_status = 1;
//         // return 1;
// 		char **expanded_args = malloc((sizeof(char *) * (count_args(args) + 1)));
// 		if (!expanded_args)
// 		{
// 			perror("malloc failed for expanded_args");
// 			ctx->exit_status = 1;
// 			return 1;
// 		}
// 		// Expansion des arguments
// 		for (int i = 0; args[i]; i++)
// 			expanded_args[i] = expand_variables(args[i], ctx, STRING); // Expansion normale
// 		expanded_args[count_args(args)] = NULL;
// 		// Exécute la commande cd
// 		if (ft_cd(expanded_args, ctx) == 0)
// 			ctx->exit_status = 0;
// 		else
// 			ctx->exit_status = 1;
// 		free_args(expanded_args); // Libérer les arguments expandis
// 		return 1;
// 	}
//     return 0; // Not a built-in command
// }

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

// int read_and_exec(char **env, t_ctx *ctx)
// {
//     char *line;
//     t_token *tokens;
//     int pid; // besoin de fork, read est un processus a part entiere, ici le parent
//     int status = 0;

//     ctx->exit_status = 0;    // Construire les variables d'environnement
//     ctx->env_vars = get_environment(env);
//     if (!ctx->env_vars)
//         return (perror("Failed to initialize environment variables"), 1);
//     while (1)
//     {
//         line = readline(PROMPT);
//         if (line == NULL)
//         {
//             write(1, "exit\n", 5);
//             free_env_vars(ctx->env_vars);
//             exit(ctx->exit_status);
//         }
//         if (*line)
//         {
//             add_history(line);
//             handle_line(line, env, ctx); // Passer `ctx` pour le contexte global
//         }
//         free(line);
//     }
//     // free_env_vars(ctx->env_vars);
//     return (ctx->exit_status);
// }

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

t_env_var *build_env_list(char **envp)
{
    printf("DEBUG - build_env_list start\n");
    t_env_var *head = NULL;
    t_env_var *current = NULL;

    for (int i = 0; envp[i]; i++)
    {
        printf("DEBUG - Processing env: %s\n", envp[i]);
        char *sep = strchr(envp[i], '=');
        if (!sep)
            continue;

        t_env_var *new_var = malloc(sizeof(t_env_var));
        if (!new_var)
        {
            perror("malloc failed");
            free_env(head);
            return NULL;
        }

        // Copier le nom et la valeur
        new_var->name = strndup(envp[i], sep - envp[i]);
        new_var->value = strdup(sep + 1);
        new_var->next = NULL;

        if (!new_var->name || !new_var->value)
        {
            free(new_var->name);
            free(new_var->value);
            free(new_var);
            free_env(head);
            return (perror("malloc failed"), NULL);
        }

        if (!head)
            head = new_var;
        else
            current->next = new_var;

        current = new_var;
    }
    printf("DEBUG - build_env_list end\n");
    return head;
}

// t_ctx *build_env_vars(char **envp)
// {
//     t_ctx *head = NULL;
//     t_ctx *current = NULL;

//     for (int i = 0; envp[i]; i++)
//     {
//         char *sep = strchr(envp[i], '=');
//         if (!sep)
//             continue;
//         t_ctx *new_var = malloc(sizeof(t_ctx));
//         if (!new_var)
//         {
//             perror("malloc failed");
//             free_ctx(head); // Libère tout si erreur
//             return NULL;
//         }
//         new_var->env_vars = NULL;
//         new_var->exit_status = 0;
//         new_var->num_pipes = 0;
//         new_var->oldpwd = NULL;
//         new_var->pwd = NULL;
//         new_var->next = NULL;

//         new_var->name = strndup(envp[i], sep - envp[i]);
//         new_var->value = strdup(sep + 1);
//         if (!new_var->name || !new_var->value)
//         {
//             free(new_var->name);
//             free(new_var->value);
//             free(new_var);
//             free_ctx(head);
//             return (perror("malloc failed"), NULL);
//         }
//         if (!head)
//             head = new_var;
//         else
//             current->next = new_var;
//         current = new_var;
//     }
//     return head;
// }
int loop_with_pipes(char **env, t_ctx *ctx) {
    char *line;
    t_token *tokens;
    int status = 0;
    (void)env;

    while (1) {
        printf("DEBUG - loop_with_pipes: Waiting for user input\n");
        line = readline(PROMPT);
        if (line == NULL) {
            write(1, "exit\n", 5);
            exit(ctx->exit_status);
        }
        if (*line) {
            add_history(line);
            printf("DEBUG - loop_with_pipes: Parsing command line: '%s'\n", line);
            tokens = parse_command_line(line, ctx);
            if (tokens) {
                pid_t pid = fork();
                if (pid == -1) {
                    perror("fork failed");
                    free_tokens(tokens);
                    free(line);
                    continue;
                }

                if (pid == 0) { // Child process
                    char **cmd_env = convert_env_to_array(ctx);
                    if (!cmd_env) {
                        free_tokens(tokens);
                        exit(1);
                    }

                    printf("DEBUG - loop_with_pipes: Validating pipes\n");
                    if (validate_pipes(tokens) == -1) {
                        free_tokens(tokens);
                        free_tab(cmd_env);
                        exit(1);
                    }

                    printf("DEBUG - loop_with_pipes: Executing command sequence\n");
                    if (contains_pipe(tokens))
                        process_pline(tokens, cmd_env, ctx);
                    else
                        exec_simple_cmd(tokens, cmd_env, ctx);

                    free_tab(cmd_env);
                    free_tokens(tokens);
                    exit(ctx->exit_status);
                }

                waitpid(pid, &status, 0);
                if (WIFEXITED(status))
                    ctx->exit_status = WEXITSTATUS(status);
                free_tokens(tokens);
            }
        }
        free(line);
    }
    return ctx->exit_status;
}


// int loop_with_pipes(char **env, t_ctx *ctx)
// {
//     char *line;
//     t_token *tokens;
//     // char **env_array;
//     int status = 0;
//     (void)env;

//     while (1)
//     {
//         line = readline(PROMPT); // Affiche le prompt et lit la commande
//         if (line == NULL) // Gestion de Ctrl+D
//         {
//             write(1, "exit\n", 5);
//             free_ctx(ctx); // Libérer les variables d'environnement
//             exit(ctx->exit_status);
//         }
//         if (*line) // Si la ligne n'est pas vide
//         {
//             add_history(line); // Ajouter à l'historique
//             tokens = parse_command_line(line, ctx); // Parsing de la commande
//             if (tokens)
//             {
//                 int pid = fork();
//                 if (pid == -1)
//                 {
//                     perror("fork failed");
//                     free_tokens(tokens);
//                     free(line);
//                     continue;
//                 }
//                 if (pid == 0) // Processus enfant
//                 {
//                     if (validate_pipes(tokens) == -1)
//                     {
//                         free_tokens(tokens);
//                         exit(1);
//                     }
//                     char **cmd_env;
//                     cmd_env = convert_env_to_array(ctx);
//                     if(!cmd_env)
//                     {
//                         free_tokens(tokens);
//                         exit(1);
//                     }
//                     if (contains_pipe(tokens))
//                         process_pline(tokens, cmd_env, ctx); // Passez directement `env`
//                     else // Pas de pipes, commande simple
//                         exec_simple_cmd(tokens, cmd_env, ctx); // Passez `env->env`
//                     free_tab(cmd_env);
//                     free_tokens(tokens);
//                     exit(ctx->exit_status); // Quitter l'enfant proprement
//                 }

//                 waitpid(pid, &status, 0); // Parent attend la fin de l'enfant
//                 if (WIFEXITED(status))
//                     ctx->exit_status = WEXITSTATUS(status); // Met à jour le code de sortie
//                 free_tokens(tokens);
//             }
//         }
//         free(line); // Libérer la mémoire de la ligne
//     }

//     return ctx->exit_status; // Retourner le code de sortie global
// }


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

// int process_pline(t_token *tokens, char **env, t_ctx *ctx)
// {
//     int pipe_fd[2];
//     int prev_fd = -1;
//     pid_t pid;
//     int status;
//     t_token *cmd_start = tokens;

//     while (cmd_start != NULL)
//     {
//         t_token *cmd_end = cmd_start;
//         while (cmd_end != NULL && cmd_end->type != TOKEN_PIPE)
//             cmd_end = cmd_end->next;

//         if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE)
//         {
//             if (pipe(pipe_fd) == -1)
//                 exit_error();
//         }
//         pid = fork();
//         if (pid == -1)
//             exit_error();

//         if (pid == 0) // Child process
//         {
//             t_token *redir_token = cmd_start;
//             int redirect_success = 1;

//             // Handle redirections
//             while (redir_token != cmd_end)
//             {
//                 if (redir_token->type == TOKEN_REDIRECT_INPUT)
//                 {
//                     int input_fd = open(redir_token->next->value, O_RDONLY);
//                     if (input_fd == -1)
//                     {
//                         if (errno == EACCES)
//                             perror("Permission denied");
//                         else
//                             perror("No such file or directory");
//                         redirect_success = 0;
//                         break;
//                     }
//                     dup2(input_fd, STDIN_FILENO);
//                     close(input_fd);
//                     redir_token = redir_token->next;
//                 }
//                 else if (redir_token->type == TOKEN_REDIRECT_OUTPUT)
//                 {
//                     int output_fd = open(redir_token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//                     if (output_fd == -1)
//                     {
//                         if (errno == EACCES)
//                             perror("Permission denied");
//                         else
//                             perror("Error creating file");
//                         redirect_success = 0;
//                         break;
//                     }
//                     dup2(output_fd, STDOUT_FILENO);
//                     close(output_fd);
//                     redir_token = redir_token->next;
//                 }
//                 else if (redir_token->type == TOKEN_REDIRECT_APPEND)
//                 {
//                     int output_fd = open(redir_token->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
//                     if (output_fd == -1)
//                     {
//                         if (errno == EACCES)
//                             perror("Permission denied");
//                         else
//                             perror("Error opening file");
//                         redirect_success = 0;
//                         break;
//                     }
//                     dup2(output_fd, STDOUT_FILENO);
//                     close(output_fd);
//                     redir_token = redir_token->next;
//                 }
//                 redir_token = redir_token->next;
//             }

//             if (!redirect_success)
//                 exit(EXIT_FAILURE);

//             // Handle pipes
//             if (prev_fd != -1)
//             {
//                 dup2(prev_fd, STDIN_FILENO);
//                 close(prev_fd);
//             }
//             if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE)
//             {
//                 close(pipe_fd[0]);
//                 dup2(pipe_fd[1], STDOUT_FILENO);
//                 close(pipe_fd[1]);
//             }

//             // Execute the command
//             exec_simple_cmd(cmd_start, env, ctx);
//             exit(ctx->exit_status); // Exit with the correct status
//         }

//         // Parent process
//         waitpid(pid, &status, 0);
//         if (WIFEXITED(status))
//             ctx->exit_status = WEXITSTATUS(status);

//         if (prev_fd != -1)
//             close(prev_fd);
//         if (cmd_end != NULL)
//         {
//             close(pipe_fd[1]);
//             prev_fd = pipe_fd[0];
//             cmd_start = cmd_end->next;
//         }
//         else
//         {
//             cmd_start = NULL;
//         }
//     }
//     while (wait(&status) > 0)
//         ; // Wait for all child processes
//     return 0;
// }

t_token *get_next_command(t_token *start)
{
    if (!start)
        return NULL;

    // Avance jusqu'au prochain pipe
    while (start && start->type != TOKEN_PIPE)
        start = start->next;

    // Si on trouve un pipe, retourne la commande suivante
    if (start && start->type == TOKEN_PIPE)
        return start->next;

    return NULL;
}

int execute_pipe_sequence(t_pipe_cmd *cmds, char **env, t_ctx *ctx)
{
    t_pipe_cmd *current = cmds;
    int prev_pipe[2] = {-1, -1};

    while (current)
    {
        int pipe_fd[2] = {-1, -1};

        // Créer un nouveau pipe si ce n'est pas la dernière commande
        if (current->next && pipe(pipe_fd) == -1)
        {
            perror("pipe");
            return -1;
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return -1;
        }

        if (pid == 0) // Processus enfant
        {
            // Gérer l'entrée depuis le pipe précédent
            if (prev_pipe[0] != -1)
            {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }

            // Gérer la sortie vers le prochain pipe
            if (pipe_fd[1] != -1)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            // Gérer les redirections spécifiques à la commande
            if (current->input_fd != -1)
                dup2(current->input_fd, STDIN_FILENO);
            if (current->output_fd != -1)
                dup2(current->output_fd, STDOUT_FILENO);

            // Exécuter la commande
            exec(current->cmd, env, ctx);
            exit(EXIT_FAILURE); // Ne devrait jamais être atteint si exec réussit
        }

        // Processus parent
        // Fermer les descripteurs du pipe précédent
        if (prev_pipe[0] != -1)
        {
            close(prev_pipe[0]);
            close(prev_pipe[1]);
        }

        // Sauvegarder le pipe actuel pour la prochaine itération
        prev_pipe[0] = pipe_fd[0];
        prev_pipe[1] = pipe_fd[1];

        current = current->next;
    }

    // Attendre que tous les processus enfants se terminent
    int status;
    while (wait(&status) > 0)
    {
        if (WIFEXITED(status))
            ctx->exit_status = WEXITSTATUS(status);
    }

    return 0;
}

t_pipe_cmd *create_pipe_cmd(t_token *cmd_tokens)
{
    t_pipe_cmd *new_cmd = malloc(sizeof(t_pipe_cmd));
    if (!new_cmd)
        return NULL;

    new_cmd->cmd = cmd_tokens;
    new_cmd->input_fd = -1;
    new_cmd->output_fd = -1;
    new_cmd->pipe_read = -1;
    new_cmd->pipe_write = -1;
    new_cmd->next = NULL;

    // Parcourir les tokens pour trouver les redirections
    t_token *current = cmd_tokens;
    while (current)
    {
        if (current->type == TOKEN_REDIRECT_INPUT)
        {
            if (current->next)
            {
                new_cmd->input_fd = open(current->next->value, O_RDONLY);
                if (new_cmd->input_fd == -1)
                {
                    perror("open input");
                    free(new_cmd);
                    return NULL;
                }
            }
        }
        else if (current->type == TOKEN_REDIRECT_OUTPUT)
        {
            if (current->next)
            {
                new_cmd->output_fd = open(current->next->value,
                    O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (new_cmd->output_fd == -1)
                {
                    perror("open output");
                    if (new_cmd->input_fd != -1)
                        close(new_cmd->input_fd);
                    free(new_cmd);
                    return NULL;
                }
            }
        }
        current = current->next;
    }

    return new_cmd;
}
// Le troisième paramètre indique quel string libérer (1 = s1, 2 = s2, 3 = les deux)
char *ft_strjoin_free(char *s1, char *s2, int free_str)
{
    char *result = ft_strjoin(s1, s2);

    if (!result)
        return NULL;

    if (free_str & 1) // Si free_str est 1 ou 3
        free(s1);
    if (free_str & 2) // Si free_str est 2 ou 3
        free(s2);

    return result;
}

char *ft_strjoin_char(char *str, char c)
{
    size_t len = ft_strlen(str);
    char *result = malloc(len + 2); // +1 pour le nouveau char, +1 pour \0

    if (!result)
        return NULL;

    strcpy(result, str);
    result[len] = c;
    result[len + 1] = '\0';

    free(str); // Libère la chaîne d'origine
    return result;
}

int	extract_var_name(char *var_start, char *var_name)
{
	int	j;

	j = 0;
	while (ft_isalpha(var_start[j]) || var_start[j] == '_')
		j++;
	ft_strncpy(var_name, var_start, j);
	var_name[j] = '\0';
	return (j);
}

void	append_env_value(char **result, char *env_value)
{
	char	*tmp;

	tmp = *result;
	if (env_value != NULL)
	{
		if (*result != NULL)
			*result = ft_strjoin(*result, env_value);
		else
			*result = ft_strjoin("", env_value);
	}
	else
	{
		if (*result != NULL)
			*result = ft_strjoin(*result, "");
		else
			*result = ft_strjoin("", "");
	}
	free(tmp);
}

int	append_character(char c, char **result)
{
	char	single_char[2];
	char	*tmp;

	single_char[0] = c;
	single_char[1] = '\0';
	tmp = *result;
	if (*result != NULL)
		*result = ft_strjoin(*result, single_char);
	else
		*result = ft_strjoin("", single_char);
	free(tmp);
	return (1);
}

char *find_in_env(char *name, t_env_var *env_vars)
{
    t_env_var *current = env_vars;

    while (current)
    {
        if (ft_strcmp(current->name, name) == 0)
            return (current->value);
        current = current->next;
    }
    return (NULL);
}

int	expand_variable(char *token, char **result, int i, t_ctx *ctx)
{
	char	var_name[256];
	char	*var_start;
	char	*env_value;
	int		j;

	var_start = &token[i + 1];
	j = extract_var_name(var_start, var_name);
	if (j > 0)
	{
		env_value = find_in_env(var_name, ctx->env_vars);
		append_env_value(result, env_value);
		return (j + 1);
	}
	return (1);
}

void	ps_expand_env(t_token *tokens, t_ctx *ctx)
{
	char	*token;
	char	*result;
	int		i;

	(void)ctx;
	while (tokens)
	{
		token = tokens->value;
		result = NULL;
		i = 0;
		while (token[i] != '\0')
		{
			if (token[i] == '$')
				i += expand_variable(token, &result, i, ctx);
			else
				i += append_character(token[i], &result);
		}
		free(tokens->value);
		tokens->value = result;
		tokens = tokens->next;
	}
}

/// nv
int process_pline(t_token *tokens, char **env, t_ctx *ctx)
{
    int     pipe_fd[2], prev_fd = -1;
    t_token *cmd_start = tokens;
    char    **args;
    char	**env_array;
    (void)env;

	env_array = convert_env_to_array(ctx);
	if(!env_array)
		return(perror("Erreur de conversion de l'env"), 0);
    ps_expand_env(tokens, ctx);
    args = prepare_args(tokens, ctx);
    if (!args)
        return (perror("Erreur d'allocation de mémoire"), 0);
    if (exec_builtin_cmd(args, env_array, ctx))
	{
        ps_expand_env(tokens, ctx);
        free_tab_2(args);
    }
    while (cmd_start != NULL)
	{
        t_token *cmd_end = cmd_start;
        while (cmd_end != NULL && cmd_end->type != TOKEN_PIPE)
            cmd_end = cmd_end->next;
        initialize_pipe_if_needed(pipe_fd, cmd_end);
        execute_command_in_child(cmd_start, cmd_end, prev_fd, pipe_fd, env_array, ctx);
        cleanup_parent_resources(&prev_fd, pipe_fd, &cmd_start, cmd_end);
    }
    wait_for_all_children();
    return 0;
}

// int process_pline(t_token *tokens, char **env, t_ctx *ctx)
// {
//     t_pipe_cmd *cmds = NULL;
//     t_pipe_cmd *current = NULL;
//     t_token *cmd_start = tokens;

//     while (cmd_start)
//     {
//         t_pipe_cmd *new_cmd = create_pipe_cmd(cmd_start);
//         if (!new_cmd)
//             return -1;

//         if (!cmds)
//             cmds = new_cmd;
//         else
//             current->next = new_cmd;
//         current = new_cmd;

//         cmd_start = get_next_command(cmd_start);
//     }

//     int ret = execute_pipe_sequence(cmds, env, ctx);

//     // Nettoyer la mémoire
//     while (cmds)
//     {
//         t_pipe_cmd *tmp = cmds;
//         cmds = cmds->next;
//         if (tmp->input_fd != -1)
//             close(tmp->input_fd);
//         if (tmp->output_fd != -1)
//             close(tmp->output_fd);
//         free(tmp);
//     }

//     return ret;
// }

// int process_pline(t_token *tokens, char **env, t_ctx *ctx)
// {
//     int pipe_fd[2];
//     int prev_fd = -1;
//     t_token *cmd_start = tokens;

//     while (cmd_start != NULL)
//     {
//         t_token *cmd_end = cmd_start;
//         while (cmd_end != NULL && cmd_end->type != TOKEN_PIPE)
//             cmd_end = cmd_end->next;

//         initialize_pipe_if_needed(pipe_fd, cmd_end);

//         execute_command_in_child(cmd_start, cmd_end, prev_fd, pipe_fd, env, ctx);

//         cleanup_parent_resources(&prev_fd, pipe_fd, &cmd_start, cmd_end);
//     }

//     wait_for_all_children();
//     return 0;
// }


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

char *find_in_env_array(char *name, char **env)
{
    printf("DEBUG - find_in_env_array: searching for %s\n", name);
    int i;
    int len;

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
	char	*full_path;
	char	*path_with_slash;

	path_with_slash = ft_strjoin(path, "/");
	full_path = ft_strjoin(path_with_slash, cmd);
	free(path_with_slash);
	return (full_path);
}

// char	*join_path_cmd(char *path, char *cmd)
// {

//     char *full_path;
//     char *path_with_slash = malloc(strlen(path) + 2);

//     if (!path_with_slash)
//         return (NULL);
//     sprintf(path_with_slash, "%s/", path);
//     full_path = malloc(ft_strlen(path_with_slash) + ft_strlen(cmd) + 1);
//     if (!full_path)
//     {
//         free(path_with_slash);
//         return (NULL);
//     }
//     sprintf(full_path, "%s%s", path_with_slash, cmd);
//     free(path_with_slash);
//     return (full_path);
// }

// char *get_path(char *cmd, char **env)
// {
//     char **paths;
//     char **s_cmd;
//     char *full_path;
//     int i;

//     i = 0;
//     s_cmd = ft_split(cmd, ' ');
//     paths = ft_split(find_in_env_array("PATH", env), ':');
//     while (paths[i])
//     {
//         full_path = join_path_cmd(paths[i], s_cmd[0]);
//         if (access(full_path, F_OK | X_OK) == 0)
//             return (free_tab(paths), free_tab(s_cmd), full_path);
//         free(full_path);
//         i++;
//     }
//     free_tab(paths);
//     free_tab(s_cmd);
//     return (cmd);
// }

char *get_path(char *cmd, char **env)
{
    // Si la commande contient un '/', c'est un chemin absolu/relatif
    if (strchr(cmd, '/'))
        return ft_strdup(cmd);

    char **paths;
    char **s_cmd;
    char *full_path;
    int i = 0;

    // Découper la commande pour avoir juste le premier mot
    s_cmd = ft_split(cmd, ' ');
    if (!s_cmd)
        return ft_strdup(cmd);

    // Trouver et découper le PATH
    char *path_value = find_in_env_array("PATH", env);
    if (!path_value)
    {
        free_tab(s_cmd);
        return ft_strdup(cmd);
    }

    paths = ft_split(path_value, ':');
    if (!paths)
    {
        free_tab(s_cmd);
        return ft_strdup(cmd);
    }

    // Essayer chaque chemin
    while (paths[i])
    {
        full_path = join_path_cmd(paths[i], s_cmd[0]);
        if (access(full_path, F_OK | X_OK) == 0)
        {
            free_tab(paths);
            free_tab(s_cmd);
            return full_path;
        }
        free(full_path);
        i++;
    }

    free_tab(paths);
    free_tab(s_cmd);
    return ft_strdup(cmd);
}

void    read_heredoc(int fd, char *limiter)
{
	char	*buf;

	while(1)
	{
		buf = NULL;
		buf = readline("> ");
		if (!buf)
		{
			exit_error();
			break ;
		}
		if (!ft_strncmp(limiter, buf, INT_MAX))
			break ;
		write(fd, buf, ft_strlen(buf));
		write(fd, "\n", 1);
		free(buf);
	}
	free(buf);
	close(fd);
}

int here_doc(char *limiter)
{
    int fd;

    fd = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0)
        exit_error();
    read_heredoc(fd, limiter);
    fd = open(".heredoc.tmp", O_RDONLY);
    if (fd >= 0)
        unlink(".heredoc.tmp");
    return fd;
}


// void handle_input_redirection(t_token *redir_token, int *redirect, int *redirect_input)
// {
//     int input_fd;

//     if (redir_token->type == TOKEN_HEREDOC) {
//         input_fd = here_doc(redir_token->next->value);
//     } else {
//         input_fd = open(redir_token->next->value, O_RDONLY);
//     }
//     //input_fd = open(redir_token->next->value, O_RDONLY);
//     if (input_fd == -1)
//         exit_error();
//     dup2(input_fd, STDIN_FILENO);
//     close(input_fd);
//     *redirect = 1;
//     *redirect_input = 1;
// }

void handle_input_redirection(t_token *redir_token, int *redirect, int *redirect_input)
{
    int input_fd;

    if (redir_token->type == TOKEN_HEREDOC)
        input_fd = here_doc(redir_token->next->value); // Process heredoc and get input FD
    else
    {
        input_fd = open(redir_token->next->value, O_RDONLY); // Open file for input redirection
    }
    if (input_fd == -1)
    {
        if (errno == EACCES)
            fprintf(stderr, "minishell: %s: Permission denied\n", redir_token->next->value);
        else
        {
            fprintf(stderr, "minishell: %s: No such file or directoryyyyyy\n", redir_token->next->value);
        }

        return;
    }
    dup2(input_fd, STDIN_FILENO); // Redirect input to STDIN
    close(input_fd);
    *redirect = 1;
    *redirect_input = 1;
}

// void handle_output_redirection(t_token *redir_token, int *redirect, int *redirect_output)
// {
//     int output_fd;
//     int flags = O_WRONLY | O_CREAT;

//     flags |= (redir_token->type == TOKEN_REDIRECT_APPEND) ? O_APPEND : O_TRUNC;

//     output_fd = open(redir_token->next->value, flags, 0644);
//     if (output_fd == -1)
//     {
//         if (errno == EACCES)
//             perror("Permission denied");
//         else
//             perror("Error creating file");
//         exit(EXIT_FAILURE);
//     }
//     dup2(output_fd, STDOUT_FILENO);
//     close(output_fd);
//     *redirect = 1;
//     *redirect_output = 1;
// }


// void collect_exec_tokens(t_token *cmd_start, t_token *cmd_end, t_token **exec_tokens, int *redirect, int *redirect_input, int *redirect_output)
// {
//     t_token **exec_tokens_tail = exec_tokens;
//     t_token *redir_token = cmd_start;

//     while (redir_token != cmd_end)
// 	{
//         if (redir_token->type == TOKEN_REDIRECT_INPUT || redir_token->type == TOKEN_HEREDOC)
// 		{
//             handle_input_redirection(redir_token, redirect, redirect_input);
//             redir_token = redir_token->next;
//         }
// 		else if (redir_token->type == TOKEN_REDIRECT_OUTPUT)
// 		{
//             handle_output_redirection(redir_token, redirect, redirect_output);
//             redir_token = redir_token->next;
//         }
//         else
// 		{
//             *exec_tokens_tail = redir_token;
//             exec_tokens_tail = &(*exec_tokens_tail)->next;
//         }
//         redir_token = redir_token->next;
//     }
//     *exec_tokens_tail = NULL;
// }


void collect_exec_tokens(t_token *cmd_start, t_token *cmd_end, t_token **exec_tokens, int *redirect, int *redirect_input, int *redirect_output)
{
    t_token *redir_token = cmd_start;
    t_token **exec_tail = exec_tokens;

    while (redir_token != cmd_end)
    {
        if (redir_token->type == TOKEN_REDIRECT_INPUT || redir_token->type == TOKEN_HEREDOC)
        {
            handle_input_redirection(redir_token, redirect, redirect_input);
            redir_token = redir_token->next; // Skip the redirection token and target
        }
        else if (redir_token->type == TOKEN_REDIRECT_OUTPUT || redir_token->type == TOKEN_REDIRECT_OUTPUT)
        {
            handle_output_redirection(redir_token, redirect, redirect_output);
            redir_token = redir_token->next; // Skip the redirection token and target
        }
        else
        {
            *exec_tail = redir_token; // Add token to exec list
            exec_tail = &(*exec_tail)->next;
        }
        redir_token = redir_token->next;
    }

    *exec_tail = NULL; // Terminate the exec tokens list
}


// void setup_pipe_for_child(int prev_fd, int *pipe_fd, int redirect_input, int redirect_output, t_token *cmd_end)
// {
//     if (prev_fd != -1 && redirect_input != 1)
// 	{
//         dup2(prev_fd, STDIN_FILENO);
//         close(prev_fd);
//     }
//     if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE && redirect_output != 1)
// 	{
//         close(pipe_fd[0]);
//         dup2(pipe_fd[1], STDOUT_FILENO);
//         close(pipe_fd[1]);
//     }
// }


void setup_pipe_for_child(int prev_fd, int *pipe_fd, int redirect_input, int redirect_output, t_token *cmd_end)
{
    if (prev_fd != -1 && !redirect_input)
    {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }
    if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE && !redirect_output)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
}

// void initialize_pipe_if_needed(int *pipe_fd, t_token *cmd_end)
// {
//     if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE) {
//         if (pipe(pipe_fd) == -1)
//             exit_error();
//     }
// }

void initialize_pipe_if_needed(int pipe_fd[2], t_token *cmd_end)
{
    if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE)
    {
        if (pipe(pipe_fd) == -1)
        {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }
    }
}


// void initialize_pipe_if_needed(int *pipe_fd, t_token *cmd_end)
// {
//     if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE)
//     {
//         if (pipe(pipe_fd) == -1)
//         {
//             perror("pipe failed");
//             exit(EXIT_FAILURE);
//         }
//     }
// }

// void execute_command_in_child(t_token *cmd_start, t_token *cmd_end, int prev_fd, int *pipe_fd, char **env, t_ctx *ctx)
// {
//     pid_t pid = fork();
//     if (pid == -1)
//         exit_error();

//     if (pid == 0) {
//         t_token *exec_tokens = NULL;
//         int redirect = 0, redirect_output = 0, redirect_input = 0;

//         collect_exec_tokens(cmd_start, cmd_end, &exec_tokens, &redirect, &redirect_input, &redirect_output);
//         setup_pipe_for_child(prev_fd, pipe_fd, redirect_input, redirect_output, cmd_end);
//         exec(exec_tokens, env, ctx);
//         exit_error();
//     }
// }

// void execute_command_in_child(t_token *cmd_start, t_token *cmd_end, int prev_fd, int *pipe_fd, char **env, t_ctx *ctx)
// {
//     pid_t pid = fork();
//     if (pid == -1)
//         exit_error();

//     if (pid == 0) {
//         t_token *exec_tokens = NULL;
//         int redirect = 0, redirect_output = 0, redirect_input = 0;

//         // Collect exec tokens and handle redirections
//         collect_exec_tokens(cmd_start, cmd_end, &exec_tokens, &redirect, &redirect_input, &redirect_output);

//         // Set up pipes and redirections for child
//         setup_pipe_for_child(prev_fd, pipe_fd, redirect_input, redirect_output, cmd_end);

//         // Execute the command
//         exec(exec_tokens, env, ctx);
//         exit_error();
//     }
// }


void cleanup_parent_resources(int *prev_fd, int pipe_fd[2], t_token **cmd_start, t_token *cmd_end)
{
    if (*prev_fd != -1)
        close(*prev_fd); // Close the previous pipe's read end

    if (cmd_end != NULL) // Move to the next command
    {
        close(pipe_fd[1]); // Close write end of the current pipe
        *prev_fd = pipe_fd[0]; // Save read end for the next child
        *cmd_start = cmd_end->next; // Advance to the next command
    }
    else
    {
        *cmd_start = NULL; // No more commands
    }
}




void wait_for_all_children(void)
{
    int status;
    while (wait(&status) > 0)
        ; // Wait for all child processes
}


// char *get_path(char *cmd, char **env)
// {
//     char **paths;
//     char *full_path;
//     int i;
//     // printf("%s\n", cmd);
//     // Extraire uniquement la commande sans les arguments
//     char **split_cmd = ft_split(cmd, ' ');  // Séparer les mots de la commande
//     char *command_only = split_cmd[0];      // Prendre seulement la commande

//     paths = ft_split(find_in_env("PATH", env), ':');
//     i = 0;
//     while (paths[i])
//     {
//         full_path = join_path_cmd(paths[i], command_only);  // Chercher uniquement la commande
//         // printf("Checking path: %s\n", full_path);  // Debug pour vérifier les chemins testés
//         if (access(full_path, F_OK | X_OK) == 0)
//         {
//             free_tab(paths);
//             free_tab(split_cmd);
//             return full_path;  // Retourner le chemin complet si la commande est trouvée
//         }
//         free(full_path);
//         i++;
//     }
//     free_tab(paths);
//     free_tab(split_cmd);
//     return command_only;  // Si aucun chemin n'est trouvé, retourner la commande brute
// }

char **prepare_args(t_token *tokens, t_ctx *ctx)
{
   (void)ctx;
   int count = 0;
   t_token *current = tokens;
   char **args;
   int i = 0;

//    printf("\n=== Debug Tokens ===\n");
//    t_token *debug = tokens;
//    while (debug)
//    {
//        printf("Token type: '%c', Value: '%s'\n",
//               (char)debug->type, debug->value);
//        debug = debug->next;
//    }
//    printf("=================\n\n");
   while (current)
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
   while (current)
   {
    //    printf("Processing token %d: type = '%c'\n", i, (char)current->type);

       if (current->type == '\'') // Vérifie explicitement la quote simple
       {
        //    printf("Found single quote: using value '%s' without expansion\n", current->value);
           args[i] = ft_strdup(current->value);
       }
       else if (current->type == '"') // Vérifie explicitement la quote double
       {
        //    printf("Found double quote: expanding value '%s'\n", current->value);
           args[i] = expand_variables(current->value, ctx, current->type);
       }
       else // Cas normal (STRING ou autres)
       {
        //    printf("Found normal token: expanding value '%s'\n", current->value);
           args[i] = expand_variables(current->value, ctx, current->type);
       }

       if (!args[i])
       {
           // Gestion d'erreur...
           while (i > 0)
               free(args[--i]);
           free(args);
           return NULL;
       }
       i++;
       current = current->next;
   }
   args[i] = NULL;
   // Debug du résultat final
//    printf("\n=== Final Args ===\n");
//    for (int j = 0; args[j] != NULL; j++)
//    {
//        printf("arg[%d]: '%s'\n", j, args[j]);
//    }
//    printf("=================\n\n");
   return args;
}

//nv
void handle_output_redirection(t_token *redir_token, int *redirect, int *redirect_output)
{
    int output_fd;
    int flags = O_WRONLY | O_CREAT;

    flags |= (redir_token->type == TOKEN_REDIRECT_APPEND) ? O_APPEND : O_TRUNC;

    output_fd = open(redir_token->next->value, flags, 0644);
    if (output_fd == -1)
        exit_error();
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);
    *redirect = 1;
    *redirect_output = 1;
}
//nv
void execute_command_in_child(t_token *cmd_start, t_token *cmd_end, int prev_fd, int *pipe_fd, char **env, t_ctx *ctx)
{
    pid_t pid = fork();
    if (pid == -1)
        exit_error();

    if (pid == 0) {
        t_token *exec_tokens = NULL;
        int redirect = 0, redirect_output = 0, redirect_input = 0;

        collect_exec_tokens(cmd_start, cmd_end, &exec_tokens, &redirect, &redirect_input, &redirect_output);
        setup_pipe_for_child(prev_fd, pipe_fd, redirect_input, redirect_output, cmd_end);
        exec(exec_tokens, env, ctx);
        exit_error();
    }
}

int is_builtin(char *cmd)
{
    char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
    int i = 0;

    while (builtins[i])
    {
        if (strcmp(cmd, builtins[i]) == 0)
            return 1;
        i++;
    }
    return 0;
}

// int exec(t_token *cmd_tokens, char **env, t_ctx *ctx)
// {
//     char **option_cmd;
//     char *path;
//     struct stat path_stat;
//     // char **env_array;
//     (void)env;
// 	char **env_array;

// 	env_array = convert_env_to_array(ctx);
// 	if(!env_array)
// 	{
// 		fprintf(stderr, "Error : Failed to convert env\n");
// 		exit(EXIT_FAILURE);
// 	}
//     option_cmd = prepare_args(cmd_tokens, ctx);
//     if (!option_cmd || !option_cmd[0])
//     {
//         fprintf(stderr, "Error: Command is empty\n");
//         free_tab(option_cmd);
//         exit(EXIT_FAILURE);
//     }
//     // if (!env_array)
//     // {
//     //     fprintf(stderr, "Error: Failed to convert environment\n");
//     //     free_tab(option_cmd);
//     //     exit(EXIT_FAILURE);
//     // }

//     // Check if the command is a builtin
//     if (is_builtin(option_cmd[0]))
//     {
// 		// char **env_array = convert_env_to_array(ctx);
// 		// if(!env_array)
// 		// {
// 		// 	free_tab(option_cmd);
// 		// 	exit(EXIT_FAILURE);
// 		// }
//         // Execute the builtin command
//         exec_builtin_cmd(option_cmd, env_array, ctx);
//         free_tab(option_cmd);
//         free_tab(env_array);
//         exit(ctx->exit_status);
//     }

//     // Find the command in PATH
//     path = get_path(option_cmd[0], env_array);
//     if (!path)
//     {
//         fprintf(stderr, "%s: command not found\n", option_cmd[0]);
//         free_tab(option_cmd);
//         free_tab(env_array);
//         exit(127);
//     }
//     if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
//     {
//         fprintf(stderr, "%s: Is a directory\n", path);
//         free_tab(option_cmd);
//         free_tab(env_array);
//         exit(126);
//     }

//     if (access(path, X_OK) != 0)
//     {
//         fprintf(stderr, "%s: Permission denied\n", path);
//         free_tab(option_cmd);
//         free_tab(env_array);
//         exit(126);
//     }

//     if (execve(path, option_cmd, env_array) == -1)
//     {
//         perror("execve");
//         free_tab(option_cmd);
//         free_tab(env_array);
//         exit(EXIT_FAILURE);
//     }

//     free_tab(option_cmd);
//     free_tab(env_array);
//     return 0;
// }

int exec(t_token *cmd_tokens, char **env, t_ctx *ctx)
{
    char **option_cmd;
    char *path;
    char **env_array;
	(void)env;

    // Convertir les tokens en tableau d'arguments
    option_cmd = prepare_args(cmd_tokens, ctx);
    if (!option_cmd || !option_cmd[0])
    {
        fprintf(stderr, "Error: Command is empty\n");
        free_tab(option_cmd);
        exit(EXIT_FAILURE);
    }

    // Convertir env_vars en tableau
    env_array = convert_env_to_array(ctx);
    if (!env_array)
    {
        free_tab(option_cmd);
        exit(EXIT_FAILURE);
    }

    // Vérifier si c'est une commande builtin
    if (is_builtin(option_cmd[0]))
    {
        exec_builtin_cmd(option_cmd, env_array, ctx);
        free_tab(option_cmd);
        free_tab(env_array);
        exit(ctx->exit_status);
    }

    // Si ce n'est pas un builtin, chercher dans PATH
    path = get_path(option_cmd[0], env_array);
    if (!path)
    {
        fprintf(stderr, "%s: command not found\n", option_cmd[0]);
        free_tab(option_cmd);
        free_tab(env_array);
        exit(127);
    }

    // Vérifier si c'est un dossier
    struct stat path_stat;
    if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
    {
        fprintf(stderr, "%s: Is a directory\n", path);
        free(path);
        free_tab(option_cmd);
        free_tab(env_array);
        exit(126);
    }

    // Vérifier les permissions
    if (access(path, X_OK) != 0)
    {
        fprintf(stderr, "%s: Permission denied\n", path);
        free(path);
        free_tab(option_cmd);
        free_tab(env_array);
        exit(126);
    }

    // Exécuter la commande
    execve(path, option_cmd, env_array);

    // Si on arrive ici, execve a échoué
    perror("execve");
    free(path);
    free_tab(option_cmd);
    free_tab(env_array);
    exit(EXIT_FAILURE);
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
        free(tmp->content);
		free(tmp);
	}
}
t_ctx *initialize_ctx(void)
{
    // Allouer de la mémoire pour la structure t_ctx
    t_ctx *ctx = (t_ctx *)malloc(sizeof(t_ctx));
    if (!ctx)
    {
        perror("Failed to allocate memory for t_ctx");
        return NULL;
    }

    // Initialiser les champs de la structure
    ctx->env_vars = NULL;           // Liste des variables d'environnement initialement vide
    ctx->exit_status = 0;           // Le code de sortie est initialisé à 0 (succès)
    ctx->num_pipes = 0;             // Initialiser le nombre de pipes à 0
    ctx->oldpwd = NULL;             // Initialiser l'ancien répertoire à NULL
    ctx->pwd = getcwd(NULL, 0);     // Obtenir le répertoire actuel
    if (!ctx->pwd)
    {
        perror("Failed to get current working directory");
        free(ctx);                  // Libérer la mémoire si une erreur survient
        return NULL;
    }

    return ctx;
}



int main(int argc __attribute__((unused)), char **argv __attribute__((unused)), char **envp)
{
    printf("DEBUG - main start\n");
    // t_env_var myEnv; // Déclarez une instance de `t_var`
    t_ctx *ctx = initialize_ctx(); // Initialisez `t_ctx`

    if (!ctx)
        return (perror("Failed to initialize context"), 1);

    ctx->env_vars = build_env_list(envp);
    // char **env_array = convert_env_to_array(ctx);
    printf("DEBUG - env_vars built: %p\n", (void*)ctx->env_vars);
    if(!ctx->env_vars)
	{
		free_ctx(ctx);
		return(perror("Failed to build env list"), 1);
	}

	loop_with_pipes(envp, ctx);
    // Nettoyage
    free_ctx(ctx);
    free(ctx);
    return 0;
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
//     // read_and_exec(env_copy, &ctx);
// 	// loop(env_copy, &ctx);
//     loop_with_pipes(env, &ctx);
//     free_env_copy(env_copy);
//     free_env_vars(ctx.env_vars);
//     return ctx.exit_status;
// }


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

void free_env(t_env_var *env_var)
{
    while(env_var)
    {
        t_env_var *to_free = env_var;
        env_var = env_var->next;

        free(to_free->name);
        free(to_free->value);
        free(to_free);
    }

}

// Libère une liste chaînée  de variables d'environnement
void free_ctx(t_ctx *ctx)
{
    while (ctx)
    {
        t_ctx *to_free = ctx;
        ctx = ctx->next;

        free_env(to_free->env_vars);

        free(to_free->oldpwd);
        free(to_free->pwd);
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

void	*free_tab_2(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}