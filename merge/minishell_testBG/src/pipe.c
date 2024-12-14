/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:02:51 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 02:03:02 by fatimazahra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void clear_all(t_ctx *ctx)
// {


//     // clear env
//     // tokens
//     // cmds
// }

int execute_piped_command(t_command *cmd, t_ctx *ctx)
{
    if (cmd->prev) {
        close(cmd->prev->pfd[1]);
        fprintf(stderr, "Debug: Closing write-end of previous pipe: [%d]\n", cmd->prev->pfd[1]);
        if (dup2(cmd->prev->pfd[0], STDIN_FILENO) == -1) {
            perror("dup2 prev->pfd[0]");
            return -1;
        }
        fprintf(stderr, "Debug: Duped previous pipe read-end [%d] to STDIN\n", cmd->prev->pfd[0]);
        close(cmd->prev->pfd[0]);
    }

    if (cmd->next) {
        close(cmd->pfd[0]);
        fprintf(stderr, "Debug: Closing read-end of current pipe: [%d]\n", cmd->pfd[0]);
        if (dup2(cmd->pfd[1], STDOUT_FILENO) == -1) {
            perror("dup2 pfd[1]");
            return -1;
        }
        fprintf(stderr, "Debug: Duped current pipe write-end [%d] to STDOUT\n", cmd->pfd[1]);
        close(cmd->pfd[1]);
    }

    if (cmd->redirs) {
        fprintf(stderr, "Debug: Applying redirections for '%s'\n", cmd->args[0]);
        apply_redirections(cmd->redirs);
    }

    fprintf(stderr, "Debug: Executing command '%s'\n", cmd->args[0]);
    if (is_builtin(cmd->args[0])) {
        execute_builtin_command(cmd, ctx);
        exit(ctx->exit_status);
    } else if (cmd->path) {
        char **env = create_env_array(ctx->env_vars);
        execve(cmd->path, cmd->args, env);
        perror("execve");
        free_array(env);
    }
    exit(127);
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
//         fprintf(stderr, "Debug: cmd->prev->pfd[0]: %d, cmd->prev->pfd[1]: %d\n", cmd->prev ? cmd->prev->pfd[0] : -1, cmd->prev ? cmd->prev->pfd[1] : -1);
//         fprintf(stderr, "Debug: cmd->pfd[0]: %d, cmd->pfd[1]: %d\n", cmd->pfd[0], cmd->pfd[1]);

//     }

//     // Configurer la sortie (pipe suivant)
//     if (cmd->next)
//     {
//         close(cmd->pfd[0]);
//         if (dup2(cmd->pfd[1], STDOUT_FILENO) == -1)
//             return -1;
//         close(cmd->pfd[1]);
//         fprintf(stderr, "Debug: cmd->prev->pfd[0]: %d, cmd->prev->pfd[1]: %d\n", cmd->prev ? cmd->prev->pfd[0] : -1, cmd->prev ? cmd->prev->pfd[1] : -1);
//         fprintf(stderr, "Debug: cmd->pfd[0]: %d, cmd->pfd[1]: %d\n", cmd->pfd[0], cmd->pfd[1]);

//     }

//     // Appliquer les redirections locales si elles existent
//     if (cmd->redirs)
//         apply_redirections(cmd->redirs);

//     // Exécuter la commande
//     if (is_builtin(cmd->args[0]))
//     {
//         printf("passer dans execute piped_command");
//         execute_builtin_command(cmd, ctx);
//         exit(ctx->exit_status);
//     }
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

// void wait_for_children(t_command *cmd, t_ctx *ctx)
// {
//     t_command *current = cmd;
//     int status;

//     // Retourner au début de la liste
//     while (current->prev)
//         current = current->prev;

//     // Attendre tous les processus
//     while (current)
//     {
//         if (current->pid > 0)
//         {
//             waitpid(current->pid, &status, 0);
//             tcsetattr(STDIN_FILENO, TCSANOW, &get_ctx()->term);
//             if (WIFEXITED(status))
//                 ctx->exit_status = WEXITSTATUS(status);
// 			if (WIFSIGNALED(status))
// 				ctx->exit_status = WTERMSIG(status) + 128;
//         }
//         current = current->next;
//     }
// }

void wait_for_children(t_command *cmd, t_ctx *ctx)
{
    t_command *current = cmd;
    int status;

    while (current->prev)
        current = current->prev;

    while (current) {
        if (current->pid > 0) {
            fprintf(stderr, "Debug: Waiting for PID %d (%s)\n", current->pid, current->args[0]);
            waitpid(current->pid, &status, 0);
            if (WIFEXITED(status)) {
                ctx->exit_status = WEXITSTATUS(status);
                fprintf(stderr, "Debug: PID %d exited with status %d\n", current->pid, ctx->exit_status);
            }
            if (WIFSIGNALED(status)) {
                ctx->exit_status = WTERMSIG(status) + 128;
                fprintf(stderr, "Debug: PID %d killed by signal %d\n", current->pid, WTERMSIG(status));
            }
        }
        current = current->next;
    }
}



// int prepare_command(t_command *cmd, t_ctx *ctx)
// {
// 	// if (!cmd || !cmd->args || !cmd->args[0])
//     // {
//     //     fprintf(stderr, "MiniBG: syntax error near unexpected token `|'\n");
//     //     return -1;
//     // }
//     if (is_builtin(cmd->args[0]))
//         return 0;
//     cmd->path = find_command_path(cmd->args[0], ctx);
//     if (!cmd->path)
//     {
//         fprintf(stderr, "MiniBG: %s: command not found\n", cmd->args[0]);
//         return -1;
//     }
//     return 0;
// }

int prepare_command(t_command *cmd, t_ctx *ctx)
{
    if (is_builtin(cmd->args[0])) {
        fprintf(stderr, "Debug: '%s' is a builtin command\n", cmd->args[0]);
        return 0;
    }
    cmd->path = find_command_path(cmd->args[0], ctx);
    fprintf(stderr, "Debug: Command: '%s', Path: '%s'\n", cmd->args[0], cmd->path ? cmd->path : "NULL");
    if (!cmd->path) {
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

static int spawn_process(t_command *cmd, t_ctx *ctx)
{
    if (cmd->next) {
        if (pipe(cmd->pfd) == -1) {
            perror("pipe");
            return -1;
        }
        // fprintf(stderr, "Debug: Created pipe [%d, %d] for '%s'\n", cmd->pfd[0], cmd->pfd[1], cmd->args[0]);
    }

    cmd->pid = fork();
    if (cmd->pid == -1) {
        perror("fork");
        return -1;
    }

    if (cmd->pid == 0) {
        // fprintf(stderr, "Debug: Forked child for '%s', PID: %d\n", cmd->args[0], getpid());
        execute_piped_command(cmd, ctx);
    }

    if (cmd->pid > 0) {
        // fprintf(stderr, "Debug: Parent process for '%s', PID: %d\n", cmd->args[0], cmd->pid);
        parent_proc(cmd);
    }
    return 0;
}

void execute_pipeline(t_command *cmd, t_ctx *ctx)
{
    if (!cmd) {
        // fprintf(stderr, "Debug: Pipeline is empty\n");
        return;
    }

    // fprintf(stderr, "Debug: Executing pipeline for command '%s'\n", cmd->args[0]);
    if (prepare_command(cmd, ctx) == -1) {
        // fprintf(stderr, "Debug: Failed to prepare command '%s'\n", cmd->args[0]);
        return;
    }

    if (spawn_process(cmd, ctx) == -1) {
        // fprintf(stderr, "Debug: Failed to spawn process for '%s'\n", cmd->args[0]);
        return;
    }

    if (cmd->next) {
        // fprintf(stderr, "Debug: Moving to next command in pipeline\n");
        execute_pipeline(cmd->next, ctx);
    } else {
        // fprintf(stderr, "Debug: Waiting for children\n");
        wait_for_children(cmd, ctx);
    }
}


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
