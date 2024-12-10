/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:24:28 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/10 15:48:38 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **create_command_array(const char *cmd_str)
{
    printf("Debug: Creating array for command: '%s'\n", cmd_str);

    // Ignorer les espaces au début
    while (*cmd_str == ' ')
        cmd_str++;

    char **cmd_array = ft_split(cmd_str, ' ');
    if (!cmd_array)
        return NULL;

    // Compter le nombre d'arguments pour debug
    int i = 0;
    while (cmd_array[i])
    {
        printf("Debug: arg[%d]: '%s'\n", i, cmd_array[i]);
        i++;
    }

    return cmd_array;
}

static void cleanup_exec_resources(char *cmd_path, char **cmd_array, char **env)
{
    free(cmd_path);
    free_array(cmd_array);
    free_array(env);
}

static void handle_command_not_found(const char *cmd, char **cmd_array, t_ctx *ctx)
{
    fprintf(stderr, "MiniBG: %s: command not found\n", cmd);
    free_array(cmd_array);
    ctx->exit_status = 127;
}

static void execute_child_process(char *cmd_path, char **cmd_array, char **env)
{
    execve(cmd_path, cmd_array, env);
    perror("execve");
    exit(1);
}

static void wait_for_child(pid_t pid, t_ctx *ctx)
{
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        ctx->exit_status = WEXITSTATUS(status);
}

void execute_external_command(const char *cmd_str, t_ctx *ctx)
{
    char **cmd_array = create_command_array(cmd_str);
    if (!cmd_array)
        return;
    char *cmd_path = find_command_path(cmd_array[0], ctx);
    if (!cmd_path)
    {
        handle_command_not_found(cmd_array[0], cmd_array, ctx);
        return;
    }
    char **env = create_env_array(ctx->env_vars);
    if (!env)
    {
        cleanup_exec_resources(cmd_path, cmd_array, NULL);
        return;
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        cleanup_exec_resources(cmd_path, cmd_array, env);
        return;
    }
    if (pid == 0)
        execute_child_process(cmd_path, cmd_array, env);
    else
        wait_for_child(pid, ctx);

    cleanup_exec_resources(cmd_path, cmd_array, env);
}

char *get_env_path(t_env_var *env_vars)
{
    t_env_var *current = env_vars;

    while (current)
    {
        if (ft_strcmp(current->name, "PATH") == 0)
            return current->value;
        current = current->next;
    }
    return NULL;
}

char *find_command_path(const char *cmd, t_ctx *ctx)
{
    // Si la commande contient un '/', c'est déjà un chemin
    if (ft_strchr(cmd, '/'))
        return ft_strdup(cmd);

    // Récupérer le PATH
    char *path = get_env_path(ctx->env_vars);
    if (!path)
        return NULL;

    // Diviser le PATH en dossiers
    char **directories = ft_split(path, ':');
    if (!directories)
        return NULL;

    // Chercher la commande dans chaque dossier
    char *cmd_path = NULL;
    for (int i = 0; directories[i]; i++)
    {
        // Construire le chemin complet
        cmd_path = ft_strjoin(directories[i], "/");
        char *temp = ft_strjoin(cmd_path, cmd);
        free(cmd_path);
        cmd_path = temp;

        // Vérifier si le fichier existe et est exécutable
        if (access(cmd_path, X_OK) == 0)
        {
            free_array(directories);
            return cmd_path;
        }
        free(cmd_path);
    }

    free_array(directories);
    return NULL;
}

char **create_env_array(t_env_var *env_vars)
{
    // Compter le nombre de variables
    int count = 0;
    t_env_var *current = env_vars;
    while (current)
    {
        count++;
        current = current->next;
    }

    // Allouer le tableau
    char **env = malloc(sizeof(char *) * (count + 1));
    if (!env)
        return NULL;

    // Remplir le tableau
    current = env_vars;
    count = 0;
    while (current)
    {
        // Format: NAME=VALUE
        char *temp = ft_strjoin(current->name, "=");
        env[count] = ft_strjoin(temp, current->value);
        free(temp);

        if (!env[count])
        {
            free_array(env);
            return NULL;
        }
        current = current->next;
        count++;
    }
    env[count] = NULL;

    return env;
}