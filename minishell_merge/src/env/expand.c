/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:52:38 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 13:25:03 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Fonction pour obtenir la partie avant le caractère `$`
char	*ps_get_before_env(char *str, char *found)
{
	return (ft_strndup(str, found - str));
}

// Fonction pour obtenir la valeur de la variable d'environnement
// ou une chaîne vide si elle n'existe pas
char	*ps_get_env_var(char *var_name, t_ctx *ctx, char **env)
{
	char	*env_value;

	if (ft_strcmp(var_name, "?") == 0)
	{
		return (ft_itoa(ctx->exit_status));
	}
	env_value = find_in_env(var_name, env);
	if (env_value != NULL)
		return (ft_strdup(env_value));
	else
		return (ft_strdup(""));
}

// Fonction pour obtenir la partie après le nom de la variable
char	*ps_get_after_env(char *found)
{
	int	i;

	i = 0;
	while (found[i] && (ft_isalnum(found[i]) || found[i] == '_'))
		i++;
	return (ft_strdup(found + i));
}

// Convertit une chaîne contenant une variable
// d'environnement en une chaîne avec la valeur étendue
// ctx =  contexte d'exécution, utilisé pour stocker des informations
// globales ou partagées, comme l'état actuel
// d'exécution, les variables d'environnement, le statut de sortie, etc.
char	*ps_convert_to_env(char *str, char *found, t_ctx *ctx)
{
	char	*before_env;
	char	*after_env;
	char	*final_result;
	char	*env_var;
	char	*new;

	before_env = ps_get_before_env(str, found);
	env_var = ps_get_env_var(found + 1, ctx, &before_env);
	after_env = ps_get_after_env(found + 1);
	if (!before_env || !env_var || !after_env)
	{
		if (before_env)
			free(before_env);
		if (env_var)
			free(env_var);
		if (after_env)
			free(after_env);
		return (NULL);
	}
	new = ft_strjoin(before_env, env_var);
	free(before_env);
	free(env_var);
	if (!new)
		return (free(after_env), NULL);
	final_result = ft_strjoin(new, after_env);
	return (free(after_env), free(new), final_result);
}

// Compte les occurrences de `$` dans une chaîne
int	ft_count_exp(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '$')
			count++;
		i++;
	}
	return (count);
}

// Gère l'expansion des variables d'environnement dans un token
int	ps_handle_env(t_token *token, t_ctx *ctx)
{
	int		nb;
	char	*tmp;
	char	*found;
	char	*new;

	nb = ft_count_exp(token->value);
	while (token->value && nb > 0)
	{
		found = ft_strchr(token->value, '$');
		if (!found)
			break ;
		tmp = token->value;
		new = ps_convert_to_env(token->value, found, ctx);
		if (new && new[0] == '\0')
			token->value = NULL;
		else
			token->value = new;
		free(tmp);
		nb--;
	}
	return (0);
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

// char *find_env_value(const char *name, t_env_var *env_vars)
// {
//     while (env_vars)
//     {
//         if (strcmp(env_vars->name, name) == 0)
//             return env_vars->value;
//         env_vars = env_vars->next;
//     }
//     return NULL; // Retourne NULL si aucune correspondance n'est trouvée
// }

// char *expand_variables(const char *str, t_ctx *ctx, t_token_type token_type)
// {
//     // Pour les single quotes, retourner la chaîne telle quelle sans expansion
//     if (token_type == SINGLE_QUOTE)
//     {
//         return ft_strdup(str);
//     }

//     char *expanded = ft_strdup("");
//     char *temp;

//     while (*str)
//     {
//         if (*str == '$' && token_type != SINGLE_QUOTE) // Ne jamais expandre dans les single quotes
//         {
//             str++; // Skip the '$'
//             if (*str == '?') // Special case for `$?`
//             {
//                 char *exit_code = ft_itoa(ctx->exit_status);
//                 temp = ft_strjoin(expanded, exit_code);
//                 free(expanded);
//                 free(exit_code);
//                 expanded = temp;
//                 str++;
//             }
//             else if (ft_isalnum(*str) || *str == '_') // Valid variable name
//             {
//                 char buffer[1024];
//                 int i = 0;
//                 while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
//                 {
//                     buffer[i] = str[i];
//                     i++;
//                 }
//                 buffer[i] = '\0';
//                 str += i;

//                 char *value = find_env_value(buffer, ctx->env_vars);
//                 temp = ft_strjoin(expanded, value ? value : "");
//                 free(expanded);
//                 expanded = temp;
//             }
//             else // Invalid variable, keep the $
//             {
//                 temp = ft_strjoin(expanded, "$");
//                 free(expanded);
//                 expanded = temp;
//                 if (*str && *str != ' ') // Si ce n'est pas un espace
//                 {
//                     char curr[2] = {*str, '\0'};
//                     temp = ft_strjoin(expanded, curr);
//                     free(expanded);
//                     expanded = temp;
//                     str++;
//                 }
//             }
//         }
//         else
//         {
//             char curr[2] = {*str, '\0'};
//             temp = ft_strjoin(expanded, curr);
//             free(expanded);
//             expanded = temp;
//             str++;
//         }
//     }
//     return expanded;
// }

char *find_env_value(const char *name, t_env_var *env_vars)
{
    if (!name || !env_vars)
        return NULL;
    t_env_var *current = env_vars;
    while (current)
    {
        if (current->name && ft_strcmp(current->name, name) == 0)
            return current->value ? current->value : "";
        current = current->next;
    }
    return NULL;
}
char *expand_variables(const char *str, t_ctx *ctx, t_token_type token_type)
{
    if (!str)
        return ft_strdup("");

    // Pour les single quotes, retourner la chaîne telle quelle
    if (token_type == SINGLE_QUOTE)
        return ft_strdup(str);

    // Pour $ seul
    if (ft_strcmp(str, "$") == 0)
        return ft_strdup("$");

    char *result = malloc(4096);
    if (!result)
        return ft_strdup("");

    int j = 0;
    for (int i = 0; str[i]; i++)
    {
        if (str[i] == '$' && token_type != SINGLE_QUOTE)
        {
            i++;
            if (!str[i]) // $ en fin de chaîne
            {
                result[j++] = '$';
                break;
            }
            else if (str[i] == '?')
            {
                char *exit_str = ft_itoa(ctx ? ctx->exit_status : 0);
                if (exit_str)
                {
                    strcpy(result + j, exit_str);
                    j += strlen(exit_str);
                    free(exit_str);
                }
            }
            else if (ft_isalnum(str[i]) || str[i] == '_')
            {
                char var_name[256] = {0};
                int k = 0;
                while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                    var_name[k++] = str[i++];
                i--;
                var_name[k] = '\0';

                char *value = find_env_value(var_name, ctx ? ctx->env_vars : NULL);
                if (value)
                {
                    strcpy(result + j, value);
                    j += strlen(value);
                }
            }
            else // $ suivi d'un caractère spécial
            {
                result[j++] = '$';
                result[j++] = str[i];
            }
        }
        else
            result[j++] = str[i];
    }
    result[j] = '\0';

    char *final = ft_strdup(result);
    free(result);
    return final ? final : ft_strdup("");
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

// void	ps_expand_env(t_token *tokens, t_ctx *ctx, t_var *myEnv)
// {
// 	char	*token;
// 	char	*result;
// 	int		i;

// 	(void)ctx;
// 	while (tokens)
// 	{
// 		token = tokens->value;
// 		result = NULL;
// 		i = 0;
// 		while (token[i] != '\0')
// 		{
// 			if (token[i] == '$')
// 				i += expand_variables(token, &result, i, myEnv);
// 			else
// 				i += append_character(token[i], &result);
// 		}
// 		free(tokens->value);
// 		tokens->value = result;
// 		tokens = tokens->next;
// 	}
// }


// void ps_expand_env(t_token *tokens, t_ctx *ctx)
// {
//     (void)ctx;

//     while (tokens)
//     {
//         char *token = tokens->value;
//         char *result = NULL;
//         int i = 0;
//         while (token[i] != '\0') {
//             if (token[i] == '$') {
//                 char *var_start = &token[i + 1];
//                 int j = 0;

//                 // Identifie la fin de la variable
//                 while (isalpha(var_start[j]) || var_start[j] == '_') {
//                     j++;
//                 }
//                 if (j > 0) {
//                     // Copie le nom de la variable et récupère sa valeur
//                     char var_name[j + 1];
//                     strncpy(var_name, var_start, j);
//                     var_name[j] = '\0';
//                     char *env_value = getenv(var_name);

//                     // Concatène la valeur de la variable si elle est définie
//                     char *tmp = result;
//                     result = ft_strjoin(result ? result : "", env_value ? env_value : "");
//                     free(tmp);
//                     // Avance l'index pour sauter le nom de la variable
//                     i += j + 1;
//                 } else {
//                     // Si le caractère suivant `$` n'est pas une lettre valide pour une variable
//                     // Ajouter `$` au résultat
//                     char single_char[2] = {token[i], '\0'};
//                     char *tmp = result;
//                     result = ft_strjoin(result ? result : "", single_char);
//                     free(tmp);
//                     i++;
//                 }
//             } else {
//                 // Concatène les autres caractères
//                 char single_char[2] = {token[i], '\0'};
//                 char *tmp = result;
//                 result = ft_strjoin(result ? result : "", single_char);
//                 free(tmp);
//                 i++;
//             }
//         }
//         // Met à jour le token avec le résultat final
//         free(tokens->value);
//         tokens->value = result;
//         tokens = tokens->next;
//     }
// }



// void ps_expand_env(t_token *tokens, t_ctx *ctx)
// {
//     (void)ctx; // Ignorer ctx pour le moment, à utiliser si nécessaire

//     while (tokens) {
//         char *original = tokens->value; // Supposons que chaque token a un champ 'value'
//         char *result = NULL;

//         // Itérer sur les caractères du token
//         int i = 0;
//         while (original[i] != '\0') {
//             if (original[i] == '$') {
//                 // Vérifiez si c'est le début d'une variable
//                 char *var_start = &original[i + 1]; // Ignorez le '$'
//                 int j = 0;

//                 // Trouver la fin de la variable
//                 while (isalpha(var_start[j]) || var_start[j] == '_') {
//                     j++;
//                 }

//                 char var_name[j + 1]; // Créer un tableau pour le nom de la variable
//                 strncpy(var_name, var_start, j);
//                 var_name[j] = '\0'; // Terminez la chaîne

//                 // Récupérez la valeur de l'environnement
//                 char *env_value = getenv(var_name);
//                 if (env_value) {
//                     // Si 'result' est nul, allouez de la mémoire pour la première fois
//                     if (!result) {
//                         result = strdup(env_value);
//                     } else {
//                         // Concaténez la valeur de l'environnement au résultat
//                         char *tmp = result;
//                         result = ft_strjoin(result, env_value);
//                         free(tmp);
//                     }
//                     // Déplacez l'index à la fin de la variable
//                     i += j; // Sautez le nom de la variable
//                 } else {
//                     // Si la variable n'est pas trouvée, conservez le '$'
//                     char *tmp = result;
//                     if (!result) {
//                         result = malloc(2);
//                         result[0] = '$'; // Conservez le '$'
//                         result[1] = '\0';
//                     } else {
//                         char single_char[2] = {'$', '\0'};
//                         result = ft_strjoin(result, single_char);
//                         free(tmp);
//                     }
//                 }
//             } else {
//                 // Gérer les autres caractères
//                 char single_char[2] = {original[i], '\0'};
//                 char *tmp = result;
//                 if (!result) {
//                     result = strdup(single_char);
//                 } else {
//                     result = ft_strjoin(result, single_char);
//                     free(tmp);
//                 }
//             }
//             i++; // Passer au caractère suivant
//         }

//         // Remplacez la valeur du token par le résultat final
//         free(tokens->value); // Libérer l'ancienne valeur
//         tokens->value = result; // Mettre à jour le token avec le nouveau résultat

//         // Passez au token suivant
//         tokens = tokens->next; // Supposons que vous avez un champ 'next' dans votre structure t_token
//     }
// }
