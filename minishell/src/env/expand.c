/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:52:38 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/13 16:54:47 by llarrey          ###   ########.fr       */
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

///j'ai mis une taille fixe a voir si ca passe, le mettre en define c'est mieux
int	expand_variable(char *token, char **result, int i, char **env)
{
	char	var_name[256];
	char	*var_start;
	char	*env_value;
	int		j;

	var_start = &token[i + 1];
	j = extract_var_name(var_start, var_name);
	if (j > 0)
	{
		env_value = find_in_env(var_name, env);
		append_env_value(result, env_value);
		return (j + 1);
	}
	return (1);
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

void	ps_expand_env(t_token *tokens, t_ctx *ctx, char **env)
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
				i += expand_variable(token, &result, i, env);
			else
				i += append_character(token[i], &result);
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
