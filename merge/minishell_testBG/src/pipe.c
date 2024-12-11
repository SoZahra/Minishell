/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:02:51 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/11 17:44:54 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void clear_all(t_ctx *ctx)
{


    // clear env
    // tokens
    // cmds
}

int execute_piped_command(t_command *cmd, t_ctx *ctx)
{
    // Configurer l'entrée (pipe précédent)
    if (cmd->prev)
    {
        close(cmd->prev->pfd[1]);
        if (dup2(cmd->prev->pfd[0], STDIN_FILENO) == -1)
            return -1;
        close(cmd->prev->pfd[0]);
    }

    // Configurer la sortie (pipe suivant)
    if (cmd->next)
    {
        close(cmd->pfd[0]);
        if (dup2(cmd->pfd[1], STDOUT_FILENO) == -1)
            return -1;
        close(cmd->pfd[1]);
    }

    // Appliquer les redirections locales si elles existent
    if (cmd->redirs)
        apply_redirections(cmd->redirs);

    // Exécuter la commande
    if (is_builtin(cmd->args[0]))
    {
        execute_builtin_command(cmd, ctx);
        exit(ctx->exit_status);
    }
    else if (cmd->path)
    {
        char **env = create_env_array(ctx->env_vars);
        execve(cmd->path, cmd->args, env);
        free_array(env);
    }
    // clear all
    clear_all(ctx);
    perror("command execution failed");
    exit(127);
}

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
    //     fprintf(stderr, "MiniBG: syntax error near unexpected token `|'\n");
    //     return -1;
    // }
    if (is_builtin(cmd->args[0]))
        return 0;
    cmd->path = find_command_path(cmd->args[0], ctx);
    if (!cmd->path)
    {
        fprintf(stderr, "MiniBG: %s: command not found\n", cmd->args[0]);
        return -1;
    }
    return 0;
}

static void parent_proc(t_command *cmd)
{
    if (cmd->prev)
    {
        close(cmd->prev->pfd[0]);
        close(cmd->prev->pfd[1]);
    }
}

static int spawn_process(t_command *cmd, t_ctx *ctx)
{
    if (cmd->next)
        pipe(cmd->pfd);
    cmd->pid = fork();
    if (cmd->pid == -1)
        return (perror("fork"), -1);
    if (cmd->pid == 0)
        execute_piped_command(cmd, ctx);
    if (cmd->pid > 0)
        parent_proc(cmd);
    return 0;
}

void execute_pipeline(t_command *cmd, t_ctx *ctx)
{
    if (!cmd)
        return;
    // if (setup_pipeline(cmd) == -1)
    //     return;
    if (prepare_command(cmd, ctx) == -1)
        return;
    if (spawn_process(cmd, ctx) == -1)
        return;
    if (cmd->next)
        execute_pipeline(cmd->next, ctx);
    else
        wait_for_children(cmd, ctx);
}
