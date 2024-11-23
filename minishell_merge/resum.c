
int main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
         char **envp)
{
    t_var myEnv;
    t_ctx *ctx;

    // Initialisation des signaux
    init_sig();

    // Initialisation du contexte global
    ctx = initialize_ctx();
    if (!ctx)
        return (fprintf(stderr, "Failed to initialize context\n"), 1);

    // Initialiser l'environnement dans t_var
    myEnv.env = get_environment(envp);
    if (!myEnv.env)
    {
        free(ctx); // Libérer le contexte en cas d'erreur
        return (fprintf(stderr, "Failed to copy environment\n"), 1);
    }

    // Construire les variables d'environnement dans t_ctx
    ctx->env_vars = build_env_vars(envp);
    if (!ctx->env_vars)
    {
        free_environment(myEnv.env); // Libérer t_var
        free(ctx);                   // Libérer le contexte
        return (fprintf(stderr, "Failed to initialize environment variables\n"), 1);
    }

    // Lancer la boucle principale
    loop(&myEnv);

    // Libérer les ressources
    free_environment(myEnv.env);
    free_ctx(ctx);

    return (ctx->exit_status);
}

t_ctx *initialize_ctx(void)
{
    t_ctx *ctx = malloc(sizeof(t_ctx));
    if (!ctx)
    {
        perror("Failed to allocate memory for t_ctx");
        return NULL;
    }

    ctx->env_vars = NULL; // Important : initialiser à NULL
    ctx->exit_status = 0;
    ctx->num_pipes = 0;
    ctx->oldpwd = NULL;
    ctx->pwd = NULL;

    return ctx;
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
		env_copy[i] = strdup(envp[i]);
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

        // Trouver le séparateur '='
        char *sep = strchr(envp[i], '=');
        if (sep)
        {
            new_var->name = strndup(envp[i], sep - envp[i]); // Partie avant '='
            new_var->value = strdup(sep + 1);               // Partie après '='
        }
        else
        {
            new_var->name = strdup(envp[i]);
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

        // Ajouter à la liste chaînée
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


int exec(t_token *cmd_tokens, t_var *env, t_ctx *ctx)
{
    char **option_cmd;
    char *path;
    struct stat path_stat;
    char **env_array;

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
            // Case: $EMPTY echo hi
            free(expanded);
            // Shift all arguments left
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

    env_array = convert_env_to_array(env->env_vars);
    if (!env_array)
    {
        free_tab(option_cmd);
        exit(1);
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
                free_tab(env_array);
                exit(126);
            }
            else
            {
                fprintf(stderr, "%s: command not found\n", option_cmd[0]);
                free_tab(option_cmd);
                free_tab(env_array);
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
            free_tab(env_array);
            exit(127);
        }
        if (access(option_cmd[0], X_OK) != 0)
        {
            fprintf(stderr, "%s: Permission denied\n", option_cmd[0]);
            free_tab(option_cmd);
            free_tab(env_array);
            exit(126);
        }
        path = ft_strdup(option_cmd[0]);
    }
    else
    {
        // Try to find in PATH
        path = get_path(option_cmd[0], env->env_vars);
        if (!path)
        {
            fprintf(stderr, "%s: command not found\n", option_cmd[0]);
            free_tab(option_cmd);
            free_tab(env_array);
            exit(127);
        }
    }

    if (execve(path, option_cmd, env_array) == -1)
    {
        free(path);
        free_tab(option_cmd);
        free_tab(env_array);
        exit(127);
    }

    free(path);
    free_tab(option_cmd);
    free_tab(env_array);
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