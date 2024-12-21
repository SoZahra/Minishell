/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:02:51 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/21 19:27:53 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void clear_all(t_ctx *ctx)
// {


//     // clear env
//     // tokens
//     // cmds
// }

// int execute_piped_command(t_command *cmd, t_ctx *ctx)
// {
//     if (cmd->prev) 
//     {
//         close(cmd->prev->pfd[1]);
//         if (dup2(cmd->prev->pfd[0], STDIN_FILENO) == -1) {
//             perror("dup2 prev->pfd[0]");
//             return -1;
//         }
//         close(cmd->prev->pfd[0]);
//     }
//     if (cmd->next) 
//     {
//         close(cmd->pfd[0]);
//         if (dup2(cmd->pfd[1], STDOUT_FILENO) == -1) {
//             perror("dup2 pfd[1]");
//             return -1;
//         }
//         close(cmd->pfd[1]);
//     }
//     if (cmd->redirs)
//         apply_redirections(cmd->redirs, ctx);
//     if (is_builtin(cmd->args[0])) 
//     {
//         execute_builtin_command(cmd, ctx);
//         free_command(cmd);
//         cleanup_shell(ctx);
//         exit(ctx->exit_status);
//     } else if (cmd->path) 
//     {
//         char **env = create_env_array(ctx->env_vars);
//         execve(cmd->path, cmd->args, env);
//         perror("execve");
//         free_array(env);
//         free_command(cmd);
//         cleanup_shell(ctx);
//     }
//     exit(127);
// }


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
            // fprintf(stderr, "Debug: Waiting for PID %d (%s)\n", current->pid, current->args[0]);
            waitpid(current->pid, &status, 0);
            if (WIFEXITED(status)) {
                ctx->exit_status = WEXITSTATUS(status);
                // fprintf(stderr, "Debug: PID %d exited with status %d\n", current->pid, ctx->exit_status);
            }
            if (WIFSIGNALED(status)) {
                ctx->exit_status = WTERMSIG(status) + 128;
                // fprintf(stderr, "Debug: PID %d killed by signal %d\n", current->pid, WTERMSIG(status));
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
        // fprintf(stderr, "Debug: '%s' is a builtin command\n", cmd->args[0]);
        return 0;
    }
    cmd->path = find_command_path(cmd->args[0], ctx);
    // fprintf(stderr, "Debug: Command: '%s', Path: '%s'\n", cmd->args[0], cmd->path ? cmd->path : "NULL");
    if (!cmd->path) {
        fprintf(stderr, "MiniBG: %s: command not found\n", cmd->args[0]);
        return -1;
    }
    return 0;
}

// static void parent_proc(t_command *cmd)
// {
//     if (cmd->prev)
//     {
//         close(cmd->prev->pfd[0]);
//         close(cmd->prev->pfd[1]);
//     }
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

// static int spawn_process(t_command *cmd, t_ctx *ctx)
// {
//     if (cmd->next) {
//         if (pipe(cmd->pfd) == -1) {
//             perror("pipe");
//             return -1;
//         }
//         // fprintf(stderr, "Debug: Created pipe [%d, %d] for '%s'\n", cmd->pfd[0], cmd->pfd[1], cmd->args[0]);
//     }
//     cmd->pid = fork();
//     if (cmd->pid == -1) {
//         perror("fork");
//         return -1;
//     }

//     if (cmd->pid == 0) {
//         // fprintf(stderr, "Debug: Forked child for '%s', PID: %d\n", cmd->args[0], getpid());
//         execute_piped_command(cmd, ctx);
//     }
//     if (cmd->pid > 0) {
//         // fprintf(stderr, "Debug: Parent process for '%s', PID: %d\n", cmd->args[0], cmd->pid);
//         parent_proc(cmd);
//     }
//     return 0;
// }

// void execute_pipeline(t_command *cmd, t_ctx *ctx)
// {
//     if (!cmd) {
//         // fprintf(stderr, "Debug: Pipeline is empty\n");
//         return;
//     }
//     // fprintf(stderr, "Debug: Executing pipeline for command '%s'\n", cmd->args[0]);
//     if (prepare_command(cmd, ctx) == -1) {
//         // fprintf(stderr, "Debug: Failed to prepare command '%s'\n", cmd->args[0]);
//         return;
//     }
//     if (spawn_process(cmd, ctx) == -1) {
//         // fprintf(stderr, "Debug: Failed to spawn process for '%s'\n", cmd->args[0]);
//         return;
//     }
//     if (cmd->next) {
//         // fprintf(stderr, "Debug: Moving to next command in pipeline\n");
//         execute_pipeline(cmd->next, ctx);
//     } else {
//         // fprintf(stderr, "Debug: Waiting for children\n");
//         wait_for_children(cmd, ctx);
//     }
// }


// Compte le nombre de commandes dans le pipeline
int count_commands(t_command *cmd)
{
    int count = 0;
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    return count;
}

// Créer les pipes pour le pipeline
int **create_pipeline_pipes(int num_commands)
{
    if (num_commands <= 1)
        return NULL;

    int **pipes = malloc(sizeof(int *) * (num_commands - 1));
    for (int i = 0; i < num_commands - 1; i++)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (pipe(pipes[i]) == -1)
        {
            perror("pipe");
            // Libérer les pipes déjà créés
            while (i > 0)
            {
                i--;
                close(pipes[i][0]);
                close(pipes[i][1]);
                free(pipes[i]);
            }
            free(pipes);
            return NULL;
        }
    }
    return pipes;
}

// Fermer les descripteurs de pipe
void close_pipes(int **pipes, int num_commands)
{
    if (!pipes)
        return;

    for (int i = 0; i < num_commands - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        free(pipes[i]);
    }
    free(pipes);
}

// Rediriger l'entrée pour un processus
void redirect_input(t_command *cmd, int **pipes, int index)
{
    // Redirections d'entrée définies
    if (cmd->redirs)
    {
        t_redirection *redir = cmd->redirs;
        while (redir && redir->type != 0)
        {
            if (redir->type == '<')
                handle_input_redirection(redir);
            redir = redir->next;
        }
    }

    // Redirection depuis le pipe précédent
    if (index > 0 && pipes)
    {
        dup2(pipes[index - 1][0], STDIN_FILENO);
        close(pipes[index - 1][0]);
        close(pipes[index - 1][1]);
    }
}

// Rediriger la sortie pour un processus
void redirect_output(int **pipes, int index, int num_commands)
{
    if (index < num_commands - 1 && pipes)
    {
        dup2(pipes[index][1], STDOUT_FILENO);
        close(pipes[index][0]);
        close(pipes[index][1]);
    }
}

// Fermer tous les descripteurs de pipe sauf ceux utilisés
void close_unused_pipes(int **pipes, int index, int num_commands)
{
    if (!pipes)
        return;

    for (int j = 0; j < num_commands - 1; j++)
    {
        if (j != index - 1 && j != index)
        {
            close(pipes[j][0]);
            close(pipes[j][1]);
        }
    }
}

char *build_command_line(t_command *cmd)
{
    // Calculer la taille nécessaire
    size_t total_len = 0;
    for (int i = 0; cmd->args[i]; i++)
    {
        total_len += strlen(cmd->args[i]) + 1; // +1 pour l'espace
    }

    // Allouer la mémoire
    char *cmd_line = malloc(total_len + 1);
    if (!cmd_line)
        return NULL;

    // Construire la ligne de commande
    cmd_line[0] = '\0';
    for (int i = 0; cmd->args[i]; i++)
    {
        if (i > 0)
            strcat(cmd_line, " ");
        strcat(cmd_line, cmd->args[i]);
    }

    return cmd_line;
}

int has_input_redirection(t_command *cmd)
{
    if (!cmd->redirs)
        return 0;
    for (int i = 0; cmd->redirs[i].type != 0; i++)
    {
        if (cmd->redirs[i].type == '<' || cmd->redirs[i].type == 'H')
            return 1;
    }
    return 0;
}
int has_output_redirection(t_command *cmd)
{
    if (!cmd->redirs)
        return 0;
    for (int i = 0; cmd->redirs[i].type != 0; i++)
    {
        if (cmd->redirs[i].type == '>' || cmd->redirs[i].type == 'A')
            return 1;
    }
    return 0;
}

void setup_pipe_redirections(t_command *cmd, int index, int cmd_count)
{
    if (index > 0 && cmd->prev)
    {
        dup2(cmd->prev->pfd[0], STDIN_FILENO);
        close(cmd->prev->pfd[0]);
        close(cmd->prev->pfd[1]);
    }
    
    if (index < cmd_count - 1)
    {
        close(cmd->pfd[0]);
        dup2(cmd->pfd[1], STDOUT_FILENO);
        close(cmd->pfd[1]);
    }
}

void execute_command_in_child(t_command *cmd, t_ctx *ctx)
{
    if (apply_redirections(cmd->redirs, ctx) == -1)
        exit(1);

    if (is_builtin(cmd->args[0]))
    {
        char *cmd_line = tokens_to_string_from_command(cmd);
        int ret = execute_builtin(cmd_line, ctx);
        free(cmd_line);
        exit(ret);
    }

    char **env = create_env_array(ctx->env_vars);
    if (prepare_command(cmd, ctx) == -1)
        exit(1);
    execve(cmd->path, cmd->args, env);
    perror("execve");
    free_command(cmd);
    cleanup_shell(ctx);
    exit(1);
}

void execute_commands_in_pipeline(t_command *cmd, int cmd_count, t_ctx *ctx)
{
    int i = 0;
    t_command *current = cmd;

    while (current && i < cmd_count)
    {
        if (i < cmd_count - 1)
            pipe(current->pfd);

        current->pid = fork();
        if (current->pid == 0)
        {
            setup_pipe_redirections(current, i, cmd_count);
            execute_command_in_child(current, ctx);
            exit(1);
        }
        
        if (i > 0)
        {
            close(current->prev->pfd[0]);
            close(current->prev->pfd[1]);
        }
        
        i++;
        current = current->next;
    }
    wait_for_children(cmd, ctx);
}

// void execute_command_in_pipeline(t_command *cmd, t_ctx *ctx, int **pipes, int index, int num_commands)
// {
//     // Gérer les redirections en premier
//     if (cmd->redirs)
//     {
//         for (int i = 0; cmd->redirs[i].type != 0; i++)
//         {
//             if (cmd->redirs[i].type == '<')
//             {
//                 if (access(cmd->redirs[i].file, F_OK) == -1)
//                 {
//                     ft_fprintf(2, "minishell: %s: No such file or directory\n", 
//                              cmd->redirs[i].file);
//                     exit(1);
//                 }
//                 int in_fd = open(cmd->redirs[i].file, O_RDONLY);
//                 if (in_fd == -1)
//                 {
//                     ft_fprintf(2, "minishell: %s: Permission denied\n",  cmd->redirs[i].file);
//                     exit(1);
//                 }
//                 dup2(in_fd, STDIN_FILENO);
//                 close(in_fd);
//             }
//             else if (cmd->redirs[i].type == 'H')
//             {
//                 if (cmd->redirs[i].heredoc_fd != -1)
//                 {
//                     if (dup2(cmd->redirs[i].heredoc_fd, STDIN_FILENO) == -1)
//                     {
//                         perror("dup2 heredoc");
//                         exit(1);
//                     }
//                     close(cmd->redirs[i].heredoc_fd);
//                 }
//                 else
//                 {
//                     // Si heredoc_fd n'est pas initialisé, on le crée maintenant
//                     int heredoc_fd = here_doc(cmd->redirs[i].file, ctx);
//                     if (heredoc_fd == -1)
//                     {
//                         ft_fprintf(2, "minishell: heredoc error\n");
//                         exit(1);
//                     }
//                     if (dup2(heredoc_fd, STDIN_FILENO) == -1)
//                     {
//                         perror("dup2 heredoc");
//                         close(heredoc_fd);
//                         exit(1);
//                     }
//                     close(heredoc_fd);
//                 }
//             }
//             else if (cmd->redirs[i].type == '>' || cmd->redirs[i].type == 'A')
//             {
//                 // Vérifier les permissions si le fichier existe
//                 if (access(cmd->redirs[i].file, F_OK) == 0 &&
//                     access(cmd->redirs[i].file, W_OK) == -1)
//                 {
//                     ft_fprintf(2, "minishell: %s: Permission denied\n", 
//                              cmd->redirs[i].file);
//                     exit(1);
//                 }

//                 int flags = O_WRONLY | O_CREAT;
//                 flags |= (cmd->redirs[i].type == 'A') ? O_APPEND : O_TRUNC;
                
//                 int out_fd = open(cmd->redirs[i].file, flags, 0644);
//                 if (out_fd == -1)
//                 {
//                     ft_fprintf(2, "minishell: %s: Permission denied\n", 
//                              cmd->redirs[i].file);
//                     exit(1);
//                 }
//                 dup2(out_fd, STDOUT_FILENO);
//                 close(out_fd);
//             }
//         }
//     }

//     // Configurer les pipes uniquement s'il n'y a pas de redirection correspondante
//     if (!has_output_redirection(cmd) && index < num_commands - 1)
//     {
//         if (dup2(pipes[index][1], STDOUT_FILENO) == -1)
//         {
//             perror("dup2");
//             exit(1);
//         }
//     }
    
//     if (!has_input_redirection(cmd) && index > 0)
//     {
//         if (dup2(pipes[index - 1][0], STDIN_FILENO) == -1)
//         {
//             perror("dup2");
//             exit(1);
//         }
//     }

//     // Fermer tous les descripteurs de pipe
//     for (int i = 0; i < num_commands - 1; i++)
//     {
//         close(pipes[i][0]);
//         close(pipes[i][1]);
//     }

//     // Exécuter la commande
//     if (is_builtin(cmd->args[0]))
//     {
//         char *builtin_cmd = build_command_line(cmd);
//         if (!builtin_cmd)
//             exit(1);
            
//         int ret = execute_builtin(builtin_cmd, ctx);
//         free(builtin_cmd);
//         clear_and_exit(NULL ,cmd, ret);
//         // exit(ret);
//     }
//     else
//     {
//         if (prepare_command(cmd, ctx) == -1)
//             exit(1);
//         execve(cmd->path, cmd->args, create_env_array(ctx->env_vars));
//         perror("execve");
//         clear_and_exit(NULL ,cmd, 1);
//         // exit(1);
//     }
// }

void clear_and_exit(pid_t *pids, t_command *cmds, int exit_code)
{
	free_ctx(get_ctx());
    free_env(get_ctx()->env_vars);
    (void)pids;
    // free pids
    free_tokens(get_ctx()->tokens);
    free_args(cmds->args);
    free_command(cmds);
    exit(exit_code);
}



// Exécuter une commande unique dans un pipeline
// void execute_command_in_pipeline(t_command *cmd, t_ctx *ctx, int **pipes, int index, int num_commands)
// {
//     // Rediriger l'entrée
//     redirect_input(cmd, pipes, index);

//     // Rediriger la sortie
//     redirect_output(pipes, index, num_commands);

//     // Fermer les descripteurs de pipe non utilisés
//     close_unused_pipes(pipes, index, num_commands);

//     // Préparer et exécuter la commande
//     if (prepare_command(cmd, ctx) == -1)
//         exit(1);

//     if (is_builtin(cmd->args[0]))
//     {
//         char *cmd_line = tokens_to_string_from_command(cmd);
//         execute_builtin(cmd_line, ctx);
//         free(cmd_line);
//         exit(0);
//     }
//     else
//     {
//         execute_external_command(cmd, ctx);
//         exit(1);
//     }
// }

// Attendre la fin des processus et définir le statut de sortie
// void wait_for_pipeline(pid_t *pids, int num_commands, t_ctx *ctx)
// {
//     for (int i = 0; i < num_commands; i++)
//     {
//         int status;
//         waitpid(pids[i], &status, 0);

//         if (WIFEXITED(status))
//             ctx->exit_status = WEXITSTATUS(status);
//         else if (WIFSIGNALED(status))
//             ctx->exit_status = WTERMSIG(status) + 128;
//     }
// }

int has_heredoc(t_command *cmd)
{
    if (!cmd->redirs)
        return 0;
    for (int i = 0; cmd->redirs[i].type != 0; i++)
    {
        if (cmd->redirs[i].type == 'H')
            return 1;
    }
    return 0;
}

// int execute_single_command(t_command *cmd, t_ctx *ctx)
// {
//     if (apply_redirections(cmd->redirs, ctx) == -1)
//         return -1;

//     if (is_builtin(cmd->args[0]))
//     {
//         char *cmd_line = tokens_to_string_from_command(cmd);
//         if (!cmd_line)
//             return -1;
//         ctx->exit_status = execute_builtin(cmd_line, ctx);
//         free(cmd_line);
//         free_command(cmd);
//         cleanup_shell(ctx);
//         exit(ctx->exit_status);
//     }
//     else
//     {
//         if (prepare_command(cmd, ctx) == -1)
//             return -1;
//         execve(cmd->path, cmd->args, create_env_array(ctx->env_vars));
//         // printf("MiniBG: %s command not found\n", cmd->args[0]);
//         free_command(cmd);
//         cleanup_shell(ctx);
//         exit(127);
//     }
//     return 0;
// }


// void setup_pipe_child(t_command *cmd)
// {
//     if (cmd->prev)
//     {
//         dup2(cmd->prev->pfd[0], STDIN_FILENO);
//         close(cmd->prev->pfd[0]);
//         close(cmd->prev->pfd[1]);
//     }
//     if (cmd->next)
//     {
//         close(cmd->pfd[0]);
//         dup2(cmd->pfd[1], STDOUT_FILENO);
//         close(cmd->pfd[1]);
//     }
// }

// void close_parent_pipes(t_command *cmd)
// {
//     if (cmd->prev)
//     {
//         close(cmd->prev->pfd[0]);
//         close(cmd->prev->pfd[1]);
//     }
// }

// void execute_pipeline(t_command *cmd, t_ctx *ctx)
// {
//    t_command *current = cmd;
//    int cmd_count = count_commands(cmd);
//    pid_t last_pid;
//    int status;

//    while (cmd_count > 0)
//    {
//        if (--cmd_count > 0)
//            pipe(current->pfd);
//        last_pid = fork();
//        if (last_pid == 0)
//        {
//            setup_pipe_child(current);
//            execute_single_command(current, ctx);
//            clear(NULL, current, 0);
//        }
//        close_parent_pipes(current);
//        current = current->next;
//    }
//    waitpid(last_pid, &status, 0);
//    if (WIFEXITED(status))
//        ctx->exit_status = WEXITSTATUS(status);
//    else 
//        ctx->exit_status = 1;
//     // free_command(cmd);
// }

// void execute_pipeline(t_command *cmd, t_ctx *ctx)
// {
//     printf("testttttt=========3\n");
//     t_command *current;
//     int cmd_count = 0;
//     pid_t last_pid;

//     // Traiter d'abord tous les heredocs
//     current = cmd;
//     while (current)
//     {
//         for (int i = 0; current->redirs[i].type != 0; i++)
//         {
//             if (current->redirs[i].type == 'H')
//             {
//                 current->redirs[i].heredoc_fd = here_doc(current->redirs[i].file, ctx);
//                 if (current->redirs[i].heredoc_fd == -1)
//                 {
//                     free_command(cmd);  // On libère tout en cas d'erreur
//                     return;
//                 }
//             }
//         }
//         cmd_count++;
//         current = current->next;
//     }

//     current = cmd;
//     while (current && cmd_count > 0)
//     {
//         if (--cmd_count > 0)
//             pipe(current->pfd);

//         last_pid = fork();
//         if (last_pid == -1)
//         {
//             free_command(cmd);
//             return;
//         }
        
//         if (last_pid == 0)
//         {
//             if (current->prev)
//             {
//                 dup2(current->prev->pfd[0], STDIN_FILENO);
//                 close(current->prev->pfd[0]);
//                 close(current->prev->pfd[1]);
//             }
            
//             if (current->next)
//             {
//                 close(current->pfd[0]);
//                 dup2(current->pfd[1], STDOUT_FILENO);
//                 close(current->pfd[1]);
//             }

//             if (execute_single_command(current, ctx) == -1)
//                 clear(NULL, current, 1);
//             clear(NULL, current, 0);
//         }
        
//         if (current->prev)
//         {
//             close(current->prev->pfd[0]);
//             close(current->prev->pfd[1]);
//         }
        
//         current = current->next;
//     }

//     int status;
//     waitpid(last_pid, &status, 0);
//     ctx->exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : 1;
//     free_command(cmd);  // On libère la liste de commandes à la fin
// }


// void execute_pipeline(t_command *cmd, t_ctx *ctx)
// {
//     t_command *current = cmd;
//     int cmd_count = 0;

//     // Compter les commandes et traiter les heredocs
//     while (current)
//     {
//         handle_heredocs(current, ctx);
//         cmd_count++;
//         current = current->next;
//     }

//     // Exécuter le pipeline
//     execute_commands_in_pipeline(cmd, cmd_count, ctx);
// }


// void execute_pipeline(t_command *cmd, t_ctx *ctx)
// {
//     if (!cmd)
//         return;

//     // Traiter d'abord tous les heredocs
//     t_command *current = cmd;
//     while (current)
//     {
//         if (current->redirs)
//         {
//             for (int i = 0; current->redirs[i].type != 0; i++)
//             {
//                 if (current->redirs[i].type == 'H')
//                 {
//                     int heredoc_fd = here_doc(current->redirs[i].file, ctx);
//                     if (heredoc_fd == -1)
//                         return;
//                     current->redirs[i].heredoc_fd = heredoc_fd;
//                 }
//             }
//         }
//         current = current->next;
//     }

//     // Compter le nombre de commandes
//     int num_commands = count_commands(cmd);
//     int **pipes = create_pipeline_pipes(num_commands);
//     pid_t *pids = malloc(sizeof(pid_t) * num_commands);

//     // Exécuter chaque commande
//     current = cmd;
//     int index = 0;
//     while (current)
//     {
//         pids[index] = fork();
//         if (pids[index] == -1)
//         {
//             perror("fork");
//             break;
//         }
//         if (pids[index] == 0) // Processus enfant
//         {
//             execute_command_in_pipeline(current, ctx, pipes, index, num_commands);
//         }
//         // Fermer les descripteurs de pipe dans le processus parent
//         if (index > 0)
//         {
//             close(pipes[index - 1][0]);
//             close(pipes[index - 1][1]);
//         }
//         current = current->next;
//         index++;
//     }
//     close_pipes(pipes, num_commands);
//     wait_for_pipeline(pids, num_commands, ctx);
//     free(pids);
// }



// void execute_pipeline(t_command *cmd, t_ctx *ctx)
// {
//     if (!cmd)
//         return;

//     // Compter le nombre de commandes
//     int num_commands = count_commands(cmd);

//     // Créer les pipes
//     int **pipes = create_pipeline_pipes(num_commands);
    
//     // Tableau pour stocker les PIDs
//     pid_t *pids = malloc(sizeof(pid_t) * num_commands);

//     // Exécuter chaque commande
//     t_command *current = cmd;
//     int index = 0;
//     while (current)
//     {
//         pids[index] = fork();
//         if (pids[index] == -1)
//         {
//             perror("fork");
//             break;
//         }

//         if (pids[index] == 0)  // Processus enfant
//         {
             
//             execute_command_in_pipeline(current, ctx, pipes, index, num_commands);
//         }

//         // Fermer les descripteurs de pipe dans le processus parent
//         if (index > 0)
//         {
//             close(pipes[index - 1][0]);
//             close(pipes[index - 1][1]);
//         }

//         current = current->next;
//         index++;
//     }
//     // Fermer tous les descripteurs de pipe
//     close_pipes(pipes, num_commands);

//     // Attendre la fin des processus
//     wait_for_pipeline(pids, num_commands, ctx);

//     // Libérer la mémoire
//     free(pids);
// }