/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:50:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/14 16:06:11 by llarrey          ###   ########.fr       */
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
			printf("value: %s \t type: %c  \t flag: %d\n", tmp->value, tmp->type, tmp->had_space);
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

/* t_command *create_command(t_token *tokens, t_ctx *ctx)
{
    (void)ctx;
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    // cmd =  {0};

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
} */

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

/* t_command *create_command_from_tokens(t_token *tokens)
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
} */

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

void print_command_debug(t_command *cmd, const char *step)
{
    printf("Debug: Command after %s:\n", step);
    printf("Args and had_spaces:\n");
    for (int i = 0; i < cmd->arg_count; i++)
        printf("arg[%d]: '%s', had_space: %d\n", i, cmd->args[i], cmd->had_spaces[i]);
}

t_command *create_command_from_tokens_range(t_token *start, t_token *end)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;

     cmd->args = NULL;
    cmd->redirs = NULL;
    cmd->path = NULL;
    cmd->pid = -1;
    cmd->next = NULL;
    cmd->prev = NULL;
    cmd->pfd[0] = -1;
    cmd->pfd[1] = -1;
    cmd->arg_count = 0;
    // Compter les arguments
    int arg_count = 0;
    t_token *current = start;
    while (current && current != end && current->type != '|')
    {
        if (current->type != '>' && current->type != 'A')
            arg_count++;
        current = current->next;
    }

    cmd->args = malloc(sizeof(char *) * (arg_count + 1));
    cmd->had_spaces = malloc(sizeof(int) * arg_count);
    cmd->arg_count = arg_count;  // Ajouter ce champ dans la structure

    // Remplir les tableaux
    int i = 0;
    current = start;
    while (current && current != end && current->type != '|')
    {
        if (current->type != '>' && current->type != 'A')
        {
            cmd->args[i] = ft_strdup(current->value);
            cmd->had_spaces[i] = current->had_space;
            // printf("Debug: Copying arg[%d]: '%s', had_space: %d\n",
                //    i, current->value, current->had_space);
            i++;
        }
        current = current->next;
    }
    cmd->args[i] = NULL;

    // print_command_debug(cmd, "creation");
    return cmd;
}

// t_command *create_command_from_tokens_range(t_token *start, t_token *end)
// {
//     t_command *cmd = malloc(sizeof(t_command));
//     if (!cmd)
//         return NULL;

//     // Initialisation
//     cmd->args = NULL;
//     cmd->redirs = NULL;
//     cmd->path = NULL;
//     cmd->pid = -1;
//     cmd->next = NULL;
//     cmd->prev = NULL;
//     cmd->pfd[0] = -1;
//     cmd->pfd[1] = -1;

//     // Parcourir les tokens jusqu'au pipe
//     t_token *current = start;
//     while (current && current != end)
//     {
//         if (current->type == '>' || current->type == 'A')
//         {
//             // Gérer les redirections
//             if (!current->next || !add_redirection(&cmd->redirs, current->type, current->next->value))
//             {
//                 free_command(cmd);
//                 return NULL;
//             }
//             current = current->next->next;
//         }
//         else
//         {
//             // Ajouter aux arguments
//             if (!add_arg_to_command(cmd, current->value))
//             {
//                 free_command(cmd);
//                 return NULL;
//             }
//             current = current->next;
//         }
//     }
//     return cmd;
// }
void print_token_debug(t_token *tokens, const char *step)
{
    printf("\nDebug: Tokens after %s:\n", step);
    t_token *current = tokens;
    while (current)
    {
        printf("value: '%s', type: %c, had_space: %d\n",
               current->value, current->type, current->had_space);
        current = current->next;
    }
}

/* t_command *parse_pipe_sequence(t_token *tokens)
{
    t_token *last = tokens;
    while (last && last->next)
        last = last->next;
    if (tokens && tokens->type == '|')
    {
        fprintf(stderr, "MiniBG: syntax error near unexpected token `|'\n");
        return NULL;
    }
    t_command *first_cmd = NULL;
    t_command *current_cmd = NULL;
    t_token *cmd_start = tokens;

    while (cmd_start)
    {
        if (cmd_start->type == '|' && (!cmd_start->next || cmd_start->next->type == '|'))
        {
            fprintf(stderr, "MiniBG: syntax error near unexpected token `|'\n");
            if (first_cmd)
                free_command(first_cmd);
            return NULL;
        }
        // print_token_debug(tokens, "avant create command from tokens");
        // Trouver la fin de la commande actuelle (prochain pipe)
        t_token *cmd_end = find_pipe_token(cmd_start);
        t_command *new_cmd = create_command_from_tokens_range(cmd_start, cmd_end);// Créer la commande jusqu'au pipe
        // print_token_debug(tokens, "apres create command from tokens");
        if (!new_cmd)
        {
            if (first_cmd)
                free_command(first_cmd);
            return NULL;
        }
        if (!first_cmd)  // Lier à la commande précédente
            first_cmd = new_cmd;
        else
        {
            current_cmd->next = new_cmd;
            new_cmd->prev = current_cmd;
        }
        current_cmd = new_cmd; // Passer au token après le pipe
        cmd_start = cmd_end ? cmd_end->next : NULL;
    }
    return first_cmd;
}

t_token *find_pipe_token(t_token *start)
{
    t_token *current = start;
    while (current && current->type != '|')
        current = current->next;
    return current;
}
 */

int handle_line_for_loop(char *line, t_ctx *ctx)
{
    if (!*line)
        return 0;
    if (*line)
    {
        add_history(line);
        t_token *tokens = tokenize_input(line);
        if (tokens)
        {
            if (expand_proc(&tokens, ctx) == -1)
            {
                free_tokens(tokens);
                return 1;
            }
			process_pline(tokens, ctx);
        }
        if (!tokens)
        {
            fprintf(stderr, "Error: tokenization failed\n");
            return 1;
        }
        }
        // print_token_debug(tokens, "tokenization");
        // fprintf(stderr, "Debug: Tokens après tokenization:\n");
        // for (t_token *cur = tokens; cur; cur = cur->next)
        // {
        //     fprintf(stderr, "->>>>>>>>value: '%s', type: %d, had_space: %d\n", cur->value, cur->type, cur->had_space);
        // }

        // Créer la commande
       /*  t_command *cmd = parse_pipe_sequence(tokens);
        if (!cmd)
        {
            free_tokens(tokens);
            return 1;
        }

        // Exécuter différemment selon le cas
        if (cmd->next)  // Si on a des pipes
        {
            execute_pipeline(cmd, ctx);
        }
        else  // Commande simple
        {
            char *final_cmd = tokens_to_string(tokens);
            if (!final_cmd)
            {
                // fprintf(stderr, "Debug: tokens_to_string returned NULL\n");
                free_command(cmd);
                free_tokens(tokens);
                return 1;
            }
            // printf("Debug: final_cmd juste avant usage: '%s'\n", final_cmd);  // Afficher l'état final_cmd avant de l'utiliser
            // printf("Debug: final_cmd: '%s'\n", final_cmd);
            // printf("Debug: cmd->args[0]: '%s'\n", cmd->args[0]);
            if (is_builtin(cmd->args[0]))
            {
                // printf("Debug: Command '%s' is a builtinnn\n", cmd->args[0]);
                execute_builtin(final_cmd, ctx);
            }
            else
            {
                // printf("Debug: Command '%s' is not a builtin\n", cmd->args[0]);
                execute_command(cmd, ctx);
            }
            free(final_cmd);
        } */
    //}
    // free_command(cmd);
    // free_tokens(tokens);
    return 0;
}

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
