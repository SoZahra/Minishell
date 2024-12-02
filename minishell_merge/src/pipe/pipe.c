/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:07:27 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 10:32:42 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


char **prepare_args(t_token *tokens, t_ctx *ctx)
{
	(void)ctx;
    int count = 0;
    t_token *current = tokens;
    char **args;
    int i = 0;

    // Compter les arguments jusqu'au prochain pipe
    while (current && current->type != TOKEN_PIPE)
    {
        count++;
        current = current->next;
    }

    args = malloc((count + 1) * sizeof(char *));
    if (!args)
    {
        perror("malloc failed");
        return NULL;
    }
    current = tokens;
    i = 0;
    while (current && current->type != TOKEN_PIPE)
    {
        args[i] = ft_strdup(current->value); // Dupliquer la valeur après expansion
        if (!args[i])
        {
            perror("Duplication de mémoire échouée");
            for (int j = 0; j < i; j++)
                free(args[j]);
            free(args);
            return NULL;
        }
        i++;
        current = current->next;
    }
    args[i] = NULL;  // Ajouter NULL à la fin pour `execve`
    return args;
}

// char **prepare_args(t_token *tokens, int *exit_status)
// {
// 	(void)exit_status;
//     int count = 0;
//     t_token *current = tokens;
//     char **args;
//     int i = 0;

//     // Compter les arguments jusqu'au prochain pipe
//     while (current && current->type != TOKEN_PIPE)
//     {
//         count++;
//         current = current->next;
//     }

//     args = malloc((count + 1) * sizeof(char *));
//     if (!args)
//     {
//         perror("malloc failed");
//         return NULL;
//     }

//     current = tokens;
//     i = 0;
//     while (current && current->type != TOKEN_PIPE)
//     {
//         args[i] = strdup(current->value); // Dupliquer la valeur après expansion
//         if (!args[i])
//         {
//             perror("Duplication de mémoire échouée");
//             for (int j = 0; j < i; j++)
//                 free(args[j]);
//             free(args);
//             return NULL;
//         }
//         i++;
//         current = current->next;
//     }
//     args[i] = NULL;  // Ajouter NULL à la fin pour `execve`
//     return args;
// }


// void	exec(t_token *cmd_tokens, char **env)
// {
// 	char	**option_cmd;
// 	char	*path;

// 	option_cmd = prepare_args(cmd_tokens);
// 	if (!option_cmd[0])
// 	{
// 		fprintf(stderr, "Error: Command is empty\n");
// 		free_tab_2(option_cmd);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (strcmp(option_cmd[0], "cd") == 0)
// 	{
// 		ft_cd(option_cmd);
// 		free_tab_2(option_cmd);
// 		return ;
// 	}
// 	path = get_path(option_cmd[0], env);
// 	if (execve(path, option_cmd, env) == -1)
// 	{
// 		perror("exec command");
// 		free_tab_2(option_cmd);
// 		exit(EXIT_FAILURE);
// 	}
// 	free_tab_2(option_cmd);
// }

char **convert_env_to_array(t_env_var *env)
{
    int count = 0;
    t_env_var *current = env;
    char **env_array;

    // Compter le nombre de variables
    while (current)
    {
        count++;
        current = current->next;
    }

    // Allouer de la mémoire pour le tableau
    env_array = malloc((count + 1) * sizeof(char *));
    if (!env_array)
        return NULL;

    current = env;
    for (int i = 0; i < count; i++)
    {
        int len = strlen(current->name) + strlen(current->value) + 2; // +2 pour '=' et '\0'
        env_array[i] = malloc(len);
        if (!env_array[i])
        {
            // Libérer en cas d'erreur
            for (int j = 0; j < i; j++)
                free(env_array[j]);
            free(env_array);
            return NULL;
        }
        snprintf(env_array[i], len, "%s=%s", current->name, current->value);
        current = current->next;
    }
    env_array[count] = NULL; // Terminer par un NULL
    return env_array;
}



// int exec(t_token *cmd_tokens, t_var *env, t_ctx *ctx)
// {
//     char **option_cmd;
//     char *path;
//     struct stat path_stat;

//     option_cmd = prepare_args(cmd_tokens, ctx);
//     if (!option_cmd[0])
//     {
//         fprintf(stderr, "Error: Command is empty\n");
//         free_tab(option_cmd);
//         exit(EXIT_FAILURE);
//     }

//     // Special case for "$"
//     if (strcmp(option_cmd[0], "$") == 0)
//     {
//         fprintf(stderr, "%s: command not found\n", option_cmd[0]);
//         free_tab(option_cmd);
//         exit(127);
//     }

//     // Expand variables for the first argument if it starts with "$"
//     if (option_cmd[0][0] == '$')
//     {
//         char *expanded = expand_variables(option_cmd[0], ctx, STRING);
//         if ((!expanded || expanded[0] == '\0') && option_cmd[1])
//         {
//             free(expanded);
//             free(option_cmd[0]);
//             for (int i = 0; option_cmd[i + 1]; i++)
//                 option_cmd[i] = option_cmd[i + 1];
//             option_cmd[count_args(option_cmd) - 1] = NULL;
//         }
//         else
//         {
//             free(option_cmd[0]);
//             option_cmd[0] = expanded;
//         }

//         if (!option_cmd[0] || option_cmd[0][0] == '\0')
//         {
//             free_tab(option_cmd);
//             exit(0);  // Exit with success
//         }
//     }

//     // Check if it's a directory
//     if (stat(option_cmd[0], &path_stat) == 0)
//     {
//         if (S_ISDIR(path_stat.st_mode))
//         {
//             // Handle explicitly called directories (e.g., ./test_files)
//             if (option_cmd[0][0] == '.' || option_cmd[0][0] == '/')
//             {
//                 fprintf(stderr, "bash: %s: Is a directory\n", option_cmd[0]);
//                 free_tab(option_cmd);
//                 exit(126);
//             }
//             else // Implicit directory call (e.g., test_files)
//             {
//                 fprintf(stderr, "%s: command not found\n", option_cmd[0]);
//                 free_tab(option_cmd);
//                 exit(127);
//             }
//         }
//     }

//     // Handle explicit paths (e.g., "./missing.out")
//     if (option_cmd[0][0] == '.' || option_cmd[0][0] == '/')
//     {
//         if (access(option_cmd[0], F_OK) != 0)
//         {
//             fprintf(stderr, "%s: No such file or directory\n", option_cmd[0]);
//             free_tab(option_cmd);
//             exit(127);
//         }
//         else if (access(option_cmd[0], X_OK) != 0)
//         {
//             fprintf(stderr, "%s: Permission denied\n", option_cmd[0]);
//             free_tab(option_cmd);
//             exit(126);
//         }
//     }
//     // Find the command in PATH
//     path = get_path(option_cmd[0], env);
//     if (execve(path, option_cmd, env) == -1)
//     {
//         fprintf(stderr, "%s: command not found\n", option_cmd[0]);
//         free_tab(option_cmd);
//         exit(127);
//     }

//     free_tab(option_cmd);
//     return 0;
// }


int exec(t_token *cmd_tokens, t_var *env, t_ctx *ctx)
{
    char **option_cmd;
    char *path;
    struct stat path_stat;

    option_cmd = prepare_args(cmd_tokens, ctx);
    if (!option_cmd || !option_cmd[0])
    {
        free_tab(option_cmd);
        exit(127);
    }

    // Special case for "$" and "$?"
    if (ft_strcmp(option_cmd[0], "$") == 0 || ft_strcmp(option_cmd[0], "$?") == 0)
    {
        fprintf(stderr, "%s: command not found\n", option_cmd[0]);
        free_tab(option_cmd);
        exit(127);
    }

    // Handle environment variables
    if (option_cmd[0][0] == '$')
    {
        char *expanded = expand_variables(option_cmd[0], ctx, STRING);
        if ((!expanded || expanded[0] == '\0') && option_cmd[1])
        {
            free(expanded);
            for (int i = 0; option_cmd[i + 1]; i++)
                option_cmd[i] = option_cmd[i + 1];
            option_cmd[count_args(option_cmd) - 1] = NULL;
        }
        else
        {
            free(option_cmd[0]);
            option_cmd[0] = expanded;
        }

        if (!option_cmd[0] || option_cmd[0][0] == '\0')
        {
            free_tab(option_cmd);
            exit(0);
        }
    }

    // Check if it's a directory
    if (stat(option_cmd[0], &path_stat) == 0)
    {
        if (S_ISDIR(path_stat.st_mode))
        {
            if (option_cmd[0][0] == '.' || option_cmd[0][0] == '/')
            {
                fprintf(stderr, "%s: Is a directory\n", option_cmd[0]);
                free_tab(option_cmd);
                exit(126);
            }
            else
            {
                fprintf(stderr, "%s: command not found\n", option_cmd[0]);
                free_tab(option_cmd);
                exit(127);
            }
        }
    }

    // Handle explicit paths
    if (option_cmd[0][0] == '.' || option_cmd[0][0] == '/')
    {
        if (access(option_cmd[0], F_OK) != 0)
        {
            fprintf(stderr, "%s: No such file or directory\n", option_cmd[0]);
            free_tab(option_cmd);
            exit(127);
        }
        if (access(option_cmd[0], X_OK) != 0)
        {
            fprintf(stderr, "%s: Permission denied\n", option_cmd[0]);
            free_tab(option_cmd);
            exit(126);
        }
        path = ft_strdup(option_cmd[0]);
    }
    else
    {
        // Try to find in PATH
        path = get_path(option_cmd[0], ctx->env_vars);
        if (!path)
        {
            fprintf(stderr, "%s: command not found\n", option_cmd[0]);
            free_tab(option_cmd);
            exit(127);
        }
    }

    if (execve(path, option_cmd, env->env) == -1)
    {
        free(path);
        free_tab(option_cmd);
        exit(127);
    }

    free(path);
    free_tab(option_cmd);
    return 0;
}


void	child(t_token *tokens, int *pipe_fd, t_var *env, t_ctx *ctx)
{
	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	exec(tokens, env, ctx);
}

void	parent(t_token *tokens, int *pipe_fd, t_var *env, t_ctx *ctx)
{
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	exec(tokens, env, ctx);
}

int	check_consecutive_pipes(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE && (!current->next
				|| current->next->type == TOKEN_PIPE))
		{
			fprintf(stderr, "Error: Consecutive pipes or missing command\n");
			return (-1);
		}
		current = current->next;
	}
	return (0);
}

int	contains_pipe(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
			return (1);
		tokens = tokens->next;
	}
	return (0);
}
