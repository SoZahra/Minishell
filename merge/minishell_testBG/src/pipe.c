/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD:minishell/src/pipe.c
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:07:27 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/03 12:15:08 by llarrey          ###   ########.fr       */
=======
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:02:51 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/12 17:38:54 by fzayani          ###   ########.fr       */
>>>>>>> Fat:merge/minishell_testBG/src/pipe.c
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

<<<<<<< HEAD:minishell/src/pipe.c
char **prepare_args(t_token *tokens)
{
    int count = 0;
    t_token *current = tokens;

    // Compter les tokens (jusqu'à un PIPE ou à la fin)
    while (current && current->type != TOKEN_PIPE)
    {
        count++;
        current = current->next;
    }
    // Allouer le tableau d'arguments
    char **args = malloc((count + 1) * sizeof(char *));
    if (!args)
    {
        perror("malloc failed");
        return NULL;
    }
    // Remplir le tableau d'arguments
    current = tokens;
    int i = 0;
    while (current && current->type != TOKEN_PIPE)
    {
        args[i++] = strdup(current->value); // Chaque token est un argument
        current = current->next;
    }
    args[i] = NULL; // Terminer par NULL pour execve
    return args;
}

// void	exec(t_token *cmd_tokens, char **env)
// {
//     char	**option_cmd;
//     char	*path;
//     pid_t	pid;
//     int		status;

//     option_cmd = prepare_args(cmd_tokens);
//     if (!option_cmd[0]) {
//         fprintf(stderr, "Error: Command is empty\n");
//         free_tab_2(option_cmd);
//         return;  // Remplacer exit() par return pour que le parent ne se termine pas
//     }
//     path = get_path(option_cmd[0], env);// Obtenir le chemin de la commande
//     pid = fork();  // Créer un processus enfant
//     if (pid == -1)  // Si fork échoue
//     {
//         perror("fork failed");
//         free_tab_2(option_cmd);
//         return;
//     }
//     if (pid == 0)  // Dans le processus enfant
//     {
//         if (execve(path, option_cmd, env) == -1)  // Exécuter la commande
//         {
//             perror("exec command");
//             free_tab_2(option_cmd);
//             exit(EXIT_FAILURE);  // Terminer l'enfant en cas d'échec
//         }
//     }
//     else  // Dans le processus parent
//         waitpid(pid, &status, 0);  // Attendre que l'enfant se termine
//     free_tab_2(option_cmd);// Nettoyer la mémoire dans le parent
// }


void	exec(t_token *cmd_tokens, char **env)
{
	char	**option_cmd;
	char	*path;

	option_cmd = prepare_args(cmd_tokens);
	if (!option_cmd[0]) {
    fprintf(stderr, "Error: Command is empty\n");
    free_tab_2(option_cmd);
    exit(EXIT_FAILURE);
	}
	if (!option_cmd || !option_cmd[0])
	{
		free_tab_2(option_cmd);
		perror("Invalid command");
		exit(EXIT_FAILURE);
	}
	path = get_path(option_cmd[0], env);
	if(execve(path, option_cmd, env) == -1)
	{
		perror("exec command");
		free_tab_2(option_cmd);
		exit(EXIT_FAILURE);
	}
}

void child(t_token *tokens, int *pipe_fd, char **env) {
    close(pipe_fd[0]); // Fermer la lecture du pipe dans l'enfant
    dup2(pipe_fd[1], STDOUT_FILENO); // Rediriger la sortie vers le pipe
    close(pipe_fd[1]); // Fermer après redirection
    exec(tokens, env);
}

void parent(t_token *tokens, int *pipe_fd, char **env) {
    close(pipe_fd[1]); // Fermer l'écriture du pipe dans l'enfant
    dup2(pipe_fd[0], STDIN_FILENO); // Rediriger l'entrée vers le pipe
    close(pipe_fd[0]); // Fermer après redirection
    exec(tokens, env);
}

int check_consecutive_pipes(t_token *tokens)
{
    t_token *current = tokens;

    while (current)
    {
        // Vérifie s'il y a deux pipes consécutifs ou si un pipe est suivi de NULL
        if (current->type == TOKEN_PIPE && (!current->next || current->next->type == TOKEN_PIPE))
        {
            fprintf(stderr, "Error: Consecutive pipes or missing command\n");
            return -1;  // Erreur trouvée
        }
        current = current->next;
    }
    return 0;  // Pas d'erreur
}

int contains_pipe(t_token *tokens)
{
    while (tokens)
    {
        if (tokens->type == TOKEN_PIPE)
            return 1; // Un pipe trouvé
        tokens = tokens->next;
    }
    return 0; // Aucun pipe trouvé
}


// void	child(t_token *tokens, int *pipe_fd, char **env)
// {
// 	close(pipe_fd[0]); // Fermer la lecture du pipe
//     dup2(pipe_fd[1], STDOUT_FILENO); // Rediriger la sortie standard vers le pipe
//     close(pipe_fd[1]); // Fermer l'écriture après redirection
//     exec(tokens, env);
// }

// void	parent(t_token *tokens, int *pipe_fd, char **env)
// {
// 	close(pipe_fd[1]); // Fermer l'écriture du pipe
//     dup2(pipe_fd[0], STDIN_FILENO); // Rediriger l'entrée standard vers le pipe
//     close(pipe_fd[0]); // Fermer la lecture après redirection
//     exec(tokens, env);
// }

// int	main(int ac, char *argv[], char *env[])
// {
// 	pid_t	pid1;
// 	pid_t	pid2;
// 	int		pipe_fd[2];
// 	int		status;

// 	if (ac != 5 || env == NULL)
// 		return (write(2, "check the arguments / env potentialy empty", 43), 0);
// 	if (pipe(pipe_fd) == -1)
// 		exit_error();
// 	pid1 = fork();
// 	if (pid1 == -1)
// 		exit_error();
// 	if (pid1 == 0)
// 		child(argv, pipe_fd, env);
// 	pid2 = fork();
// 	if (pid2 == -1)
// 		exit_error();
// 	if (pid2 == 0)
// 		parent(argv, pipe_fd, env);
// 	close(pipe_fd[0]);
// 	close(pipe_fd[1]);
// 	waitpid(pid1, &status, 0);
// 	waitpid(pid2, &status, 0);
// 	return (0);
// }
=======
// void clear_all(t_ctx *ctx)
// {


//     // clear env
//     // tokens
//     // cmds
// }

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

    // Appliquer les redirections locales si elles existent
    if (cmd->redirs && apply_redirections(cmd->redirs) == -1)
    {
        perror("redirection");
        exit(1);
    }

    // Exécuter la commande
    if (is_builtin(cmd->args[0]))
    {
        execute_builtin(cmd->args[0], ctx);
        exit(ctx->exit_status);  // Important!
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
>>>>>>> Fat:merge/minishell_testBG/src/pipe.c
