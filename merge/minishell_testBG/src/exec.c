/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:24:28 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 18:49:39 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void wait_for_child(pid_t pid, t_ctx *ctx)
{
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        ctx->exit_status = WEXITSTATUS(status);
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

int add_redirection(t_redirection **redirs, char type, char *file)
{
    t_redirection *new = malloc(sizeof(t_redirection));
    if (!new)
        return 0;

    new->type = type;
    new->file = ft_strdup(file);
    new->fd = -1;
    new->next = NULL;

    if (!new->file)
    {
        free(new);
        return 0;
    }

    // Ajouter au début de la liste
    new->next = *redirs;
    *redirs = new;

    return 1;
}

void execute_command(t_command *cmd, t_ctx *ctx)
{
    int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO);

    if (apply_redirections(cmd->redirs) == -1)
    {
        perror("apply_redirections");
        restore_fds(stdin_backup, stdout_backup);
        return;
    }
    // if (expand_proc(cmd, ctx) == -1)
    //     return (free(cmd), 1);
    if (is_builtin(cmd->args[0]))
        execute_builtin(cmd->args[0], ctx);
    else
        execute_external_command(cmd, ctx);
    restore_fds(stdin_backup, stdout_backup);
}

int apply_redirections(t_redirection *redirs)
{
    t_redirection *current = redirs;
    int last_fd = -1;

    t_redirection *reversed = NULL;
    while (current)
    {
        t_redirection *next = current->next;
        current->next = reversed;
        reversed = current;
        current = next;
    }
    current = reversed;
    while (current)
    {
        if (current->type == '>' || current->type == 'A')
        {
            // Si on avait déjà ouvert un fichier, le fermer
            if (last_fd != -1)
                close(last_fd);

            // Redirection sortante
            int flags = O_WRONLY | O_CREAT;
            flags |= (current->type == 'A') ? O_APPEND : O_TRUNC;

            current->fd = open(current->file, flags, 0644);
            if (current->fd == -1)
            {
                perror("open");
                return -1;
            }

            if (dup2(current->fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                return -1;
            }
            close(current->fd);

            last_fd = current->fd;
        }
        current = current->next;
    }

    return 0;
}

void restore_fds(int stdin_fd, int stdout_fd)
{
   if (stdin_fd != -1)
   {
       dup2(stdin_fd, STDIN_FILENO);
       close(stdin_fd);
   }
   if (stdout_fd != -1)
   {
       dup2(stdout_fd, STDOUT_FILENO);
       close(stdout_fd);
   }
}

void execute_external_command(t_command *cmd, t_ctx *ctx)
{
   char *cmd_path = find_command_path(cmd->args[0], ctx);
   if (!cmd_path)
   {
       fprintf(stderr, "MiniBG: %s: command not found\n", cmd->args[0]);
       return;
   }
   pid_t pid = fork();
   if (pid == -1)
   {
       perror("fork");
       free(cmd_path);
       return;
   }
   if (pid == 0)
   {
       char **env = create_env_array(ctx->env_vars);
       execve(cmd_path, cmd->args, env);
       perror("execve");
       exit(1);
   }
   else
   {
       int status;
       waitpid(pid, &status, 0);
       if (WIFEXITED(status))
           ctx->exit_status = WEXITSTATUS(status);
       free(cmd_path);
   }
}

void free_command(t_command *cmd)
{
    if (!cmd)
        return;

    if (cmd->args)
    {
        for (int i = 0; cmd->args[i]; i++)
            free(cmd->args[i]);
        free(cmd->args);
    }
    t_redirection *current = cmd->redirs;
    while (current)
    {
        t_redirection *next = current->next;
        if (current->file)
            free(current->file);
        free(current);
        current = next;
    }
    free(cmd);
}
