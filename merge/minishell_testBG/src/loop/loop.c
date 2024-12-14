/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:50:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 17:03:27 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void print_tokens(t_token *tokens)
{
	t_token *tmp;

	if (!tokens)
		return ;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->value)
			printf("value: [%s] \t type: %c  \t flag: %d\n", tmp->value, tmp->type, tmp->had_space);
		tmp = tmp->next;
	}
}

t_token *tokenize_input(char *line)
{
    t_token *tokens = NULL;

    if (tokenizer(&tokens, line) < 0)
    {
        free_tokens(tokens);
        return (NULL);
    }
    print_tokens(tokens);
    return (tokens);
}

// void handle_line_for_loop(char *line, t_ctx *ctx)
// {
//     t_token *tokens;
//     char *output;

//     if (*line)
//     {
//         add_history(line);
//         tokens = tokenize_input(line);
//         if (tokens)
//         {
//             output = prepare_command(tokens, ctx);
//             if (output)
//             {
// 				char **cmd_array = create_command_array(output);
//                 if (cmd_array)
//                 {
//                     int builtin_result = is_builtin(output);
//                     if (builtin_result)
//                         execute_builtin(output, ctx); // ou passer cmd_array à la place de output
//                     else
//                         execute_external_command(output, ctx);
//                     free_array(cmd_array);
//                 }
//                 free(output);
//             }
//             free_tokens(tokens);
//         }
//         else
//             fprintf(stderr, "Error: tokenization failed\n");
//     }
// }

// t_command *create_command(t_token *tokens, t_ctx *ctx)
// {
// 	(void)ctx;
//     t_command *cmd = malloc(sizeof(t_command));
//     if (!cmd)
//         return NULL;

//     // cmd->redirs = NULL;
//     // cmd->pipe_in = -1;
//     // cmd->pipe_out = -1;


//     // Traiter les tokens pour séparer commande et redirections
//     char **args = NULL;
//     int arg_count = 0;

//     t_token *current = tokens;
//     while (current)
//     {
//         if (current->type == '>' || current->type == 'A')
//         {
//             // Ajouter une redirection sortante
//             if (!add_redirection(&cmd->redirs, current->type, current->next ? current->next->value : NULL))
//             {
//                 free_command(cmd);
//                 return NULL;
//             }
//             current = current->next ? current->next->next : NULL;
//         }
//         else
//         {
//             // Ajouter aux arguments de la commande
//             args = realloc(args, sizeof(char *) * (arg_count + 2));
//             args[arg_count++] = ft_strdup(current->value);
//             args[arg_count] = NULL;
//             current = current->next;
//         }
//     }
//     cmd->args = args;

//     return cmd;
// }

t_command *create_command(t_token *tokens, t_ctx *ctx)
{
    (void)ctx;
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;

    // Initialisation
    cmd->redirs = NULL;
    cmd->args = NULL;
    cmd->path = NULL;
    cmd->pid = -1;
    cmd->next = NULL;
    cmd->prev = NULL;
    cmd->pfd[0] = -1;
    cmd->pfd[1] = -1;

    char **args = NULL;
    int arg_count = 0;

    t_token *current = tokens;
    while (current)
    {
        if (current->type == '>' || current->type == 'A')
        {
            // Ajouter une redirection sortante
            if (!add_redirection(&cmd->redirs, current->type,
                               current->next ? current->next->value : NULL))
            {
                free_command(cmd);
                return NULL;
            }
            current = current->next ? current->next->next : NULL;
        }
        else
        {
            // Ajouter aux arguments de la commande
            args = realloc(args, sizeof(char *) * (arg_count + 2));
            if (!args)
            {
                free_command(cmd);
                return NULL;
            }
            args[arg_count] = ft_strdup(current->value);
            if (!args[arg_count])
            {
                free_array(args);
                free_command(cmd);
                return NULL;
            }
            args[++arg_count] = NULL;
            current = current->next;
        }
    }
    cmd->args = args;

    return cmd;
}

// Compte le nombre d'arguments en ignorant les redirections
int count_command_args(char **array)
{
    int arg_count = 0;
    int i = 0;

    while (array[i])
    {
        if (array[i][0] == '>' || ft_strcmp(array[i], ">>") == 0)
        {
            if(array[i + 1])
                i++;
            else
                break;
        }
        else
            arg_count++;
        i++;
    }
    return arg_count;
}

// Crée et ajoute une redirection à la liste
// static int add_redirection_to_command(t_command *cmd, char *file, char type)
// {
//     // On passe l'adresse de cmd->redirs car la fonction attend un t_redirection**
//     if (!add_redirection(&cmd->redirs, type, file))
//         return 0;
//     return 1;
// }

// Alloue et remplit le tableau d'arguments
// static char **fill_command_args(char **array, int arg_count)
// {
//     char **args = malloc(sizeof(char *) * (arg_count + 1));
//     if (!args)
//         return NULL;

//     int arg_idx = 0;
//     int i = 0;
//     while (array[i])
//     {
//         if (array[i][0] == '>' || ft_strcmp(array[i], ">>") == 0)
//         {
//             if(array[i + 1])
//                 i++;
//             else
//                 break;
//         }
//         else
//             args[arg_idx++] = ft_strdup(array[i++]);
//         i++;
//     }
//     args[arg_idx] = NULL;
//     return args;
// }

int add_arg_to_command(t_command *cmd, char *arg)
{
   if (!arg)
       return 0;

   // Compter les arguments actuels
   int arg_count = count_args(cmd->args);

   // Réallouer le tableau avec un espace de plus
   char **new_args = realloc(cmd->args, sizeof(char *) * (arg_count + 2));
   if (!new_args)
       return 0;

   cmd->args = new_args;

   // Ajouter le nouvel argument
   cmd->args[arg_count] = ft_strdup(arg);
   if (!cmd->args[arg_count])
       return 0;

   // Terminer par NULL
   cmd->args[arg_count + 1] = NULL;

   return 1;
}

// Fonction principale simplifiée
// t_command *create_command_from_array(char **array, t_ctx *ctx)
// {
// 	(void)ctx;
//     t_command *cmd = malloc(sizeof(t_command));
//     if (!cmd)
//         return NULL;

//     // Initialisation
//     cmd->redirs = NULL;
//     cmd->pipe_in = -1;
//     cmd->pipe_out = -1;

//     // Traitement des redirections
//     for (int i = 0; array[i]; i++)
//     {
//         if (array[i][0] == '>' || ft_strcmp(array[i], ">>") == 0)
//         {
//             if(!array[i + 1])
//                 return (free_command(cmd), NULL);
//             char type = (array[i][1] == '>') ? 'A' : '>';
//             if (!add_redirection_to_command(cmd, array[i + 1], type))
//             {
//                 free_command(cmd);
//                 return NULL;
//             }
//             i++;  // Sauter le fichier de redirection
//         }
//     }
//     // Création des arguments
//     int arg_count = count_command_args(array);
//     cmd->args = fill_command_args(array, arg_count);
//     if (!cmd->args)
//     {
//         free_command(cmd);
//         return NULL;
//     }
//     return cmd;
// }

int append_arg_value(char **current_arg, const char *value, int had_space)
{
    if (!*current_arg)
    {
        *current_arg = ft_strdup(value);
        return *current_arg != NULL;
    }

    // Si had_space est à 1, on ajoute directement
    if (had_space)
    {
        char *temp = ft_strjoin(*current_arg, value);
        if (!temp)
            return 0;
        free(*current_arg);
        *current_arg = temp;
        return 1;
    }

    // Si had_space est à 0, on ajoute avec espace
    char *temp = *current_arg;
    *current_arg = ft_strjoin(temp, " ");
    free(temp);
    if (!*current_arg)
        return 0;

    temp = *current_arg;
    *current_arg = ft_strjoin(temp, value);
    free(temp);
    return *current_arg != NULL;
}

t_command *create_command_from_tokens(t_token *tokens)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    *cmd = (t_command){0};

    t_token *current = tokens;
    char *current_arg = NULL;
    // int arg_count = 0;

    while (current)
    {
        if (current->type == '>' || current->type == 'A')
        {
            // Finir l'argument en cours si nécessaire
            if (current_arg)
            {
                if (!add_arg_to_command(cmd, current_arg))
                {
                    free(current_arg);
                    free_command(cmd);
                    return NULL;
                }
                free(current_arg);
                current_arg = NULL;
            }
            // Ajouter la redirection
            if (!current->next || !add_redirection(&cmd->redirs, current->type, current->next->value))
            {
                free_command(cmd);
                return NULL;
            }
            current = current->next ? current->next->next : NULL;
        }
        else
        {
            // Ajouter à l'argument en cours ou créer un nouvel argument
            if (!append_arg_value(&current_arg, current->value, current->had_space))
            {
                free(current_arg);
                free_command(cmd);
                return NULL;
            }
            // Si c'est le dernier token ou le suivant est une redirection
            if (!current->next || (current->next && (current->next->type == '>' || current->next->type == 'A')))
            {
                if (!add_arg_to_command(cmd, current_arg))
                {
                    free(current_arg);
                    free_command(cmd);
                    return NULL;
                }
                free(current_arg);
                current_arg = NULL;
            }
            current = current->next;
        }
    }
    return cmd;
}

// int handle_line_for_loop(char *line, t_ctx *ctx)
// {
//     if (!*line)
//         return 0;

//     add_history(line);
//     // 1. Tokenization
//     t_token *tokens = tokenize_input(line);
//     if (!tokens)
//     {
//         fprintf(stderr, "Error: tokenization failed\n");
//         return 1;
//     }
//     // 2. Expansion et préparation
//     if (expand_proc(&tokens, ctx) == -1)
//     {
//         free_tokens(tokens);
//         return 1;
//     }
//     // 3. Création de la commande et gestion des redirections
//     t_command *cmd = create_command_from_tokens(tokens);
//     if (!cmd)
//     {
//         free_tokens(tokens);
//         return 1;
//     }
//     // 4. Exécution avec redirections
//     execute_command(cmd, ctx);
//     // Nettoyage
//     free_command(cmd);
//     free_tokens(tokens);
//     return 0;
// }

t_command *create_command_from_tokens_range(t_token *start, t_token *end)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;

    // Initialisation
    cmd->args = NULL;
    cmd->redirs = NULL;
    cmd->path = NULL;
    cmd->pid = -1;
    cmd->next = NULL;
    cmd->prev = NULL;
    cmd->pfd[0] = -1;
    cmd->pfd[1] = -1;

    // Parcourir les tokens jusqu'au pipe
    t_token *current = start;
    while (current && current != end)
    {
        if (current->type == '>' || current->type == 'A')
        {
            // Gérer les redirections
            if (!current->next || !add_redirection(&cmd->redirs, current->type, current->next->value))
            {
                free_command(cmd);
                return NULL;
            }
            current = current->next->next;
        }
        else
        {
            // check if token->value[0] == '\0', if true then join the prev and next
            // printf("wesh ici\n");
            if (current->value[0] != 0 && !add_arg_to_command(cmd, current->value))
            {
                free_command(cmd);
                return NULL;
            }
            current = current->next;
        }
    }
    return cmd;
}

// t_command *parse_pipe_sequence(t_token *tokens)
// {
//     t_command *first_cmd = NULL;
//     t_command *current_cmd = NULL;
//     t_token *current = tokens;

//     while (current)
//     {
//         // Créer une nouvelle commande
//         t_command *new_cmd = malloc(sizeof(t_command));
//         memset(new_cmd, 0, sizeof(t_command));
//         new_cmd->redirs = NULL;  // Initialiser à NULL

//         // Compter les arguments et les redirections
//         int arg_count = 0;
//         int redir_count = 0;
//         t_token *token = current;

//         // Parcourir et compter
//         while (token && token->type != '|')
//         {
//             if (token->type == '>' || token->type == '<' ||
//                 token->type == TOKEN_REDIRECT_APPEND ||
//                 token->type == TOKEN_HEREDOC)
//             {
//                 redir_count++;
//                 if (token->next)
//                     token = token->next;  // Passer le fichier de redirection
//             }
//             else if (token->type == 'S')
//             {
//                 arg_count++;
//             }
//             token = token->next;
//         }

//         // Allouer les arguments
//         new_cmd->args = malloc(sizeof(char *) * (arg_count + 1));
//         new_cmd->had_spaces = malloc(sizeof(int) * arg_count);
//         new_cmd->arg_count = arg_count;

//         // Allouer et initialiser les redirections
//         new_cmd->redirs = malloc(sizeof(t_redirection) * (redir_count + 1));
//         memset(new_cmd->redirs, 0, sizeof(t_redirection) * (redir_count + 1));

//         // Réinitialiser pour le remplissage
//         int arg_index = 0;
//         int redir_index = 0;
//         token = current;

//         // Remplir les arguments et les redirections
//         while (token && token->type != '|')
//         {
//             if (token->type == '>' || token->type == '<' ||
//                 token->type == TOKEN_REDIRECT_APPEND ||
//                 token->type == TOKEN_HEREDOC)
//             {
//                 // Ajouter la redirection
//                 new_cmd->redirs[redir_index].type = token->type;
//                 if (token->next)
//                 {
//                     new_cmd->redirs[redir_index].file = strdup(token->next->value);

//                     // Chaîner les redirections
//                     if (redir_index > 0)
//                     {
//                         new_cmd->redirs[redir_index - 1].next = &new_cmd->redirs[redir_index];
//                     }

//                     token = token->next;  // Passer le fichier de redirection
//                 }
//                 redir_index++;
//             }
//             else if (token->type == 'S')
//             {
//                 // Ajouter l'argument
//                 new_cmd->args[arg_index] = strdup(token->value);
//                 new_cmd->had_spaces[arg_index] = token->had_space;
//                 arg_index++;
//             }
//             token = token->next;
//         }

//         // Terminer les tableaux
//         new_cmd->args[arg_count] = NULL;
//         new_cmd->redirs[redir_count].type = 0;  // Marquer la fin
//         new_cmd->redirs[redir_count].file = NULL;

//         // Chaîner les commandes
//         if (!first_cmd)
//             first_cmd = new_cmd;
//         else
//         {
//             current_cmd->next = new_cmd;
//             new_cmd->prev = current_cmd;
//         }
//         current_cmd = new_cmd;

//         // Passer au prochain token après le pipe
//         current = token ? token->next : NULL;
//     }
//     return first_cmd;
// }

void count_tokens_redir(t_token *token, int *arg_count, int *redir_count)
{
    *arg_count = 0;
    *redir_count = 0;

    while (token && token->type != '|')
    {
        if (token->type == '>' || token->type == '<' ||
            token->type == TOKEN_REDIRECT_APPEND ||
            token->type == TOKEN_HEREDOC)
        {
            (*redir_count)++;
            token = token->next ? token->next : NULL;
        }
        else if (token->type == 'S')
            (*arg_count)++;
        if (token)
            token = token->next;
    }
}

void fill_command_tokens(t_token *token, t_command *new_cmd)
{
    int arg_index = 0;
    int redir_index = 0;

    while (token && token->type != '|')
    {
        if (token->type == '>' || token->type == '<' ||
            token->type == TOKEN_REDIRECT_APPEND ||
            token->type == TOKEN_HEREDOC)
        {
            new_cmd->redirs[redir_index].type = token->type;
            if (token->next)
            {
                new_cmd->redirs[redir_index].file = strdup(token->next->value);

                if (redir_index > 0)
                {
                    new_cmd->redirs[redir_index - 1].next = &new_cmd->redirs[redir_index];
                }
                token = token->next;
            }
            redir_index++;
        }
        else if (token->type == 'S')
        {
            new_cmd->args[arg_index] = strdup(token->value);
            new_cmd->had_spaces[arg_index] = token->had_space;
            arg_index++;
        }
        token = token->next;
    }
    new_cmd->args[arg_index] = NULL;
    new_cmd->redirs[redir_index].type = 0;
    new_cmd->redirs[redir_index].file = NULL;
}

t_command *parse_pipe_sequence(t_token *tokens)
{
    t_command *first_cmd = NULL;
    t_command *current_cmd = NULL;
    t_token *current = tokens;

    while (current)
    {
        int arg_count = 0;
        int redir_count = 0;

        // Compter les tokens jusqu'au prochain pipe
        t_token *token_counter = current;
        while (token_counter && token_counter->type != '|')
        {
            if (token_counter->type == '>' || token_counter->type == '<' ||
                token_counter->type == TOKEN_REDIRECT_APPEND ||
                token_counter->type == TOKEN_HEREDOC)
            {
                redir_count++;
                token_counter = token_counter->next ? token_counter->next : NULL;
            }
            else if (token_counter->type == 'S')
                arg_count++;
            if (token_counter)
                token_counter = token_counter->next;
        }
        t_command *new_cmd = allocate_command(arg_count, redir_count);
        // Remplir les tokens
        fill_command_tokens(current, new_cmd);
        // Chaîner les commandes
        link_commands(&first_cmd, &current_cmd, new_cmd);
        // Passer à la prochaine commande
        current = (current_cmd->redirs && current_cmd->redirs[0].type != 0)
                  ? NULL
                  : (token_counter ? token_counter->next : NULL);
    }
    return first_cmd;
}

void link_commands(t_command **first_cmd, t_command **current_cmd, t_command *new_cmd)
{
    if (!*first_cmd)
        *first_cmd = new_cmd;
    else
    {
        (*current_cmd)->next = new_cmd;
        new_cmd->prev = *current_cmd;
    }
    *current_cmd = new_cmd;
}

t_command *allocate_command(int arg_count, int redir_count)
{
    t_command *new_cmd = malloc(sizeof(t_command));
    memset(new_cmd, 0, sizeof(t_command));

    new_cmd->args = malloc(sizeof(char *) * (arg_count + 1));
    new_cmd->had_spaces = malloc(sizeof(int) * arg_count);
    new_cmd->arg_count = arg_count;

    new_cmd->redirs = malloc(sizeof(t_redirection) * (redir_count + 1));
    ft_memset(new_cmd->redirs, 0, sizeof(t_redirection) * (redir_count + 1));
    return new_cmd;
}

t_token *find_pipe_token(t_token *start)
{
    t_token *current = start;
    while (current && current->type != '|')
        current = current->next;
    return current;
}

// char *trim_whitespace(const char *str) {
//     if (!str)
//         return NULL;

//     const char *start = str;
//     const char *end = str + strlen(str) - 1;

//     // Skip spaces au début
//     while (*start && ft_isspace((unsigned char)*start))
//         start++;
//     // Si la chaîne est vide après avoir skip les espaces
//     if (*start == '\0')
//         return strdup("");
//     // Skip spaces à la fin
//     while (end > start && ft_isspace((unsigned char)*end))
//         end--;
//     // Calculer la taille de la chaîne nettoyée
//     size_t len = end - start + 1;
//     // Allouer et copier la nouvelle chaîne
//     char *trimmed = malloc(len + 1);
//     if (!trimmed)
//         return NULL;
//     strncpy(trimmed, start, len);
//     trimmed[len] = '\0';
//     return trimmed;
// }

// void clean_token_whitespace(t_command *cmd) {
//     while (cmd) {
//         for (int i = 0; i < cmd->arg_count; i++) {
//             if (cmd->args[i]) {
//                 fprintf(stderr, "Debug: Before trim: '%s'\n", cmd->args[i]);
//                 char *cleaned = trim_whitespace(cmd->args[i]);
//                 if (cleaned) {
//                     free(cmd->args[i]);
//                     cmd->args[i] = cleaned;
//                     fprintf(stderr, "Debug: After trim: '%s'\n", cmd->args[i]);
//                 } else {
//                     perror("trim_whitespace failed");
//                 }
//             }
//         }
//         cmd = cmd->next;
//     }
// }

int handle_line_for_loop(char *line, t_ctx *ctx)
{
    if (!*line)
        return 0;

    add_history(line);
    // 1. Tokenization
    t_token *tokens = tokenize_input(line);
    if (!tokens)
    {
        fprintf(stderr, "Error: tokenization failed\n");
        return 1;
    }
    // 2. Expansion et préparation
    if (expand_proc(&tokens, ctx) == -1)
    {
        free_tokens(tokens);
        return 1;
    }
    print_tokens(tokens);
    char *final_cmd = tokens_to_string(tokens);
    if (!final_cmd)
    {
        free_tokens(tokens);
        return 1;
    }
    t_command *cmd = parse_pipe_sequence(tokens);
    if (!cmd)
    {
        free_tokens(tokens);
        return 1;
    }
    t_command *current = cmd;
    while (current) {
        fprintf(stderr, "Debug: Command: '%s', Next: %s, Prev: %s\n",
            current->args[0],
            current->next ? current->next->args[0] : "NULL",
            current->prev ? current->prev->args[0] : "NULL");
        current = current->next;
    }
    // Vérifier la présence de redirections
    int has_redirections = 0;
    current = cmd;
    while (current)
    {
        if (current->redirs && current->redirs[0].type != 0)
        {
            has_redirections = 1;
            break;
        }
        current = current->next;
    }// Choisir le mode d'exécution
    if (cmd->next)  // Si on a des pipes
        execute_pipeline(cmd, ctx);
    else
        execute_command(cmd, ctx);
    // else if (has_redirections)
    //     execute_command(cmd, ctx);  // Avec redirections
    // else if (is_builtin(cmd->args[0]))
    //     execute_builtin(final_cmd, ctx);
    // else
    //     execute_command(cmd, ctx);
    free_tokens(tokens);
    return 0;
}

// int handle_line_for_loop(char *line, t_ctx *ctx)
// {
//     if (!*line)
//         return 0;

//     add_history(line);
//     // 1. Tokenization
//     t_token *tokens = tokenize_input(line);
//     if (!tokens)
//     {
//         fprintf(stderr, "Error: tokenization failed\n");
//         return 1;
//     }
//     // 2. Expansion et préparation
//     if (expand_proc(&tokens, ctx) == -1)
//     {
//         free_tokens(tokens);
//         return 1;
//     }
//     print_tokens(tokens);
//     char *final_cmd = tokens_to_string(tokens);
//     if (!final_cmd)
//     {
//         // fprintf(stderr, "Debug: tokens_to_string returned NULL\n");
//         // free_command(cmd);
//         free_tokens(tokens);
//         return 1;
//     }
//     t_command *cmd = parse_pipe_sequence(tokens);
//     // clean_token_whitespace(cmd);
//     if (!cmd)
//     {
//         // fprintf(stderr, "Debug: Failed to parse command sequence\n");
//         free_tokens(tokens);
//         return 1;
//     }
//     t_command *current = cmd;
//     while (current) {
//         fprintf(stderr, "Debug: Command: '%s', Next: %s, Prev: %s\n",
//             current->args[0],
//             current->next ? current->next->args[0] : "NULL",
//             current->prev ? current->prev->args[0] : "NULL");
//         current = current->next;
//     }
//     if (cmd->next)  // Si on a des pipes
//         execute_pipeline(cmd, ctx);
//     else if (is_builtin(cmd->args[0]))
//             execute_builtin(final_cmd, ctx);
//     else
//         execute_command(cmd, ctx);
//     // Nettoyage
//     free_command(cmd);
//     free_tokens(tokens);
//     return 0;
// }

t_token *prepare_tokens(t_token *tokens, t_ctx *ctx)
{
    // Faire l'expansion des variables
    if (expand_proc(&tokens, ctx) == -1)
        return NULL;

    // Joindre les tokens avec espace en respectant les flags
    if (join_proc(&tokens, false) == -1)
        return NULL;

    return tokens;
}

int	loop_with_pipes(t_ctx *ctx)
{
	char	*line;

	while (1)
	{
		// line = readline(PROMPT);
		line = readline("MiniBG> ");
		if (line == NULL)
		{
			write(1, "exit\n", 5);
			exit(ctx->exit_status);
		}
		handle_line_for_loop(line, ctx);
		free(line);
	}
	return (ctx->exit_status);
}
