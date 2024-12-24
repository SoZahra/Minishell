/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:24:28 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/24 16:58:19 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **create_command_array(const char *cmd_str)
{
    // printf("Debug: Creating array for command: '%s'\n", cmd_str);

    while (*cmd_str == ' ')
        cmd_str++;

    char **cmd_array = ft_split(cmd_str, ' ');
    if (!cmd_array)
        return NULL;
    return cmd_array;
}



// void wait_for_child(pid_t pid, t_ctx *ctx)
// {
//     int status;
//     waitpid(pid, &status, 0);
//     if (WIFEXITED(status))
//         ctx->exit_status = WEXITSTATUS(status);
// }


// char *get_env_path(t_env_var *env_vars)
// {
//     t_env_var *current = env_vars;

//     while (current)
//     {
//         if (ft_strcmp(current->name, "PATH") == 0)
//             return current->value;
//         current = current->next;
//     }
//     return NULL;
// }

char *join_path(const char *dir, const char *file)
{
    size_t len_dir;
    size_t len_file;
    char *full_path;

    len_file = ft_strlen(file);
    len_dir = ft_strlen(dir);
    full_path = malloc(len_dir + len_file + 2);
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
    char *path_env;
    char *path_copy;
    char *token;
    char *full_path;
     
    if (strchr(command, '/'))
    {
        if (is_executable(command))
            return strdup(command);
        return NULL;
    }
    path_env = ps_get_env_var("PATH", ctx);
    if (!path_env)
        return NULL;
    path_copy = ft_strdup(path_env);
    if (!path_copy)
    {
        perror("strdup");
        return NULL;
    }
    token = strtok(path_copy, ":");
    while (token)
    {
        full_path = join_path(token, command);
        if (!full_path)
        {
            return (NULL);
        }
        if (is_executable(full_path))
        {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        token = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

char **create_env_array(t_env_var *env_vars)
{
    int count = 0;
    t_env_var *current = env_vars;
    while (current)
    {
        count++;
        current = current->next;
    }
    char **env = malloc(sizeof(char *) * (count + 1));
    if (!env)
        return NULL;
    current = env_vars;
    count = 0;
    while (current)
    {
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

char *tokens_to_string_from_command(t_command *cmd)
{
    if (!cmd->args || cmd->arg_count == 0)
        return ft_strdup("\0");

    char *result = ft_strdup(cmd->args[0]);
    if (!result)
        return (NULL);
    for (int i = 1; i < cmd->arg_count; i++)
    {
        char *temp;
        if (!cmd->had_spaces[i])
        {
            temp = ft_strjoin(result, " ");
             if (!temp)
            {
                free_command_list(cmd);
                return NULL;
            }
            free(result);
            result = temp;
        }
        temp = ft_strjoin(result, cmd->args[i]);
        if (!temp)
        {
            free_command_list(cmd);
            return NULL;
        }
        free(result);
        result = temp;
    }
    return result;
}
