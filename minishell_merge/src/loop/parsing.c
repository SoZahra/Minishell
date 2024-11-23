/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:34:21 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 14:23:32 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


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

int ft_export(t_ctx *ctx, char *key, char *value)
{
    if (!ctx || !key) // Vérifiez les pointeurs
        return -1;

    t_env_var *current = ctx->env_vars;
    // Recherche si la variable existe déjà
    while (current)
    {
        if (!current->name) // Vérifie que le nom est valide
        {
            fprintf(stderr, "Error: current->name is NULL\n");
            return -1;
        }
        if (ft_strcmp(current->name, key) == 0)
        {
            free(current->value); // Libérer l'ancienne valeur
            current->value = value ? strdup(value) : NULL; // Mettre à jour
            if (value && !current->value)
                return (perror("strdup failed"), -1);
            return 0; // Succès
        }
        current = current->next;
    }

    // Ajoutez une nouvelle variable si elle n'existe pas
    t_env_var *new_var = (t_env_var *)malloc(sizeof(t_env_var));
    if (!new_var)
        return (perror("malloc failed"), -1);

    new_var->name = strdup(key);
    if (!new_var->name)
    {
        free(new_var);
        return (perror("strdup failed"), -1);
    }

    new_var->value = value ? strdup(value) : NULL;
    if (value && !new_var->value)
    {
        free(new_var->name);
        free(new_var);
        return (perror("strdup failed"), -1);
    }

    new_var->next = ctx->env_vars;
    ctx->env_vars = new_var;

    return 0; // Succès
}


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
	path = expand_variables(args[1], ctx, TOKEN_ARGUMENT);
	if (!path || ft_strlen(path) == 0) // Si l'expansion échoue ou retourne une chaîne vide
	{
		fprintf(stderr, "cd: %s: No such file or directory\n", args[1]);
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

int ft_cd_home(t_ctx *ctx)
{
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

// int	ft_cd_oldpwd(char **oldpwd)
// {
// 	if (*oldpwd == NULL)
// 	{
// 		fprintf(stderr, "cd: OLDPWD not set\n");
// 		return (0);
// 	}
// 	if (chdir(*oldpwd) != 0)
// 	{
// 		perror("cd");
// 		return (0);
// 	}
// 	printf("%s\n", *oldpwd);
// 	return(0);
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

int ft_update_pwd(t_ctx *ctx)
{
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
