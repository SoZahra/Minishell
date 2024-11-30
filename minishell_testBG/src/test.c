/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:49:42 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/30 17:42:16 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ctx *get_ctx(void)
{
    static t_ctx ctx;
    return (&ctx);
}

int main(int argc __attribute__((unused)), char **argv __attribute__((unused)), char **envp)
{
    if (initialize_ctx(get_ctx()))
        return 1; // Initialisez `t_ctx`

    if (!get_ctx())
        return (perror("Failed to initialize context"), 1);

    get_ctx()->env_vars = build_env_list(envp);
    if(!get_ctx()->env_vars)
	{
		free_ctx(get_ctx());
		return(perror("Failed to build env list"), 1);
	}
	loop_with_pipes(envp, get_ctx());
    free_ctx(get_ctx());
    return 0;
}

int initialize_ctx(t_ctx *ctx)
{
    if (!ctx)
    {
        perror("Failed to allocate memory for t_ctx");
        return 1;
    }

    // Initialiser les champs de la structure
    ctx->env_vars = NULL;           // Liste des variables d'environnement initialement vide
    ctx->exit_status = 0;           // Le code de sortie est initialisé à 0 (succès)
    ctx->num_pipes = 0;             // Initialiser le nombre de pipes à 0
    ctx->oldpwd = NULL;             // Initialiser l'ancien répertoire à NULL
    ctx->pwd = getcwd(NULL, 0);     // Obtenir le répertoire actuel
    if (!ctx->pwd)
    {
        perror("Failed to get current working directory");
        free(ctx);                  // Libérer la mémoire si une erreur survient
        return 1;
    }

    return 0;
}


t_env_var *build_env_list(char **envp)
{
    t_env_var *head = NULL;
    t_env_var *current = NULL;

    for (int i = 0; envp[i]; i++)
    {
        char *sep = strchr(envp[i], '=');
        if (!sep)
            continue;

        t_env_var *new_var = malloc(sizeof(t_env_var));
        if (!new_var)
        {
            perror("malloc failed");
            free_env(head);
            return NULL;
        }
        // Copier le nom et la valeur
        new_var->name = ft_strndup(envp[i], sep - envp[i]);
        new_var->value = ft_strdup(sep + 1);
        new_var->next = NULL;

        if (!new_var->name || !new_var->value)
        {
            free(new_var->name);
            free(new_var->value);
            free(new_var);
            free_env(head);
            return (perror("malloc failed"), NULL);
        }
        if (!head)
            head = new_var;
        else
            current->next = new_var;

        current = new_var;
    }
    return head;
}

char **convert_env_to_array(t_ctx *ctx)
{
    int count = 0;
    t_env_var *current = ctx->env_vars;
    char **env_array;

    while (current)
    {
        count++;
        current = current->next;
    }

    env_array = malloc((count + 1) * sizeof(char *));
    if (!env_array)
        return NULL;

    current = ctx->env_vars;
    for (int i = 0; i < count && current; i++)
    {
        int len = ft_strlen(current->name) + (current->value ? ft_strlen(current->value) : 0) + 2;
        env_array[i] = malloc(len);
        if (!env_array[i])
        {
            free_tab(env_array);
            return NULL;
        }
        if (current->value)
            snprintf(env_array[i], len, "%s=%s", current->name, current->value);
        else
            snprintf(env_array[i], len, "%s=", current->name);
        current = current->next;
    }
    env_array[count] = NULL;
    return env_array;
}

char *reconstruct_line(t_token *token_list)
{
    char *reconstructed = ft_strdup("");

    while (token_list)
    {
        char *token_value = token_list->value;
        if (token_list->type == SINGLE_QUOTE || token_list->type == DOUBLEQUOTE)
        {
            // Supprimer les quotes si nécessaire
            token_value = strip_quotes(token_value);
        }
        char *temp = ft_strjoin(reconstructed, token_value);
        free(reconstructed);
        reconstructed = temp;

        token_list = token_list->next;
    }

    return reconstructed;
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


char *find_env_value(const char *name, t_env_var *env_vars)
{
    while (env_vars)
    {
        if (strcmp(env_vars->name, name) == 0)
            return env_vars->value;
        env_vars = env_vars->next;
    }
    return NULL; // Retourne NULL si aucune correspondance n'est trouvée
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int loop_with_pipes(char **env, t_ctx *ctx) {
    char *line;
    t_token *tokens;
    // int status = 0;
    (void)env;

    while (1)
    {
        line = readline(PROMPT);
        if (line == NULL)
        {
            write(1, "exit\n", 5);
            exit(ctx->exit_status);
        }
        if (*line)
        {
            add_history(line);
            tokens = parse_command_line(line, ctx);
            if (tokens)
            {
                char **cmd_env = convert_env_to_array(ctx);
                exec_simple_cmd(tokens, cmd_env, ctx);
                free_tab(cmd_env);
                free_tokens(tokens);
                // exit(ctx->exit_status);
                // free_tokens(tokens);
            }
        }
        free(line);
    }
    return ctx->exit_status;
}

int is_builtin(char *cmd)
{
    char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
    int i = 0;

    while (builtins[i])
    {
        if (strcmp(cmd, builtins[i]) == 0)
            return 1;
        i++;
    }
    return 0;
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

void write_echo_content(t_token *token_list, int n_option, t_ctx *ctx)
{
    t_token *current = token_list;
    int first_arg = 1;
    int prev_was_quote = 0;
	(void)ctx;

    while (current)
    {
        if (!first_arg && !(prev_was_quote))
            write(STDOUT_FILENO, " ", 1);

        if (current->value)
        {
            if (current->type == SINGLE_QUOTE)
            {
                // Ne pas expandre les variables entre single quotes
                write(STDOUT_FILENO, current->value, ft_strlen(current->value));
            }
            else if (current->type == DOUBLEQUOTE)
            {
                // Les variables ont déjà été expandées dans parse_command_line
                write(STDOUT_FILENO, current->value, ft_strlen(current->value));
            }
            else
            {
                // Les variables ont déjà été expandées dans parse_command_line
                write(STDOUT_FILENO, current->value, ft_strlen(current->value));
            }
        }

        first_arg = 0;
        prev_was_quote = (current->type == SINGLE_QUOTE || current->type == DOUBLEQUOTE);
        current = current->next;
    }

    if (!n_option)
        write(STDOUT_FILENO, "\n", 1);
}

int split_env_v(char *arg, char **var, char **value)
{
    char *equal_sign = strchr(arg, '=');

    if (!equal_sign)
    {
        *var = strdup(arg);
        *value = NULL;
        return *var != NULL;
    }

    // Extraire le nom de la variable avant le '='
    *var = strndup(arg, equal_sign - arg);
    *value = strdup(equal_sign + 1); // On garde les quotes dans la valeur

    if (!*var || (equal_sign && !*value))
    {
        free(*var);
        free(*value);
        return 0;
    }

    return 1;
}

int is_valid_id(const char *var)
{
    char *equal_sign = strchr(var, '=');
    size_t len = equal_sign ? (size_t)(equal_sign - var) : strlen(var);

    if (!var || !*var)
        return 0;

    // Vérifier seulement la partie avant le '=' si présent
    if (!ft_isalpha(var[0]) && var[0] != '_')
        return 0;

    for (size_t i = 1; i < len; i++)
    {
        if (!ft_isalnum(var[i]) && var[i] != '_')
            return 0;
    }
    return 1;
}

void print_export(t_ctx *ctx)
{
    t_env_var *current = ctx->env_vars;
    while (current) {
        if (current->value)
            printf("%s=%s\n", current->name, current->value);
        else
            printf("%s\n", current->name);
        current = current->next;
    }
}

t_env_var *get_last_env_node(t_env_var **env)
{
    t_env_var *tmp;
    tmp = *env;
    while (tmp->next)
        tmp = tmp->next;
    return tmp;
}

int count_args(char **args)
{
    int count = 0;
    while (args[count])
        count++;
    return count;
}

int ft_cd(char **args, t_ctx *ctx)
{
    // Vérifier si trop d'arguments sont passés
    if (args[1] && args[2])
	{
        fprintf(stderr, "cd: too many arguments\n");
        ctx->exit_status = 1;
        return 1;
    }
    if (!args[1] || ft_strcmp(args[1], "~") == 0)
        return ft_cd_home(ctx);
    else if (ft_strcmp(args[1], "-") == 0)
        return ft_cd_oldpwd(ctx);
	// Les arguments sont déjà expandés par parse_command_line
    if (chdir(args[1]) != 0)
    {
        fprintf(stderr, "cd: %s: No such file or directory\n", args[1]);
        ctx->exit_status = 1;
        return 1;
    }
    return ft_update_pwd(ctx);
}


int ft_cd_home(t_ctx *ctx) {
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

int ft_cd_oldpwd(t_ctx *ctx)
{
    if (ctx->oldpwd == NULL)  // Condition inversée pour plus de clarté
    {
        fprintf(stderr, "cd: OLDPWD not set\n");
        return 1;
    }
    if (chdir(ctx->oldpwd) != 0)
    {
        perror("cd");
        return 1;
    }
    printf("%s\n", ctx->oldpwd);
    return ft_update_pwd(ctx);  // N'oubliez pas de mettre à jour PWD après le changement
}

int ft_update_pwd(t_ctx *ctx) {
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


int process_exit_arg(char **args, t_ctx *ctx)
{
    char *cleaned_arg;
    long exit_code;

    if (args[1])
	{
        cleaned_arg = strip_quotes(args[1]); // Supprimer les quotes
        if (!is_numeric_argument(cleaned_arg))
		{
            fprintf(stderr, "minishell: exit: %s: numeric argument required\n", cleaned_arg);
            free(cleaned_arg);
            ctx->exit_status = 2;
            return 1; // Indique une sortie immédiate avec code 2
        }
        exit_code = ft_atoi(cleaned_arg);
        free(cleaned_arg);
        if (args[2])
		{
            fprintf(stderr, "minishell: exit: too many arguments\n");
            ctx->exit_status = 1;
            return 0; // Ne pas quitter immédiatement
        }
        ctx->exit_status = (exit_code % 256 + 256) % 256; // Calculer modulo 256
    }
	else
		ctx->exit_status = 0;
    write(1, "exit\n", 5);
    exit(ctx->exit_status);
    return 1; // Pour respecter les conventions de retour (même si unreachable ici)
}


int exec_builtin_cmd(char **args, char **env, t_ctx *ctx)
{
    int i;
    (void)env;

    if (ft_strcmp(args[0], "exit") == 0)
    {
        if (process_exit_arg(args, ctx))
            return 1;
        return 1;
    }

    if (ft_strcmp(args[0], "echo") == 0)
    {
        t_token *token_list = create_token_list(args + 1);
        if (token_list)
        {
            handle_echo(token_list, ctx);
            // free_tokens(token_list);
        }
        ctx->exit_status = 0;
        return 1;
    }
    if (ft_strcmp(args[0], "export") == 0)
    {
        if (!args[1])
        {
            print_export(ctx);
            ctx->exit_status = 0;
            return 1;
        }

        i = 1;
        int has_error = 0;
        while (args[i])
        {
            char *var = NULL;
            char *value = NULL;
            if (!split_env_v(args[i], &var, &value))
            {
                has_error = 1;
                i++;
                continue;
            }
            if (!var || !is_valid_id(var))
            {
                fprintf(stderr, "export: `%s`: not a valid identifier\n", var ? var : args[i]);
                free(var);
                if (value)
                    free(value);
                has_error = 1;
                i++;
                continue;
            }

            t_env_var *new = malloc(sizeof(t_env_var));
            if (!new)
            {
                free(var);
                if (value)
                    free(value);
                has_error = 1;
                i++;
                continue;
            }

            new->name = var;
            new->value = value; // La valeur peut contenir n'importe quoi
            new->next = NULL;
            get_last_env_node(&get_ctx()->env_vars)->next = new;
            i++;
        }
        ctx->exit_status = has_error ? 1 : 0;
        return 1;
    }
    // if (ft_strcmp(args[0], "unset") == 0)
    // {
    //     if (!args[1])
    //     {
    //         ctx->exit_status = 0;
    //         return 1;
    //     }
	// 	for (int i = 1; args[i]; i++)
    //     {
    //         if (!is_valid_id(args[i]))
    //         {
    //             fprintf(stderr, "unset: `%s`: not a valid identifier\n", args[i]);
	// 			ctx->exit_status = 1;
    //             continue;
    //         }
    //         unset_v(&(ctx->env_vars), args[i]);
    //     }
    //     ctx->exit_status = 0;
    //     return 1;
    // }
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
        if (ft_cd(args, ctx) == 0)
            ctx->exit_status = 0;
        else
            ctx->exit_status = 1;
        return 1;
		// char **expanded_args = malloc((sizeof(char *) * (count_args(args) + 1)));
		// if (!expanded_args)
		// {
		// 	perror("malloc failed for expanded_args");
		// 	ctx->exit_status = 1;
		// 	return 1;
		// }
		// // Expansion des arguments
		// for (int i = 0; args[i]; i++)
		// 	expanded_args[i] = expand_variables(args[i], ctx, STRING); // Expansion normale
		// expanded_args[count_args(args)] = NULL;
		// // Exécute la commande cd
		// if (ft_cd(expanded_args, ctx) == 0)
		// 	ctx->exit_status = 0;
		// else
		// 	ctx->exit_status = 1;
		// free_args(expanded_args); // Libérer les arguments expandis
		// return 1;
	}
    // if (ft_strcmp(args[0], "env") == 0)
    // {
    //     print_env(get_ctx());
    //     return 1;
    // }
    return 0; // Not a built-in command
}

int exec_simple_cmd(t_token *tokens, char **env, t_ctx *ctx)
{
    char **args;
    // pid_t pid;
    // int status;
	(void)env;

    args = prepare_args(tokens, ctx);// Préparer les arguments
    if (!args || !args[0])
    {
        perror("Erreur d'allocation de mémoire pour les arguments");
        free_tab(args);
        ctx->exit_status = 1;
        return 0;
    }
    if (is_builtin(args[0]))
    {
        // Convertir env_vars en tableau pour les builtins
        char **env_array = convert_env_to_array(ctx);
        if (!env_array)
        {
            free_tab(args);
            ctx->exit_status = 1;
            return 0;
        }
        int ret = exec_builtin_cmd(args, env_array, ctx);
        free_tab(args);
        free_tab(env_array);
        return ret;
    }
    return 0;
}

int check_invalid_quotes(char *line)
{
    char quote_type = 0;  // Garde le type de quote qu'on cherche à fermer
    int i = 0;

    while (line[i])
    {
        if ((line[i] == '\'' || line[i] == '"') && !quote_type)
            quote_type = line[i];
        else if (line[i] == quote_type)
            quote_type = 0;
        else if ((line[i] == '\'' || line[i] == '"') && !quote_type)
            return 1;
        i++;
    }
    if (quote_type)  // Quote non fermée
        return 1;
    return 0;
}


char **prepare_args(t_token *tokens, t_ctx *ctx)
{
   (void)ctx;
   int count = 0;
   t_token *current = tokens;
   char **args;
   int i = 0;

   while (current)
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
   while (current)
   {
    //    printf("Processing token %d: type = '%c'\n", i, (char)current->type);

       if (current->type == '\'') // Vérifie explicitement la quote simple
       {
        //    printf("Found single quote: using value '%s' without expansion\n", current->value);
           args[i] = ft_strdup(current->value);
       }
       else if (current->type == '"') // Vérifie explicitement la quote double
       {
        //    printf("Found double quote: expanding value '%s'\n", current->value);
           args[i] = expand_variables(current->value, ctx, current->type);
       }
       else // Cas normal (STRING ou autres)
       {
        //    printf("Found normal token: expanding value '%s'\n", current->value);
           args[i] = expand_variables(current->value, ctx, current->type);
       }

       if (!args[i])
       {
           // Gestion d'erreur...
           while (i > 0)
               free(args[--i]);
           free(args);
           return NULL;
       }
       i++;
       current = current->next;
   }
   args[i] = NULL;
   return args;
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
    new_token->content = ft_strdup(value); // Duplicate aussi pour content
    if (!new_token->value || !new_token->content)
    {
        if (new_token->value)
            free(new_token->value);
        if (new_token->content)
            free(new_token->content);
        free(new_token);
        perror("ft_strdup failed");
        return NULL;
    }
    new_token->type = type;
    new_token->next = NULL;
    if (!*token_list)
    {
        *token_list = new_token;
    }
    else
    {
        t_token *current = *token_list;
        while (current->next)
            current = current->next;
        current->next = new_token;
    }
    return new_token;
}

t_token *parse_command_line(char *line, t_ctx *ctx)
{
    if (check_invalid_quotes(line))
    {
        fprintf(stderr, "Error: quote found without matching opening quote\n");
        return NULL;
    }

    t_token *token_list = NULL;
    char buffer[1024] = {0};
    int i = 0;
    int j = 0;
    char current_quote_type = 0;
    // t_token_type current_token_type = STRING;
    char *temp = NULL;
    t_token_type temp_type = STRING;  // Pour garder le type du token en cours

    while (line[i])
    {
        if ((line[i] == '\'' || line[i] == '"') && !current_quote_type)
        {
            current_quote_type = line[i];
            i++; // Skip opening quote

            // Si les quotes sont vides ("" ou ''), on les ignore
            if (line[i] == current_quote_type)
            {
                i++; // Skip closing quote
                current_quote_type = 0;
                continue; // Continue avec le reste du token
            }

            // Définir le type de quote
            temp_type = (current_quote_type == '\'') ? SINGLE_QUOTE : DOUBLEQUOTE;

            // Si on a du contenu avant les quotes, on le garde dans temp
            if (j > 0)
            {
                buffer[j] = '\0';
                if (!temp)
                    temp = ft_strdup(buffer);
                else
                {
                    char *new_temp = ft_strjoin(temp, buffer);
                    free(temp);
                    temp = new_temp;
                }
                j = 0;
            }

            // Capture le contenu entre quotes
            while (line[i] && line[i] != current_quote_type)
                buffer[j++] = line[i++];

            if (!line[i])
            {
                fprintf(stderr, "Error: unclosed quote\n");
                free(temp);
                free_tokens(token_list);
                return NULL;
            }

            buffer[j] = '\0';

            // Ajouter directement le token avec son type
            if (temp)
            {
                char *combined = ft_strjoin(temp, buffer);
                add_token(&token_list, temp_type, combined);
                free(combined);
                free(temp);
                temp = NULL;
            }
            else
            {
                add_token(&token_list, temp_type, buffer);
            }

            j = 0;
            i++; // Skip la quote fermante
            current_quote_type = 0;
        }
        else if (is_whitespace(line[i]))
        {
            if (j > 0)
            {
                buffer[j] = '\0';
                add_token(&token_list, STRING, buffer);
                j = 0;
            }
            i++;
        }
        else
        {
            buffer[j++] = line[i++];
        }
    }

    // Ajoute le dernier token s'il existe
    if (j > 0)
    {
        buffer[j] = '\0';
        add_token(&token_list, STRING, buffer);
    }

    // Traitement des variables d'environnement pour les tokens
    t_token *current = token_list;
    while (current)
    {
        if (current->type == DOUBLEQUOTE || current->type == STRING)
        {
            char *expanded = expand_variables(current->value, ctx, current->type);
            if (expanded)
            {
                free(current->value);
                current->value = expanded;
            }
        }
        current = current->next;
    }

    return token_list;
}

//Les tests qui échouent maintenant sont ceux où il ne devrait pas y avoir
//d'espace entre les mots quand il n'y en a pas dans la commande originale.
// t_token *parse_command_line(char *line, t_ctx *ctx)
// {
//     if (check_invalid_quotes(line))
//     {
//         fprintf(stderr, "Error: quote found without matching opening quote\n");
//         return NULL;
//     }

//     t_token *token_list = NULL;
//     char buffer[1024] = {0};
//     int i = 0;
//     int j = 0;
//     char current_quote_type = 0;
//     t_token_type current_token_type = STRING;

//     while (line[i])
//     {
//         if ((line[i] == '\'' || line[i] == '"') && !current_quote_type)
//         {
//             current_quote_type = line[i];
//             // Mémoriser le type de quote pour l'expansion plus tard
//             if (current_quote_type == '\'')
//                 current_token_type = SINGLE_QUOTE;
//             else if (current_quote_type == '"')
//                 current_token_type = DOUBLEQUOTE;
//             i++;

//             while (line[i] && line[i] != current_quote_type)
//             {
//                 buffer[j++] = line[i++];
//             }

//             if (!line[i])
//             {
//                 fprintf(stderr, "Error: unclosed quote\n");
//                 free_tokens(token_list);
//                 return NULL;
//             }

//             i++; // Skip la quote fermante
//             current_quote_type = 0;
//         }
//         else if (is_whitespace(line[i]))
//         {
//             if (j > 0)
//             {
//                 buffer[j] = '\0';
//                 add_token(&token_list, current_token_type, buffer);
//                 j = 0;
//                 current_token_type = STRING;  // Reset le type pour le prochain token
//             }
//             i++;
//         }
//         else
//         {
//             buffer[j++] = line[i++];
//         }
//     }

//     // Ajoute le dernier token s'il existe
//     if (j > 0)
//     {
//         buffer[j] = '\0';
//         add_token(&token_list, current_token_type, buffer);
//     }

//     // Traitement des variables d'environnement pour les tokens avec double quotes uniquement
//     t_token *current = token_list;
//     while (current)
//     {
//         if (current->type == DOUBLEQUOTE || current->type == STRING)
//         {
//             char *expanded = expand_variables(current->value, ctx, current->type);
//             if (expanded)
//             {
//                 free(current->value);
//                 current->value = expanded;
//             }
//         }
//         current = current->next;
//     }

//     return token_list;
// }

///bon avec deux variables colles expand et pas expand
// t_token *parse_command_line(char *line, t_ctx *ctx)
// {
//     if (check_invalid_quotes(line))
//     {
//         fprintf(stderr, "Error: quote found without matching opening quote\n");
//         return NULL;
//     }

//     t_token *token_list = NULL;
//     char buffer[1024] = {0};
//     int i = 0;
//     int j = 0;
//     char current_quote_type = 0;
//     t_token_type current_token_type = STRING;

//     while (line[i])
//     {
//         if ((line[i] == '\'' || line[i] == '"') && !current_quote_type)
//         {
//             // Si on a du contenu non-quote avant, on l'ajoute d'abord
//             if (j > 0)
//             {
//                 buffer[j] = '\0';
//                 add_token(&token_list, STRING, buffer);
//                 j = 0;
//             }

//             current_quote_type = line[i];
//             current_token_type = (current_quote_type == '\'') ? SINGLE_QUOTE : DOUBLEQUOTE;
//             i++;

//             // Capture le contenu entre quotes
//             j = 0;
//             while (line[i] && line[i] != current_quote_type)
//                 buffer[j++] = line[i++];

//             if (!line[i])
//             {
//                 fprintf(stderr, "Error: unclosed quote\n");
//                 free_tokens(token_list);
//                 return NULL;
//             }

//             // Ajoute le token avec son contenu
//             buffer[j] = '\0';
//             add_token(&token_list, current_token_type, buffer);
//             j = 0;

//             i++; // Skip la quote fermante
//             current_quote_type = 0;
//             current_token_type = STRING;
//         }
//         else if (is_whitespace(line[i]))
//         {
//             if (j > 0)
//             {
//                 buffer[j] = '\0';
//                 add_token(&token_list, current_token_type, buffer);
//                 j = 0;
//             }
//             i++;
//         }
//         else
//         {
//             buffer[j++] = line[i++];
//         }
//     }

//     // Ajoute le dernier token s'il existe
//     if (j > 0)
//     {
//         buffer[j] = '\0';
//         add_token(&token_list, current_token_type, buffer);
//     }

//     // Traitement des variables d'environnement pour les tokens
//     t_token *current = token_list;
//     while (current)
//     {
//         if (current->type == DOUBLEQUOTE || current->type == STRING)
//         {
//             char *expanded = expand_variables(current->value, ctx, current->type);
//             if (expanded)
//             {
//                 free(current->value);
//                 current->value = expanded;
//             }
//         }
//         current = current->next;
//     }

//     return token_list;
// }

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
        new_token->type = STRING;  // Type par défaut
        if (!new_token->value)
        {
            free(new_token);
            free_tokens(head);
            return NULL;
        }

        new_token->next = NULL;

        // Ajoute le token à la liste
        if (!head)
            head = new_token;
        else
            current->next = new_token;
        current = new_token;
    }

    return head;
}

char *strip_quotes(char *arg)
{
    if ((*arg == '"' || *arg == '\'') && arg[strlen(arg) - 1] == *arg) {
        return ft_strndup(arg + 1, strlen(arg) - 2); // Supprimer les quotes
    }
    return ft_strdup(arg);
}

int is_numeric_argument(const char *arg)
{
    if (!arg || (*arg != '+' && *arg != '-' && !ft_isdigit(*arg)))
        return 0; // Must start with +, -, or a digit
    if (*arg == '+' || *arg == '-')
        arg++; // Skip initial sign
    while (*arg)
    {
        if (!ft_isdigit(*arg))
            return 0; // All remaining characters must be digits
        arg++;
    }
    return 1;
}

void free_env_copy(char **env_copy)
{
    if (!env_copy)
        return;
    for (int i = 0; env_copy[i]; i++)
        free(env_copy[i]);
    free(env_copy);
}

void free_env(t_env_var *env_var)
{
    while(env_var)
    {
        t_env_var *to_free = env_var;
        env_var = env_var->next;

        free(to_free->name);
        free(to_free->value);
        free(to_free);
    }

}
void free_ctx(t_ctx *ctx)
{
    while (ctx)
    {
        t_ctx *to_free = ctx;
        ctx = ctx->next;

        free_env(to_free->env_vars);

        free(to_free->oldpwd);
        free(to_free->pwd);
        free(to_free);
    }
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

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
        free(tmp->content);
		free(tmp);
	}
}

void free_args(char **args)
{
    if (!args)
        return;
    for (int i = 0; args[i]; i++)
        free(args[i]);
    free(args);
}