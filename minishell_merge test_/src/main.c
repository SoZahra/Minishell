/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 11:14:49 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 12:15:23 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		g_var_global = 0;

t_ctx *initialize_ctx(void)
{
    t_ctx *ctx = malloc(sizeof(t_ctx));
    if (!ctx)
    {
        perror("Failed to allocate memory for t_ctx");
        return NULL;
    }
    ctx->env_vars = NULL;
    ctx->exit_status = 0;
    ctx->num_pipes = 0;
    ctx->oldpwd = NULL;
    ctx->pwd = NULL;
    return (ctx);
}

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	rl_clear_history();
	g_var_global = 1;
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	init_sig(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
         char **envp)
{
    t_var myEnv;
    t_ctx *ctx;

    init_sig();
    ctx = initialize_ctx();
    if (!ctx)
        return (fprintf(stderr, "Failed to initialize context\n"), 1);
    myEnv.env = get_environment(envp);
    if (!myEnv.env)
    {
        free(ctx); // Libérer le contexte en cas d'erreur
        return (fprintf(stderr, "Failed to copy environment\n"), 1);
    }
    ctx->env_vars = build_env_vars(envp);
    if (!ctx->env_vars)
    {
        free_environment(myEnv.env); // Libérer t_var
        free(ctx);                   // Libérer le contexte
        return (fprintf(stderr, "Failed to initialize environment variables\n"), 1);
    }
    loop(&myEnv);
    free_environment(myEnv.env);
    free_ctx(ctx);

    return (ctx->exit_status);
}


char	**get_environment(char **envp)
{
	int		i;
	int		count;
	char	**env_copy;

	i = 0;
	count = 0;
	while (envp[count])
		count++;
	env_copy = (char **)malloc((count + 1) * sizeof(char *));
	if (!env_copy)
		return (perror("malloc failed"), NULL);
	while (i < count)
	{
		env_copy[i] = ft_strdup(envp[i]);
		if (!env_copy[i])
		{
			perror("strdup failed");
			while (--i >= 0)
				free(env_copy[i]);
			return (free(env_copy), NULL);
		}
		i++;
	}
	return (env_copy[count] = NULL, env_copy);
}

t_env_var *build_env_vars(char **envp)
{
    t_env_var *head = NULL;
    t_env_var *current = NULL;

    for (int i = 0; envp[i]; i++)
    {
        // Allouer un nouvel élément
        t_env_var *new_var = malloc(sizeof(t_env_var));
        if (!new_var)
        {
            perror("Failed to allocate memory for environment variable");
            free_env_vars(head); // Libérer les variables déjà créées
            return NULL;
        }
        char *sep = ft_strchr(envp[i], '=');// Trouver le séparateur '='
        if (sep)
        {
            new_var->name = ft_strndup(envp[i], sep - envp[i]); // Partie avant '='
            new_var->value = ft_strdup(sep + 1);// Partie après '='
        }
        else
        {
            new_var->name = ft_strdup(envp[i]);
            new_var->value = NULL;
        }
        if (!new_var->name || (sep && !new_var->value))
        {
            perror("Failed to allocate memory for environment variable fields");
            free(new_var->name);
            free(new_var->value);
            free(new_var);
            free_env_vars(head); // Libérer les variables déjà créées
            return NULL;
        }
        new_var->next = NULL;
        if (!head)
            head = new_var;
        else
            current->next = new_var;
        current = new_var;
    }

    return head;
}

int	loop(t_var *myEnv)
{
	// int exit_status = 0;
	while (1)
		read_and_exec(myEnv);
	return (0);
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
    }
    // Vérification directe de la commande exit
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

int handle_line(char *line, t_var *myEnv, t_ctx *ctx)
{
    t_token *tokens;

    add_history(line);
    tokens = parse_command_line(line);
    if (tokens)
    {
        if (check_consecutive_pipes(tokens) == -1)
        {
            free_tokens(tokens);
            return 0;
        }
        // print_tokens(tokens);
        if (contains_pipe(tokens))
            process_pline(tokens, myEnv, ctx);
        else
            exec_simple_cmd(tokens, myEnv, ctx); // Passer le contexte ici
        free_tokens(tokens);
        init_sig();
    }
    return 0;
}


t_token *parse_command_line(char *line, t_ctx *ctx)
{
	(void)ctx;
    t_token *token_list = NULL;
    char buffer[1024];
    int i = 0;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

    while (*line)
    {
        // Handle single quotes
        if (*line == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            line++;
            continue;
        }
        // Handle double quotes
        if (*line == '"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            line++;
            continue;
        }
        // Handle whitespace
        if (is_whitespace(*line) && !in_single_quotes && !in_double_quotes)
        {
            if (i > 0)
            {
                buffer[i] = '\0';
                t_token *new_token = add_token(&token_list, TOKEN_ARGUMENT, buffer);
                new_token->type = in_single_quotes ? SINGLE_QUOTE : (in_double_quotes ? DOUBLEQUOTE : STRING);
                i = 0;
            }
            line++;
            continue;
        }
        // Normal characters
        buffer[i++] = *line++;
    }
    // Add last token
    if (i > 0)
    {
        buffer[i] = '\0';
        t_token *new_token = add_token(&token_list, TOKEN_ARGUMENT, buffer);
        new_token->type = in_single_quotes ? SINGLE_QUOTE : (in_double_quotes ? DOUBLEQUOTE : STRING);
    }
    return token_list;
}

t_token *add_token(t_token **token_list, t_token_type type, const char *value)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
    {
        perror("malloc failed");
        return NULL;
    }
    new_token->value = ft_strdup(value); // Duplicate the string
    if (!new_token->value)
    {
        free(new_token);
        perror("ft_strdup failed");
        return NULL;
    }
    new_token->type = type;
    new_token->next = NULL;

    if (!*token_list)
    {
        *token_list = new_token; // Set the head if the list is empty
    }
    else
    {
        t_token *current = *token_list;
        while (current->next)
            current = current->next;
        current->next = new_token; // Append to the end of the list
    }
    return new_token; // Return the newly created token
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		perror("malloc failed");
		return (NULL);
	}
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	return (new_token);
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


int process_pline(t_token *tokens, t_var *myEnv, t_ctx *ctx)
{
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t pid;
    int status;
    t_token *cmd_start = tokens;
    int redirect = 0;
    int redirect_output = 0;
    int redirect_input = 0;

    while (cmd_start != NULL)
    {
        t_token *cmd_end = cmd_start;
        while (cmd_end != NULL && cmd_end->type != TOKEN_PIPE)
            cmd_end = cmd_end->next;

        if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE)
        {
            if (pipe(pipe_fd) == -1)
                exit_error();
        }
        pid = fork();
        if (pid == -1)
            exit_error();
        if (pid == 0)
        {
            t_token *exec_tokens = NULL;
            t_token **exec_tokens_tail = &exec_tokens;
            t_token *redir_token = cmd_start;

            while (redir_token != cmd_end)
            {
                if (redir_token->type == TOKEN_REDIRECT_INPUT)
                {
                    int input_fd = open(redir_token->next->value, O_RDONLY);
                    if (input_fd == -1)
                        exit_error();
                    dup2(input_fd, STDIN_FILENO);
                    close(input_fd);
                    redir_token = redir_token->next;
                    redirect = 1;
                    redirect_input = 1;
                }
                else if (redir_token->type == TOKEN_REDIRECT_OUTPUT)
                {
                    fprintf(stderr, "Output redirection \n");
                    int output_fd = open(redir_token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (output_fd == -1)
                        exit_error();
                    dup2(output_fd, STDOUT_FILENO);
                    close(output_fd);
                    redir_token = redir_token->next;
                    redirect_output = 1;
                    redirect = 1;
                }
                else
                {
                    *exec_tokens_tail = redir_token;
                    exec_tokens_tail = &(*exec_tokens_tail)->next;
                    redirect = 0;
                }
                fprintf(stderr, "Did I leave ? \n");
                redir_token = redir_token->next;
            }
            *exec_tokens_tail = NULL;
            if (prev_fd != -1 && redirect_input != 1)
            {
                fprintf(stderr, "First if \n");
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
                redirect_input = 0;
            }
            if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE && redirect_output != 1)
            {
                fprintf(stderr, "Second if \n");
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE && redirect != 1)
            {
                fprintf(stderr, "Second / half if \n");
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            else if (cmd_end != NULL && cmd_end->type == TOKEN_PIPE && redirect_output == 1)
            {
                fprintf(stderr, "Third if \n");
            	close(pipe_fd[0]);
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
                /* dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]); */
            }
            fprintf(stderr, "Check on tokens value : %s \n\n", exec_tokens->value);
            exec(exec_tokens, myEnv, ctx);
            exit_error();
        }
        wait(0);
        if (prev_fd != -1)
            close(prev_fd);
        if (cmd_end != NULL)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
            cmd_start = cmd_end->next;
        }
        else
            cmd_start = NULL;
    }
    while (wait(&status) > 0);
    return 0;
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


char *expand_variables(const char *str, t_ctx *ctx, t_token_type token_type)
{
    // Pour les single quotes, retourner la chaîne telle quelle sans expansion
    if (token_type == SINGLE_QUOTE)
    {
        return ft_strdup(str);
    }

    char *expanded = ft_strdup("");
    char *temp;

    while (*str)
    {
        if (*str == '$' && token_type != SINGLE_QUOTE) // Ne jamais expandre dans les single quotes
        {
            str++; // Skip the '$'
            if (*str == '?') // Special case for `$?`
            {
                char *exit_code = ft_itoa(ctx->exit_status);
                temp = ft_strjoin(expanded, exit_code);
                free(expanded);
                free(exit_code);
                expanded = temp;
                str++;
            }
            else if (ft_isalnum(*str) || *str == '_') // Valid variable name
            {
                char buffer[1024];
                int i = 0;
                while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                {
                    buffer[i] = str[i];
                    i++;
                }
                buffer[i] = '\0';
                str += i;

                char *value = find_env_value(buffer, ctx->env_vars);
                temp = ft_strjoin(expanded, value ? value : "");
                free(expanded);
                expanded = temp;
            }
            else // Invalid variable, keep the $
            {
                temp = ft_strjoin(expanded, "$");
                free(expanded);
                expanded = temp;
                if (*str && *str != ' ') // Si ce n'est pas un espace
                {
                    char curr[2] = {*str, '\0'};
                    temp = ft_strjoin(expanded, curr);
                    free(expanded);
                    expanded = temp;
                    str++;
                }
            }
        }
        else
        {
            char curr[2] = {*str, '\0'};
            temp = ft_strjoin(expanded, curr);
            free(expanded);
            expanded = temp;
            str++;
        }
    }
    return expanded;
}

char *get_path(char *cmd, t_env_var *env)
{
    char *path_env = get_env_value("PATH", env); // Obtenir la valeur de PATH
    char *path = NULL;

    if (!path_env)
        return NULL; // Si PATH n'existe pas
    path = search_in_path(cmd, path_env); // Rechercher la commande dans PATH
    return path;
}


char *get_env_value(const char *name, t_env_var *env)
{
    t_env_var *current = env;

    while (current)
    {
        if (ft_strcmp(current->name, name) == 0)
            return current->value;
        current = current->next;
    }
    return NULL; // Si la variable n'est pas trouvée
}


char *search_in_path(const char *cmd, const char *path_env)
{
    char *path_copy, *path_token, *full_path;
    struct stat sb;

    // Si la commande contient un '/', c'est un chemin absolu ou relatif
    if (ft_strchr(cmd, '/'))
    {
        if (stat(cmd, &sb) == 0 && (sb.st_mode & S_IXUSR))
            return ft_strdup(cmd); // Commande trouvée
        return NULL; // Commande non trouvée ou non exécutable
    }

    // Dupliquer PATH pour le tokeniser
    path_copy = ft_strdup(path_env);
    if (!path_copy)
        return NULL;

    // Parcourir les chemins dans PATH
    path_token = strtok(path_copy, ":");
    while (path_token)
    {
        full_path = join_path_cmd(path_token, cmd);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }

        if (stat(full_path, &sb) == 0 && (sb.st_mode & S_IXUSR))
        {
            free(path_copy); // Libérer la copie de PATH
            return full_path; // Commande trouvée
        }

        free(full_path);
        path_token = strtok(NULL, ":");
    }

    free(path_copy); // Libérer la copie de PATH
    return NULL; // Commande non trouvée
}


char	*join_path_cmd(const char *path, const char *cmd)
{
	char	*full_path;
	char	*path_with_slash;

	path_with_slash = ft_strjoin(path, "/");
    if (!path_with_slash)
		return (NULL);
	full_path = ft_strjoin(path_with_slash, cmd);
	free(path_with_slash);
	return (full_path);
}



int count_args(char **args)
{
    int count = 0;
    while (args[count])
        count++;
    return count;
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
    if (ft_strcmp(args[0], "export") == 0)
    {
        if (!args[1])
        {
            t_env_var *current = ctx->env_vars;
            if (!ctx->env_vars)
            {
                fprintf(stderr, "export: Environment variables not initialized\n");
                ctx->exit_status = 1;
                return 1;
            }
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
            if (!is_valid_id(var))
            {
                fprintf(stderr, "export: `%s`: not a valid identifier\n", var);
                free(var);
                free(value);
                ctx->exit_status = 1;
                continue;
            }

            if (ft_export(ctx, var, value) == -1)
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


int process_exit_arg(char **args, t_ctx *ctx)
{
    long exit_code;

    write(1, "exit\n", 5);

    if (!args[1])
        exit(ctx->exit_status);

    if (args[2])
    {
        fprintf(stderr, "exit: too many arguments\n");
        ctx->exit_status = 1;
        return 1;  // Ne pas exit, retourner au shell
    }

    // Extraire le signe si présent
    const char *str = args[1];
    int sign = 1;
    if (*str == '+')
        str++;
    else if (*str == '-')
    {
        sign = -1;
        str++;
    }

    // Retirer les quotes si présentes
    char *cleaned = strip_quotes(str);
    if (!cleaned)
    {
        fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
        exit(2);
    }

    // Vérifier que c'est un nombre valide
    for (const char *p = cleaned; *p; p++)
    {
        if (!ft_isdigit(*p))
        {
            free(cleaned);
            fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
            exit(2);
        }
    }

    exit_code = sign * ft_atoi(cleaned);
    free(cleaned);

    // Calculer le code de sortie modulo 256
    if (exit_code < 0)
        exit_code = 256 - ((-exit_code) % 256);
    else
        exit_code = exit_code % 256;

    exit(exit_code);
}


char *strip_quotes(const char *arg)
{
    if ((*arg == '"' || *arg == '\'') && arg[ft_strlen(arg) - 1] == *arg)
        return ft_strndup(arg + 1, ft_strlen(arg) - 2); // Supprimer les quotes
    return ft_strdup(arg);
}


t_token *create_token_list(char **args)
{
    t_token *head = NULL;
    t_token *current = NULL;

    for (int i = 0; args[i]; i++)
    {
        t_token *new_token = malloc(sizeof(t_token));
        if (!new_token)
        {
            perror("malloc failed");
            free_tokens(head);
            return NULL;
        }

        new_token->value = ft_strdup(args[i]);
        if (!new_token->value)
        {
            free(new_token);
            free_tokens(head);
            return NULL;
        }

        // Détermine le type de token en fonction des quotes
        if (args[i][0] == '\'' && args[i][ft_strlen(args[i]) - 1] == '\'')
        {
            new_token->type = SINGLE_QUOTE;
            // Enlever les quotes du début et de la fin
            char *unquoted = ft_strndup(args[i] + 1, ft_strlen(args[i]) - 2);
            free(new_token->value);
            new_token->value = unquoted;
        }
        else if (args[i][0] == '"' && args[i][ft_strlen(args[i]) - 1] == '"')
        {
            new_token->type = DOUBLEQUOTE;
            // Enlever les quotes du début et de la fin
            char *unquoted = ft_strndup(args[i] + 1, ft_strlen(args[i]) - 2);
            free(new_token->value);
            new_token->value = unquoted;
        }
        else
        {
            new_token->type = STRING;
        }

        new_token->next = NULL;

        if (!head)
            head = new_token;
        else
            current->next = new_token;
        current = new_token;
    }

    return head;
}


void handle_echo(t_token *token_list, t_ctx *ctx)
{
    int n_option = 0;

    // Vérifie si le premier argument est "-n"
    if (token_list && ft_strcmp(token_list->value, "-n") == 0)
    {
        n_option = 1;
        token_list = token_list->next;
    }
    write_echo_content(token_list, n_option, ctx);
	ctx->exit_status = 0;
}


int split_env_v(char *arg, char **var, char **value)
{
    if (!arg || !var || !value)
        return 0;

    char *equal_sign = ft_strchr(arg, '=');

    if (!equal_sign)
    {
        *var = ft_strdup(arg);
        *value = NULL;
        return *var != NULL; // Échec si allocation échoue
    }
    *var = ft_strndup(arg, equal_sign - arg);
    if (!*var)
        return (perror("strndup failed"), 0);

    *value = ft_strdup(equal_sign + 1);
    if (!*value)
    {
        free(*var);
        return (perror("strdup failed"), 0);
    }

    return 1; // Succès
}

int is_valid_id(const char *var)
{
    int i = 0;

    if (!var || !*var)
        return 0;
    if (!ft_isalpha(var[0]) && var[0] != '_') // Le premier caractère doit être alphabétique ou '_'
        return 0;
    while (var[i] != '\0')
    {
        if (!ft_isalnum(var[i]) && var[i] != '_') // Les caractères restants doivent être alphanumériques ou '_'
            return 0;
        i++;
    }
    return 1;
}


int ft_export(t_ctx *ctx, char *key, char *value)
{
    if (!ctx || !key) // Vérifiez les pointeurs
        return -1;

    t_env_var *current = ctx->env_vars;

    // Recherche si la variable existe déjà
    while (current)
    {
        if (!current->name) // Vérifie que le nom est valide
        {
            fprintf(stderr, "Error: current->name is NULL\n");
            return -1;
        }
        if (strcmp(current->name, key) == 0)
        {
            free(current->value); // Libérer l'ancienne valeur
            current->value = value ? ft_strdup(value) : NULL; // Mettre à jour
            if (value && !current->value)
                return (perror("strdup failed"), -1);
            return 0; // Succès
        }
        current = current->next;
    }

    // Ajoutez une nouvelle variable si elle n'existe pas
    t_env_var *new_var = (t_env_var *)malloc(sizeof(t_env_var));
    if (!new_var)
        return (perror("malloc failed"), -1);

    new_var->name = ft_strdup(key);
    if (!new_var->name)
    {
        free(new_var);
        return (perror("strdup failed"), -1);
    }

    new_var->value = value ? strdup(value) : NULL;
    if (value && !new_var->value)
    {
        free(new_var->name);
        free(new_var);
        return (perror("strdup failed"), -1);
    }

    new_var->next = ctx->env_vars;
    ctx->env_vars = new_var;

    return 0; // Succès
}


int unset_v(t_env_var **env_vars, const char *var)
{
    t_env_var *current;
    t_env_var *prev = NULL;

    if (!env_vars || !*env_vars || !var) // Vérification des pointeurs
        return -1;

    current = *env_vars;

    while (current)
    {
        // Vérifie si le nom de la variable correspond
        if (ft_strcmp(current->name, var) == 0)
        {
            if (prev)
                prev->next = current->next; // Relier le précédent au suivant
            else
                *env_vars = current->next;  // Supprimer le premier élément

            // Libérer les ressources associées
            if (current->name)
                free(current->name);
            if (current->value)
                free(current->value);
            free(current);
            return 0; // Succès
        }

        prev = current;
        current = current->next;
    }

    return 0; // Variable non trouvée
}


int ft_cd(char **args, t_ctx *ctx)
{
    char *path;

    // Vérifier si trop d'arguments sont passés
    if (args[1] && args[2])
	{
        fprintf(stderr, "cd: too many arguments\n");
        ctx->exit_status = 1;
        return 1;
    }
    if (!args[1] || ft_strcmp(args[1], "~") == 0) {
        return ft_cd_home(ctx);
    } else if (ft_strcmp(args[1], "-") == 0) {
        return ft_cd_oldpwd(ctx);
    }

    // Expansion de la variable si nécessaire
	path = expand_variables(args[1], ctx, TOKEN_ARGUMENT);
	if (!path || ft_strlen(path) == 0) // Si l'expansion échoue ou retourne une chaîne vide
	{
		fprintf(stderr, "cd: %s: No such file or directory\n", args[1]);
		free(path);
		ctx->exit_status = 1;
		return 1;
	}
	// Vérification de chdir
	if (chdir(path) != 0)
	{
		perror("cd");
		free(path);
		ctx->exit_status = 1;
		return 1;
	}
	free(path);
	return ft_update_pwd(ctx);
}

int ft_cd_home(t_ctx *ctx)
{
    char *home = find_env_value("HOME", ctx->env_vars);
    if (!home) {
        fprintf(stderr, "cd: HOME not set\n");
        return 1;
    }
    if (chdir(home) != 0) {
        perror("cd");
        return 1;
    }
    return ft_update_pwd(ctx);
}

int ft_update_pwd(t_ctx *ctx)
{
    char *cwd = getcwd(NULL, 0); // Obtenir le répertoire actuel
    if (!cwd) {
        perror("getcwd");
        return 1;
    }
    if (ctx->oldpwd)
        free(ctx->oldpwd);   // Libérer l'ancien `OLDPWD`
    ctx->oldpwd = ctx->pwd;  // Mettre à jour `OLDPWD`
    ctx->pwd = cwd;          // Attribuer le nouveau chemin à `PWD`
    return 0;
}


int	ft_cd_oldpwd(t_ctx *ctx)
{
	if (!(ctx->oldpwd == NULL))
	{
		fprintf(stderr, "cd: OLDPWD not set\n");
		return (0);
	}
	if (chdir(ctx->oldpwd) != 0)
	{
		perror("cd");
		return (0);
	}
	printf("%s\n", ctx->oldpwd);
	return(0);
}


char *find_env_value(const char *name, t_env_var *env_vars)
{
    while (env_vars)
    {
        if (ft_strcmp(env_vars->name, name) == 0)
            return env_vars->value;
        env_vars = env_vars->next;
    }
    return NULL; // Retourne NULL si aucune correspondance n'est trouvée
}


void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

void	*free_tab_2(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

void free_env_vars(t_env_var *head)
{
    t_env_var *current = head;
    t_env_var *tmp;

    while (current)
    {
        tmp = current;
        current = current->next;

        free(tmp->name);
        free(tmp->value);
        free(tmp);
    }
}

void free_environment(char **env_copy)
{
    int i = 0;

    if (!env_copy)
        return;

    while (env_copy[i])
    {
        free(env_copy[i]);
        i++;
    }
    free(env_copy);
}

void free_ctx(t_ctx *ctx)
{
    if (!ctx)
        return;

    // Libérer les variables d'environnement
    free_env_vars(ctx->env_vars);

    // Libérer oldpwd et pwd
    if (ctx->oldpwd)
        free(ctx->oldpwd);
    if (ctx->pwd)
        free(ctx->pwd);

    free(ctx);
}

void	*free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	return (NULL);
}