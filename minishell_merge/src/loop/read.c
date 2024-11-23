/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:37:16 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 14:25:04 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	exec_cmd(t_token *cmd, int fd_in, int pipe_fd[2], int last_cmd)
{
	char	**args;

	args = prepare_print_args(cmd);
	if (fd_in != 0)
		dup2(fd_in, STDIN_FILENO);
	if (!last_cmd)
		dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	if (execvp(args[0], args) == -1)
	{
		perror("Échec de exec");
		free(args);
		exit(EXIT_FAILURE);
	}
}

void	process_tokens(t_token *cmd_tokens, int num_pipes)
{
	int		pipe_fd[2];
	int		fd_in;
	int		j;
	pid_t	pid;

	fd_in = 0;
	j = 0;
	print_cmds(cmd_tokens, num_pipes);
	while (j <= num_pipes)
	{
		if (j < num_pipes)
			create_pipe(pipe_fd);
		pid = fork();
		if (pid == -1)
		{
			perror("Echec de fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
			handle_child(&cmd_tokens[j], fd_in, pipe_fd, j == num_pipes);
		else
			handle_parent(pipe_fd, &fd_in, pid);
		j++;
	}
}

// void	exec_simple_cmd(t_token *tokens, char **env)
// {
// 	char	**args;
// 	pid_t	pid;

// 	args = prepare_print_args(tokens);
// 	if(exec_builtin_cmd(args, env))
// 	{
// 		free(args);
// 		return ;
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("Echec fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (pid == 0)
// 	{
// 		exec(tokens, env);
// 		exit(0);
// 	}
// 	waitpid(pid, NULL, 0);
// 	free(args);
// }

int count_tokens(t_token *tokens)
{
    int count = 0;

	while(tokens)
	{
		count++;
		tokens = tokens->next;
	}
    return (count);
}

void print_env(char **env)
{
    int i = 0;

    while (env && env[i]) // Boucle tant que les éléments sont valides
    {
        printf("%s\n", env[i]);
        i++;
    }
}

int exec_simple_cmd(t_token *tokens, t_var *myEnv, t_ctx *ctx)
{
    char **args;
    pid_t pid;
	int status;

    args = prepare_args(tokens, ctx); // Passer les tokens ici, pas une chaîne de caractères
    if (!args)
	{
        perror("Erreur d'allocation de mémoire pour les arguments");
		ctx->exit_status = 1;
        return 0;
    }
    if (exec_builtin_cmd(args, myEnv, ctx))
	{
        free(args);
		// ctx->exit_status = 0;
        return 0;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("Echec fork");
		ctx->exit_status = 1;
        free(args);
        return (-1);
    }
    if (pid == 0)
	{   // Processus enfant: exécuter la commande
        exec(tokens, myEnv, ctx); // Assurez-vous que cette fonction exécute correctement `execve`
        free(args); // Libérer les arguments dans le processus enfant aussi
        exit(127);
    }
    waitpid(pid, &status, 0);
	if(WIFEXITED(status))
		ctx->exit_status = WEXITSTATUS(status);
	else
		ctx->exit_status = 1;
    free(args); // Libérer les arguments après exécution
	return 0;
}

// void exec_simple_cmd(t_token *tokens, char **env, t_ctx *ctx)
// {
//     char **args;
//     pid_t pid;

//     // 1. Étendre les variables d'environnement dans chaque token
//     ps_expand_env(tokens, ctx);

//     // 2. Préparer les arguments après l'expansion
//     args = prepare_args(tokens, &ctx->exit_status);
//     if (!args) {
//         perror("Erreur d'allocation de mémoire pour les arguments");
//         return;
//     }

//     // 3. Vérifier si la commande est une commande interne (builtin)
//     if (exec_builtin_cmd(args, env)) {
//         free(args);
//         return;
//     }

//     // 4. Fork pour exécuter la commande externe
//     pid = fork();
//     if (pid == -1) {
//         perror("Echec fork");
//         free(args);
//         exit(EXIT_FAILURE);
//     }
//     if (pid == 0) {
//         // Processus enfant: exécuter la commande
//         exec(tokens, env); // Assurez-vous que cette fonction exécute correctement `execve`
//         free(args); // Libérer les arguments dans le processus enfant aussi
//         exit(0);
//     }

//     // 5. Processus parent: attendre la fin de la commande
//     waitpid(pid, NULL, 0);
//     free(args); // Libérer les arguments après exécution
// }


// void split_env_v(const char *input, char **var, char **value)
// {
// 	const char *equal_sign;

// 	equal_sign = ft_strchr(input, '=');
// 	if(equal_sign)
// 	{
// 		size_t	var_len = equal_sign - input;
// 		*var = ft_strndup(input, var_len);
// 		*value = ft_strdup(equal_sign + 1); //apres le =
// 	}
// 	else
// 	{
// 		*var = ft_strdup(input);
// 		*value = NULL;
// 	}
// }

char *strip_quotes(const char *arg)
{
    if ((*arg == '"' || *arg == '\'') && arg[ft_strlen(arg) - 1] == *arg)
        return ft_strndup(arg + 1, ft_strlen(arg) - 2); // Supprimer les quotes
    return ft_strdup(arg);
}

int split_env_v(char *arg, char **var, char **value)
{
    if (!arg || !var || !value)
        return 0;

    char *equal_sign = strchr(arg, '=');

    if (!equal_sign)
    {
        *var = strdup(arg);
        *value = NULL;
        return *var != NULL; // Échec si allocation échoue
    }
    *var = strndup(arg, equal_sign - arg);
    if (!*var)
        return (perror("strndup failed"), 0);

    *value = strdup(equal_sign + 1);
    if (!*value)
    {
        free(*var);
        return (perror("strdup failed"), 0);
    }

    return 1; // Succès
}


int count_args(char **args)
{
    int count = 0;
    while (args[count])
        count++;
    return count;
}

void free_args(char **args)
{
    if (!args)
        return;
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
}

int exec_builtin_cmd(char **args, t_var *myEnv, t_ctx *ctx)
{
    (void)myEnv;

    if (!args || !args[0])
        return 0;
    if (ft_strcmp(args[0], "exit") == 0)
    {
        if(process_exit_arg(args, ctx))
            return 1;
    }
    if (ft_strcmp(args[0], "echo") == 0)
    {
        t_token *token_list = create_token_list(args + 1);
        if (token_list)
        {
            handle_echo(token_list, ctx);
            free_tokens(token_list);
        }
        ctx->exit_status = 0;
        return 1;
    }
    printf("testttoooooooooooooooo");
    if (ft_strcmp(args[0], "export") == 0)
    {
        printf("testttxxxxxxxxxxxxxx");
        if (!args[1])
        {
            t_env_var *current = ctx->env_vars;
            while (current)
            {
                printf("declare -x %s=\"%s\"\n", current->name, current->value ? current->value : "");
                current = current->next;
            }
            ctx->exit_status = 0;
            return 1;
        }

        for (int i = 1; args[i]; i++)
        {
            char *var = NULL;
            char *value = NULL;

            if (!split_env_v(args[i], &var, &value))
            {
                fprintf(stderr, "export: `%s`: not a valid identifier\n", args[i]);
                ctx->exit_status = 1;
                free(var);
                free(value);
                continue;
            }

            if (!var || !*var) // Vérifiez si `var` est valide
            {
                fprintf(stderr, "export: Invalid variable name\n");
                ctx->exit_status = 1;
                free(var);
                free(value);
                continue;
            }
            printf("testtttttttttttttttt");
            if (ft_export(ctx, var, value) == -1) // Appel sécurisé
                ctx->exit_status = 1;

            free(var);
            free(value);
        }
        return 1;
    }
    if (ft_strcmp(args[0], "unset") == 0)
    {
        if (!args[1]) // Si aucun argument n'est passé
        {
            ctx->exit_status = 0;
            return 1;
        }
        for (int i = 1; args[i]; i++)
        {
            printf("Trying to unset: %s\n", args[i]);

            if (!is_valid_id(args[i]))
            {
                fprintf(stderr, "unset: %s: not a valid identifier\n", args[i]);
                ctx->exit_status = 1;
                continue;
            }
            if (ctx->env_vars) // Vérifier si env_vars n'est pas NULL
            {
                if (unset_v(&(ctx->env_vars), args[i]) == -1)
                {
                    fprintf(stderr, "unset: Failed to unset %s\n", args[i]);
                    ctx->exit_status = 1;
                }
            }
        }

        ctx->exit_status = 0;
        return 1;
    }
	if (ft_strcmp(args[0], "pwd") == 0)
	{
		if (args[1]) // Check for extra arguments
			;
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			printf("%s\n", cwd);
			ctx->exit_status = 0; // Success
		}
		else
		{
			perror("pwd");
			ctx->exit_status = 1; // Failure
		}
		return 1;
	}
    if (ft_strcmp(args[0], "cd") == 0)
    {
        // if (ft_cd(args, ctx) == 0)
        //     ctx->exit_status = 0;
        // else
        //     ctx->exit_status = 1;
        // return 1;
		char **expanded_args = malloc((sizeof(char *) * (count_args(args) + 1)));
		if (!expanded_args)
		{
			perror("malloc failed for expanded_args");
			ctx->exit_status = 1;
			return 1;
		}
		// Expansion des arguments
		for (int i = 0; args[i]; i++)
			expanded_args[i] = expand_variables(args[i], ctx, STRING); // Expansion normale
		expanded_args[count_args(args)] = NULL;
		// Exécute la commande cd
		if (ft_cd(expanded_args, ctx) == 0)
			ctx->exit_status = 0;
		else
			ctx->exit_status = 1;
		free_args(expanded_args); // Libérer les arguments expandis
		return 1;
	}
    return 0; // Not a built-in command
}

int read_and_exec(t_var *myEnv)
{
    char *line;
    t_ctx ctx;
    ctx.exit_status = 0;

    line = readline(PROMPT);
    if (line == NULL)
    {
        write(1, "exit\n", 5);
        free_environment(myEnv->env);
        free(line);
        exit(0);
    }    // Vérification directe de la commande exit
    if (ft_strncmp(line, "exit", 4) == 0 && (line[4] == '\0' || line[4] == ' '))
    {
        char **args = ft_split(line, ' ');
        process_exit_arg(args, &ctx);  // Cette fonction va gérer l'exit
        free(args);
    }
    if (*line)
        handle_line(line, myEnv, &ctx);
    free(line);
    return ctx.exit_status;
}


// void	read_and_exec(char **env)
// {
// 	char	*line;
// 	int i;

// 	i = 0;
// 	line = readline(PROMPT);
// 	if (line == NULL)
// 	{
// 		write(1, "exit\n", 5);
// 		exit(0);
// 	}
// 	if(ft_strncmp(line, "exit", 4) == 0 && (line [4] == '\0' || line[4] == ' '))
// 	{
// 		write(1, "exit\n", 5);
// 		if(line[4] != '\0' && line[5] != '\0')
// 		{
// 			char *arg = &line[5];
// 			while(arg[i] && ft_isdigit(arg[i]))
// 				i++;
// 			if(arg[i] != '\0')
// 			{
// 				fprintf(stderr, "Exit : %s : numeric argument required\n", arg);
// 				exit(255);
// 			}
// 			exit(ft_atoi(arg));
// 		}
// 		exit(0);
// 	}
// 	if (*line)
// 		handle_line(line, env);
// 	free(line);
// }

int	loop(t_var *myEnv)
{
	// int exit_status = 0;
	while (1)
		read_and_exec(myEnv);
	return (0);
}
