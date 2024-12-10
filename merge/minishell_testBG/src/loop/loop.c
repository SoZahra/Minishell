/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatimazahrazayani <fatimazahrazayani@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:50:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/10 23:43:35 by fatimazahra      ###   ########.fr       */
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

//     cmd->redirs = NULL;
//     cmd->pipe_in = -1;
//     cmd->pipe_out = -1;

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

// Compte le nombre d'arguments en ignorant les redirections
static int count_command_args(char **array)
{
    int arg_count = 0;
    int i = 0;

    while (array[i])
    {
        if (array[i][0] == '>' || ft_strcmp(array[i], ">>") == 0)
            i += 2;  // Sauter la redirection et son fichier
        else
            arg_count++;
        i++;
    }
    return arg_count;
}

// Crée et ajoute une redirection à la liste
static int add_redirection_to_command(t_command *cmd, char *file, char type)
{
    // On passe l'adresse de cmd->redirs car la fonction attend un t_redirection**
    if (!add_redirection(&cmd->redirs, type, file))
        return 0;
    return 1;
}

// Alloue et remplit le tableau d'arguments
static char **fill_command_args(char **array, int arg_count)
{
    char **args = malloc(sizeof(char *) * (arg_count + 1));
    if (!args)
        return NULL;

    int arg_idx = 0;
    int i = 0;
    while (array[i])
    {
        if (array[i][0] == '>' || ft_strcmp(array[i], ">>") == 0)
            i += 2;
        else
            args[arg_idx++] = ft_strdup(array[i++]);
    }
    args[arg_idx] = NULL;

    return args;
}

// Fonction principale simplifiée
t_command *create_command_from_array(char **array, t_ctx *ctx)
{
	(void)ctx;
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;

    // Initialisation
    cmd->redirs = NULL;
    cmd->pipe_in = -1;
    cmd->pipe_out = -1;

    // Traitement des redirections
    for (int i = 0; array[i]; i++)
    {
        if (array[i][0] == '>' || ft_strcmp(array[i], ">>") == 0)
        {
            char type = (array[i][1] == '>') ? 'A' : '>';
            if (!add_redirection_to_command(cmd, array[i + 1], type))
            {
                free_command(cmd);
                return NULL;
            }
            i++;  // Sauter le fichier de redirection
        }
    }

    // Création des arguments
    int arg_count = count_command_args(array);
    cmd->args = fill_command_args(array, arg_count);
    if (!cmd->args)
    {
        free_command(cmd);
        return NULL;
    }

    return cmd;
}

void handle_line_for_loop(char *line, t_ctx *ctx)
{
    if (!*line)
        return;

    add_history(line);
    t_token *tokens = tokenize_input(line);
    if (!tokens)
    {
        fprintf(stderr, "Error: tokenization failed\n");
        return;
    }

    // Garder la même logique de préparation
    char *prepared_str = prepare_command(tokens, ctx);
    if (!prepared_str)
    {
        free_tokens(tokens);
        return;
    }
    // Créer le tableau de commandes
    char **cmd_array = create_command_array(prepared_str);
    if (!cmd_array)
    {
        free(prepared_str);
        free_tokens(tokens);
        return;
    }
    // Créer la structure de commande à partir du tableau
    t_command *cmd = create_command_from_array(cmd_array, ctx);
    if (!cmd)
    {
        free_array(cmd_array);
        free(prepared_str);
        free_tokens(tokens);
        return;
    }
    // Exécuter la commande
    execute_command(cmd, ctx);
    // Nettoyage
    free_command(cmd);
    free_array(cmd_array);
    free(prepared_str);
    free_tokens(tokens);
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
