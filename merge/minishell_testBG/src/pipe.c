/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:02:51 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 22:10:29 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void clear_all(t_ctx *ctx)
// {


//     // clear env
//     // tokens
//     // cmds
// }

void close_unused_pipes(t_command *current_cmd)
{
    t_command *cmd = current_cmd;

    // Remonter au début de la liste
    while (cmd->prev)
        cmd = cmd->prev;

    // Parcourir toutes les commandes
    while (cmd)
    {
        if (cmd != current_cmd)
        {
            if (cmd->pfd[0] != -1)
                close(cmd->pfd[0]);
            if (cmd->pfd[1] != -1)
                close(cmd->pfd[1]);
        }
        cmd = cmd->next;
    }
}

int execute_piped_command(t_command *cmd, t_ctx *ctx)
{
    // Configurer l'entrée (pipe précédent)
    if (cmd->prev)
    {
        close(cmd->prev->pfd[1]);
        if (dup2(cmd->prev->pfd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
        close(cmd->prev->pfd[0]);
    }

    // Configurer la sortie (pipe suivant)
    if (cmd->next)
    {
        close(cmd->pfd[0]);
        if (dup2(cmd->pfd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
        close(cmd->pfd[1]);
    }
    close_unused_pipes(cmd);

    // Appliquer les redirections locales si elles existent
    if (cmd->redirs && apply_redirections(cmd->redirs) == -1)
    {
        perror("redirection");
        exit(1);
    }

    // Exécuter la commande
    if (is_builtin(cmd->args[0]))
    {
        // execute_builtin(cmd->args[0], ctx);
        int builtin_result = execute_builtin_in_pipe(cmd, ctx);
        exit(builtin_result);  // Important!
    }
    else if (cmd->path)
    {
        char **env = create_env_array(ctx->env_vars);
        execve(cmd->path, cmd->args, env);
        free_array(env);
    }
    perror("command execution failed");
    exit(127);
}

t_token *tokens_from_command(t_command *cmd)
{
    t_token *first = NULL;

    for (int i = 0; cmd->args[i]; i++)
        add_token(&first, 'S', cmd->args[i]);
    return first;
}

int execute_builtin_in_pipe(t_command *cmd, t_ctx *ctx)
{
    // Créer une ligne de commande à partir des arguments
    char *cmd_line = ft_strjoin(cmd->args[0], " ");
    for (int i = 1; cmd->args[i]; i++)
    {
        char *temp = ft_strjoin(cmd_line, cmd->args[i]);
        free(cmd_line);
        cmd_line = temp;

        // Ajouter un espace entre les arguments
        if (cmd->args[i + 1])
        {
            temp = ft_strjoin(cmd_line, " ");
            free(cmd_line);
            cmd_line = temp;
        }
    }

    // Extraire le premier mot (la commande)
    char *cmd_name = ft_strdup(cmd->args[0]);
    char *space = strchr(cmd_line, ' ');
    if (space)
        space[0] = '\0';

    // Trouver le début des arguments
    const char *args = cmd_line + ft_strlen(cmd_name);
    while (*args == ' ')
        args++;

    // Exécuter le bon builtin
    int result = 0;
    if (ft_strcmp(cmd_name, "echo") == 0)
        result = handle_echo_builtin(args, ctx);
    else if (ft_strcmp(cmd_name, "pwd") == 0)
        result = handle_pwd_builtin(args, ctx);
    else if (ft_strcmp(cmd_name, "env") == 0)
        result = handle_env_builtin(args, ctx);

    // Les built-ins qui ne doivent PAS être exécutés dans un pipe
    if (ft_strcmp(cmd_name, "cd") == 0 ||
        ft_strcmp(cmd_name, "export") == 0 ||
        ft_strcmp(cmd_name, "unset") == 0 ||
        ft_strcmp(cmd_name, "exit") == 0)
    {
        ft_fprintf(2, "MiniBG: %s: cannot be used in a pipeline\n", cmd_name);
        result = 1;
    }

    free(cmd_name);
    free(cmd_line);
    return result;
}


// int execute_piped_command(t_command *cmd, t_ctx *ctx)
// {
//     // Configurer l'entrée (pipe précédent)
//     if (cmd->prev)
//     {
//         close(cmd->prev->pfd[1]);
//         if (dup2(cmd->prev->pfd[0], STDIN_FILENO) == -1)
//             return -1;
//         close(cmd->prev->pfd[0]);
//     }

//     // Configurer la sortie (pipe suivant)
//     if (cmd->next)
//     {
//         close(cmd->pfd[0]);
//         if (dup2(cmd->pfd[1], STDOUT_FILENO) == -1)
//             return -1;
//         close(cmd->pfd[1]);
//     }

//     // Appliquer les redirections locales si elles existent
//     if (cmd->redirs)
//         apply_redirections(cmd->redirs);

//     // Exécuter la commande
//     if (is_builtin(cmd->args[0]))
//         execute_builtin(cmd->args, ctx);
//     else if (cmd->path)
//     {
//         char **env = create_env_array(ctx->env_vars);
//         execve(cmd->path, cmd->args, env);
//         free_array(env);
//     }
//     // clear all
//     // clear_all(ctx);
//     perror("command execution failed");
//     exit(127);
// }

void wait_for_children(t_command *cmd, t_ctx *ctx)
{
    t_command *current = cmd;
    int status;

    // Retourner au début de la liste
    while (current->prev)
        current = current->prev;

    // Attendre tous les processus
    while (current)
    {
        if (current->pid > 0)
        {
            waitpid(current->pid, &status, 0);
            tcsetattr(STDIN_FILENO, TCSANOW, &get_ctx()->term);
            if (WIFEXITED(status))
                ctx->exit_status = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
				ctx->exit_status = WTERMSIG(status) + 128;
        }
        current = current->next;
    }
}

// static int setup_pipeline(t_command *cmd)
// {
//     if (!cmd->next)
//         return 0;

//     if (pipe(cmd->pfd) < 0)
//     {
//         perror("pipe");
//         return -1;
//     }
//     return 0;
// }

int prepare_command(t_command *cmd, t_ctx *ctx)
{
	// if (!cmd || !cmd->args || !cmd->args[0])
    // {
    //     ft_fprintf(2, "MiniBG: syntax error near unexpected token `|'\n");
    //     return -1;
    // }
    if (is_builtin(cmd->args[0]))
        return 0;
    cmd->path = find_command_path(cmd->args[0], ctx);
    if (!cmd->path)
    {
        ft_fprintf(2, "MiniBG: %s: command not found\n", cmd->args[0]);
        return -1;
    }
    return 0;
}

void execute_pipeline(t_command *cmd, t_ctx *ctx)
{
    int num_commands = 0;
    t_command *current = cmd;

    // Compter le nombre de commandes
    while (current)
    {
        num_commands++;
        current = current->next;
    }

    int pipes[num_commands - 1][2];
    pid_t pids[num_commands];

    // Initialiser tous les pipes
    for (int i = 0; i < num_commands - 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            return;
        }
    }

    current = cmd;
    for (int i = 0; current; i++)
    {
        pids[i] = fork();

        if (pids[i] == -1)
        {
            perror("fork");
            return;
        }

        if (pids[i] == 0)  // Processus enfant
        {
            ft_fprintf(2, "Processus %d (commande %s) démarré\n", getpid(), current->args[0]);
            // Rediriger l'entrée si ce n'est pas la première commande
            if (i > 0)
            {
                if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
                {
                    perror("dup2 stdin");
                    exit(1);
                }
            }

            // Rediriger la sortie si ce n'est pas la dernière commande
            if (i < num_commands - 1)
            {
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 stdout");
                    exit(1);
                }
            }

            // Fermer tous les descripteurs de pipe
            for (int j = 0; j < num_commands - 1; j++)
            {
                if (j != i && j != i - 1)
                {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }

            // Exécuter la commande
            if (is_builtin(current->args[0]))
            {
                int builtin_result = execute_builtin_in_pipe(current, ctx);
                exit(builtin_result);
            }
            else
            {
                char **env = create_env_array(ctx->env_vars);
                execve(current->path, current->args, env);
                free_array(env);
                perror("execve");
                exit(1);
            }
                ft_fprintf(2, "Processus %d (commande %s) va exécuter la commande\n", getpid(), current->args[0]);
        }

        // Processus parent
        if (i > 0)
        {
            close(pipes[i - 1][0]);
            close(pipes[i - 1][1]);
        }

        current = current->next;
    }

    // Fermer le dernier pipe
    if (num_commands > 1)
    {
        close(pipes[num_commands - 2][0]);
        close(pipes[num_commands - 2][1]);
    }

    // Attendre tous les processus
    for (int i = 0; i < num_commands; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);

        // Mettre à jour le statut de sortie avec le dernier processus
        if (WIFEXITED(status))
            ctx->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            ctx->exit_status = WTERMSIG(status) + 128;
    }
}



// static void parent_proc(t_command *cmd)
// {
//     if (cmd->prev)
//     {
//         close(cmd->prev->pfd[0]);
//         close(cmd->prev->pfd[1]);
//     }
//     if (cmd->next)
//         close(cmd->pfd[1]);
// }

// static int spawn_process(t_command *cmd, t_ctx *ctx)
// {
//     if (cmd->next)
//         pipe(cmd->pfd);
//     cmd->pid = fork();
//     if (cmd->pid == -1)
//         return (perror("fork"), -1);
//     if (cmd->pid == 0)
//         execute_piped_command(cmd, ctx);
//     if (cmd->pid > 0)
//         parent_proc(cmd);
//     return 0;
// }


// void execute_pipeline(t_command *cmd, t_ctx *ctx)
// {
//     if (!cmd)
//         return;
//     // if (setup_pipeline(cmd) == -1)
//     //     return;
//     if (prepare_command(cmd, ctx) == -1)
//         return;
//     if (spawn_process(cmd, ctx) == -1)
//         return;
//     if (cmd->next)
//         execute_pipeline(cmd->next, ctx);
//     else
//         wait_for_children(cmd, ctx);
// }
