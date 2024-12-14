/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:24:28 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 13:30:15 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **create_command_array(const char *cmd_str)
{
    // printf("Debug: Creating array for command: '%s'\n", cmd_str);

    // Ignorer les espaces au début
    while (*cmd_str == ' ')
        cmd_str++;

    char **cmd_array = ft_split(cmd_str, ' ');
    if (!cmd_array)
        return NULL;

    // Compter le nombre d'arguments pour debug
    // int i = 0;
    // while (cmd_array[i])
    // {
    //     printf("Debug: arg[%d]: '%s'\n", i, cmd_array[i]);
    //     i++;
    // }

    return cmd_array;
}

// static void cleanup_exec_resources(char *cmd_path, char **cmd_array, char **env)
// {
//     free(cmd_path);
//     free_array(cmd_array);
//     free_array(env);
// }

// static void handle_command_not_found(const char *cmd, char **cmd_array, t_ctx *ctx)
// {
//     fprintf(stderr, "MiniBG: %s: command not found\n", cmd);
//     free_array(cmd_array);
//     ctx->exit_status = 127;
// }

// void execute_child_process(char *cmd_path, char **cmd_array, char **env)
// {
//     t_redir *redirs = get_redirections(cmd_array);
//     if (redirs)
//     {
//         if (apply_redirections(redirs) == -1)
//             exit(1);
//         free_redirections(redirs);
//     }
//     execve(cmd_path, cmd_array, env);
//     perror("execve");
//     exit(1);
// }

void wait_for_child(pid_t pid, t_ctx *ctx)
{
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        ctx->exit_status = WEXITSTATUS(status);
}

// void execute_external_command(const char *cmd_str, t_ctx *ctx)
// {
//     printf("Debug: Executing command: '%s'\n", cmd_str);

//     char **cmd_array = create_command_array(cmd_str);
//     if (!cmd_array)
//         return;

//     // Debug: Afficher le tableau initial
//     printf("Debug: Initial command array:\n");
//     for (int i = 0; cmd_array[i]; i++)
//         printf("  arg[%d]: '%s'\n", i, cmd_array[i]);

//     // Extraire les redirections
//     t_redir *redirs = get_redirections(cmd_array);
//     print_redirections(redirs);

//     char *cmd_path = find_command_path(cmd_array[0], ctx);
//     if (!cmd_path)
//     {
//         handle_command_not_found(cmd_array[0], cmd_array, ctx);
//         free_redirections(redirs);
//         return;
//     }

//     char **env = create_env_array(ctx->env_vars);
//     if (!env)
//     {
//         cleanup_exec_resources(cmd_path, cmd_array, NULL);
//         free_redirections(redirs);
//         return;
//     }

//     pid_t pid = fork();
//     if (pid == -1)
//     {
//         perror("fork");
//         cleanup_exec_resources(cmd_path, cmd_array, env);
//         free_redirections(redirs);
//         return;
//     }

//     if (pid == 0)
//     {
//         // Dans le processus enfant, appliquer les redirections
//         if (redirs && apply_redirections(redirs) == -1)
//             exit(1);
//         execve(cmd_path, cmd_array, env);
//         perror("execve");
//         exit(1);
//     }
//     else
//         wait_for_child(pid, ctx);

//     cleanup_exec_resources(cmd_path, cmd_array, env);
//     free_redirections(redirs);
// }

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

char *join_path(const char *dir, const char *file)
{
    size_t len_dir = strlen(dir);
    size_t len_file = strlen(file);
    char *full_path = malloc(len_dir + len_file + 2); // +1 pour '/' et +1 pour '\0'

    if (!full_path)
    {
        perror("malloc");
        return NULL;
    }

    strcpy(full_path, dir);
    if (full_path[len_dir - 1] != '/')
        strcat(full_path, "/");
    strcat(full_path, file);

    return full_path;
}

int is_executable(const char *path)
{
    struct stat st;
    if (stat(path, &st) == 0 && (st.st_mode & S_IXUSR))
        return 1;
    return 0;
}

char *find_command_path(const char *command, t_ctx *ctx)
{
    (void)ctx;
    // Vérifier si la commande est déjà un chemin absolu ou relatif
    if (strchr(command, '/'))
    {
        if (is_executable(command))
        {
            // fprintf(stderr, "Debug: Command '%s' is directly executable.\n", command);
            return strdup(command);
        }
        // fprintf(stderr, "Debug: Command '%s' is not executable or does not exist.\n", command);
        return NULL;
    }

    // Récupérer la variable PATH
    char *path_env = getenv("PATH");
    if (!path_env)
    {
        // fprintf(stderr, "Debug: PATH environment variable is not set.\n");
        return NULL;
    }

    // Séparer les chemins dans PATH
    char *path_copy = strdup(path_env);
    if (!path_copy)
    {
        perror("strdup");
        return NULL;
    }

    char *token = strtok(path_copy, ":");
    while (token)
    {
        char *full_path = join_path(token, command);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }
        // fprintf(stderr, "Debug: Checking path: %s\n", full_path);
        if (is_executable(full_path))
        {
            // fprintf(stderr, "Debug: Found command '%s' at %s\n", command, full_path);
            free(path_copy);
            return full_path;
        }

        free(full_path);
        token = strtok(NULL, ":");
    }

    // fprintf(stderr, "Debug: Command '%s' not found in PATH.\n", command);
    free(path_copy);
    return NULL;
}


// char *find_command_path(const char *cmd, t_ctx *ctx)
// {
//     // Si la commande contient un '/', c'est déjà un chemin
//     if (ft_strchr(cmd, '/'))
//         return ft_strdup(cmd);

//     // Récupérer le PATH
//     char *path = get_env_path(ctx->env_vars);
//     if (!path)
//         return NULL;

//     // Diviser le PATH en dossiers
//     char **directories = ft_split(path, ':');
//     if (!directories)
//         return NULL;

//     // Chercher la commande dans chaque dossier
//     char *cmd_path = NULL;
//     for (int i = 0; directories[i]; i++)
//     {
//         // Construire le chemin complet
//         cmd_path = ft_strjoin(directories[i], "/");
//         char *temp = ft_strjoin(cmd_path, cmd);
//         free(cmd_path);
//         cmd_path = temp;

//         // Vérifier si le fichier existe et est exécutable
//         if (access(cmd_path, X_OK) == 0)
//         {
//             free_array(directories);
//             return cmd_path;
//         }
//         free(cmd_path);
//     }

//     free_array(directories);
//     return NULL;
// }

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

// void remove_elements(char **array, int start, int count)
// {
//     int i = start;

//     // Libérer les éléments à supprimer
//     for (int j = 0; j < count && array[i + j]; j++)
//         free(array[i + j]);

//     // Décaler tous les éléments suivants
//     while (array[i + count])
//     {
//         array[i] = array[i + count];
//         i++;
//     }
//     array[i] = NULL;
// }

// void free_redirections(t_redir *redirs)
// {
//     t_redir *current;
//     t_redir *next;

//     current = redirs;
//     while (current)
//     {
//         next = current->next;
//         free(current->file);
//         free(current);
//         current = next;
//     }
// }

// // Fonction auxiliaire pour ajouter une redirection à la liste
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

char *tokens_to_string_from_command(t_command *cmd)
{
    // Si aucun argument, retourner une chaîne vide
    if (!cmd->args || cmd->arg_count == 0)
        return ft_strdup("");

    // Commencer par le premier argument
    char *result = ft_strdup(cmd->args[0]);
    if (!result)
        return NULL;

    // Ajouter les arguments suivants
    for (int i = 1; i < cmd->arg_count; i++)
    {
        char *temp;

        // Ajouter un espace si nécessaire
        if (!cmd->had_spaces[i])
        {
            temp = ft_strjoin(result, " ");
            free(result);
            result = temp;
        }

        // Ajouter l'argument
        temp = ft_strjoin(result, cmd->args[i]);
        free(result);
        result = temp;
    }

    return result;
}

void execute_command(t_command *cmd, t_ctx *ctx)
{
   // Sauvegarder les FD originaux
   int stdin_backup = dup(STDIN_FILENO);
   int stdout_backup = dup(STDOUT_FILENO);

    t_command *current = cmd;
    while (current)
	{
        fprintf(stderr, "Debug: Command: '%s', Args: ", current->args[0]);
        for (int i = 0; i < current->arg_count; i++) {
            fprintf(stderr, "'%s' ", current->args[i]);
        }
    fprintf(stderr, "\n");
    current = current->next;
}
   // Appliquer toutes les redirections
   if (apply_redirections(cmd->redirs) == -1)
   {
       restore_fds(stdin_backup, stdout_backup);
       return;
   }
   if (is_builtin(cmd->args[0]))
    {
		char *cmd_line = tokens_to_string_from_command(cmd);
		execute_builtin(cmd_line, ctx);
		free(cmd_line);
	}
		// execute_builtin(cmd, ctx);
   else
       execute_external_command(cmd, ctx);
   restore_fds(stdin_backup, stdout_backup);
}

// void execute_command(t_command *cmd, t_ctx *ctx)
// {
//    // Sauvegarder les FD originaux
//    int stdin_backup = dup(STDIN_FILENO);
//    int stdout_backup = dup(STDOUT_FILENO);

//    // Créer des tokens à partir des arguments de la commande
//    t_token *tokens = create_tokens_from_command(cmd);

//    // Expansion des variables
//    if (expand_proc(&tokens, ctx) == -1)
//    {
//        free_tokens(tokens);
//        restore_fds(stdin_backup, stdout_backup);
//        return;
//    }

//    // Mettre à jour les arguments de la commande avec les tokens expandus
//    update_command_from_tokens(cmd, tokens);

//    // Libérer les tokens temporaires
//    free_tokens(tokens);

//    // Appliquer toutes les redirections
//    if (apply_redirections(cmd->redirs) == -1)
//    {
//        restore_fds(stdin_backup, stdout_backup);
//        return;
//    }

//    // Exécuter la commande
//    if (is_builtin(cmd->args[0]))
//        execute_builtin_command(cmd, ctx);
//    else
//        execute_external_command(cmd, ctx);

//    // Restaurer les FD originaux
//    restore_fds(stdin_backup, stdout_backup);
// }

// // Fonction pour créer des tokens à partir des arguments de la commande
// t_token *create_tokens_from_command(t_command *cmd)
// {
//     t_token *tokens = NULL;
//     for (int i = 0; i < cmd->arg_count; i++)
//     {
//         add_token(&tokens, 'S', cmd->args[i]);
//     }
//     return tokens;
// }

// // Fonction pour mettre à jour la commande avec les tokens expandus
// void update_command_from_tokens(t_command *cmd, t_token *tokens)
// {
//     // Libérer les anciens arguments
//     for (int i = 0; i < cmd->arg_count; i++)
//     {
//         free(cmd->args[i]);
//     }
//     free(cmd->args);
//     free(cmd->had_spaces);

//     // Compter les nouveaux tokens
//     int new_arg_count = 0;
//     t_token *current = tokens;
//     while (current)
//     {
//         new_arg_count++;
//         current = current->next;
//     }

//     // Allouer de nouveaux tableaux
//     cmd->args = malloc(sizeof(char *) * (new_arg_count + 1));
//     cmd->had_spaces = malloc(sizeof(int) * new_arg_count);
//     cmd->arg_count = new_arg_count;

//     // Copier les nouveaux arguments
//     current = tokens;
//     for (int i = 0; current; i++)
//     {
//         cmd->args[i] = strdup(current->value);
//         cmd->had_spaces[i] = current->had_space;
//         current = current->next;
//     }
//     cmd->args[new_arg_count] = NULL;
// }

int apply_redirections(t_redirection *redirs)
{
    if (!redirs || redirs->type == 0)
        return 0;

    t_redirection *current = redirs;
    while (current && current->type != 0)
    {
        if (current->type == '>' || current->type == 'A')
        {
            // Redirection sortante
            int flags = O_WRONLY | O_CREAT;
            flags |= (current->type == 'A') ? O_APPEND : O_TRUNC;

            int fd = open(current->file, flags, 0644);
            if (fd == -1)
            {
                perror("open");
                return -1;
            }

            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }
            close(fd);
        }
        else if (current->type == '<' || current->type == 'H')
        {
            // Redirection entrante
            int fd = open(current->file, O_RDONLY);
            if (fd == -1)
            {
                perror("open");
                return -1;
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return -1;
            }
            close(fd);
        }
        current = current->next;
    }

    return 0;
}

// int apply_redirections(t_redirection *redirs)
// {
//     t_redirection *current = redirs;
//     int last_fd = -1;

//     // D'abord inverser l'ordre de la liste pour traiter la dernière redirection en premier
//     t_redirection *reversed = NULL;
//     if(!current)
//         return -1;
//     while (current)
//     {
//         t_redirection *next = current->next;
//         current->next = reversed;
//         reversed = current;
//         current = next;
//     }

//     // Maintenant traiter les redirections dans l'ordre inverse
//     current = reversed;
//     while (current)
//     {
//         if (current->type == '>' || current->type == 'A')
//         {
//             // Si on avait déjà ouvert un fichier, le fermer
//             if (last_fd != -1)
//                 close(last_fd);

//             // Redirection sortante
//             int flags = O_WRONLY | O_CREAT;
//             flags |= (current->type == 'A') ? O_APPEND : O_TRUNC;

//             current->fd = open(current->file, flags, 0644);
//             if (current->fd == -1)
//             {
//                 perror("open");
//                 return -1;
//             }

//             if (dup2(current->fd, STDOUT_FILENO) == -1)
//             {
//                 perror("dup2");
//                 return -1;
//             }
//             close(current->fd);

//             last_fd = current->fd;
//         }
//         current = current->next;
//     }

//     return 0;
// }

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
char *args_to_string(char **args)
{
    if (!args || !args[0])
        return NULL;

    // Calculer la longueur totale nécessaire
    size_t total_len = 0;
    for (int i = 0; args[i]; i++)
    {
        total_len += ft_strlen(args[i]);
        if (args[i + 1]) // Ajouter un espace si ce n'est pas le dernier argument
            total_len++;
    }

    // Allouer la mémoire pour la chaîne finale
    char *result = malloc(total_len + 1);
    if (!result)
        return NULL;
    // Construire la chaîne
    result[0] = '\0';
    for (int i = 0; args[i]; i++)
    {
        strcat(result, args[i]);
        if (args[i + 1]) // Ajouter un espace si ce n'est pas le dernier argument
            strcat(result, " ");
    }
    return result;
}

void execute_builtin_command(t_command *cmd, t_ctx *ctx)
{
   char *cmd_str = args_to_string(cmd->args);
//    printf("Debug: cmd->argssss[0]: '%s'\n", cmd->args[0]);
//     printf("Debug: cmd->argssss[1]: '%s'\n", cmd->args[1]);
   if (cmd_str)
   {
       ctx->exit_status = execute_builtin(cmd_str, ctx);
       free(cmd_str);
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

    // Libérer le tableau d'arguments
    if (cmd->args)
    {
        for (int i = 0; cmd->args[i]; i++)
            free(cmd->args[i]);
        free(cmd->args);
    }

    // Libérer les redirections
    t_redirection *current = cmd->redirs;
    while (current)
    {
        t_redirection *next = current->next;
        if (current->file)
            free(current->file);
        free(current);
        current = next;
    }

    // Libérer la structure elle-même
    free(cmd);
}
