/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:52:38 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/11 13:40:14 by llarrey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
    return ft_strndup(str, found - str); // Copie jusqu'au `$`
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
        // Libérer tous les pointeurs s'ils sont non nuls
        if (before_env)
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
    return (free(new), free(after_env), final_result); // Renvoie le résultat final
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

void ps_expand_env(t_token *tokens, t_ctx *ctx, char **env) //#to do # add char **env and reproduce getenv with the env used by the miniBG
{
    (void)ctx;

    while (tokens)
    {
        char *token = tokens->value;
        char *result = NULL;
        int i = 0;
        while (token[i] != '\0') {
            if (token[i] == '$') 
            {
                char *var_start = &token[i + 1];
                int j = 0;
                while (isalpha(var_start[j]) || var_start[j] == '_') 
                    j++;
                if (j > 0) 
                {
                    char var_name[j + 1];
                    strncpy(var_name, var_start, j);
                    var_name[j] = '\0';
                    char * env_value = find_in_env(var_name, env);
                    char *tmp = result;
                    result = ft_strjoin(result ? result : "", env_value ? env_value : "");
                    free(tmp);
                    i += j + 1;
                } else {
                    char single_char[2] = {token[i], '\0'};
                    char *tmp = result;
                    result = ft_strjoin(result ? result : "", single_char);
                    free(tmp);
                    i++;
                }
            } 
            else 
            {
                char single_char[2] = {token[i], '\0'};
                char *tmp = result;
                result = ft_strjoin(result ? result : "", single_char);
                free(tmp);
                i++;
            }
        }
        free(tokens->value);
        tokens->value = result;
        tokens = tokens->next;
    }
}

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
