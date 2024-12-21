/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:50:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/21 18:20:47 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		g_var_global = 0;

char	*empty_completion(const char *text, int state)
{
	(void)text;
	(void)state;
	return (NULL);
}

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_var_global = 0;
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

void print_command_debug(t_command *command)
{
    t_command *cmd;


    cmd = command;
    fprintf(stderr, "\n=== Command Debug ===\n");
    
    while (cmd)
    {
        fprintf(stderr, "Command:\n");
        fprintf(stderr, "  Args:\n");
        for (int i = 0; cmd->args && cmd->args[i]; i++)
        {
            fprintf(stderr, "    [%d]: '%s' (had_space: %d)\n", 
                i, cmd->args[i], cmd->had_spaces[i]);
        }

        fprintf(stderr, "  Redirections:\n");
        for (int i = 0; cmd->redirs && cmd->redirs[i].type != 0; i++)
        {
            fprintf(stderr, "    [%d]: type: '%c' file: '%s'\n",
                i, cmd->redirs[i].type, cmd->redirs[i].file);
        }

        fprintf(stderr, "  Links:\n");
        fprintf(stderr, "    Next: %s\n", cmd->next ? cmd->next->args[0] : "NULL");
        fprintf(stderr, "    Prev: %s\n", cmd->prev ? cmd->prev->args[0] : "NULL");
        
        fprintf(stderr, "  Path: %s\n", cmd->path ? cmd->path : "NULL");
        
        fprintf(stderr, "-----------------\n");
        cmd = cmd->next;
    }
    fprintf(stderr, "=== End Debug ===\n\n");
}

t_token *get_last_node(t_token *tokens)
{
    if(!tokens)
        return (NULL);
    t_token *tmp;

    tmp = tokens;
    while (tmp->next)
        tmp = tmp->next;
    return tmp;
}


int check_syntax_errors(const char *line)
{
   if (ft_strlen(line) == 1 && is_token(line[0], REDIRS))
       return (printf("syntax error\n"), 1);
   if (ft_strlen(line) == 2 && line[0] == line[1] && 
       (line[0] == '<' || line[0] == '>' || line[0] == '|'))
       return (printf("syntax error\n"), 1);
   return 0;
}

t_token *tokenize_input(char *line)
{
    if (!line)
        return NULL;
    if (check_syntax_errors(line))
        return NULL;
    t_token *tokens = NULL;
    t_token *tmp;
    if (tokenizer(&tokens, line) < 0)
        return NULL;

    tmp = get_last_node(tokens);
    if(!tmp)
        return(NULL);
    if (is_token(tmp->type, REDIRS))
    {
        printf("syntax error\n");
        if(!tokens)
            free_tokens(tokens);
        return NULL;
    }
    return tokens;
}


// void count_tokens_redir(t_token *start, t_token *end, int *arg_count, int *redir_count)
// {
//     t_token *current = start;
    
//     while (current && current != end)
//     {
//         if (current->type == '<' || current->type == '>' || 
//             current->type == 'A' || current->type == 'H')
//         {
//             (*redir_count)++;
//             current = current->next;
//         }
//         else if (current->type == 'S' || current->type == '"' || 
//                  current->type == '\'')
//             (*arg_count)++;
//         if (current)
//             current = current->next;
//     }
// }

// void fill_command_tokens(t_token *token, t_command *new_cmd)
// {
//     int arg_index = 0;
//     int redir_index = 0;

//     while (token && token->type != '|')
//     {
//         if (token->type == '>' || token->type == '<' ||
//             token->type == TOKEN_REDIRECT_APPEND ||
//             token->type == TOKEN_HEREDOC)
//         {
//             new_cmd->redirs[redir_index].type = token->type;
//             if (token->next)
//             {
//                 new_cmd->redirs[redir_index].file = strdup(token->next->value);
//                 if (redir_index > 0)
//                 {
//                     new_cmd->redirs[redir_index - 1].next = &new_cmd->redirs[redir_index];
//                 }
//                 token = token->next;
//             }
//             redir_index++;
//         }
//         else if (token->type == 'S' || 
//                  token->type == '\'' ||
//                  token->type == '"')  // Ajout du type double quote
//         {
//             new_cmd->args[arg_index] = ft_strdup(token->value);
//             new_cmd->had_spaces[arg_index] = token->had_space;
//             arg_index++;
//         }
//         token = token->next;
//     }
//     new_cmd->args[arg_index] = NULL;
//     new_cmd->redirs[redir_index].type = 0;
//     new_cmd->redirs[redir_index].file = NULL;
// }


// int fill_command_content(t_command *cmd, t_token *start, t_token *end, int arg_count)
// {
//     int arg_idx = 0;
//     int redir_idx = 0;
//     t_token *current = start;
//     t_redirection *redir_ptr;

//     while (current && current != end)
//     {
//         if (is_redirection(current->type))
//         {
//             if (!current->next)
//                 return -1;
//             redir_ptr = &(cmd->redirs[redir_idx]);
//             if (add_redirection(&redir_ptr, current->type, current->next->value) == -1)
//                 return -1;
//             redir_idx++;
//             current = current->next;
//         }
//         else if (is_argument(current->type))
//         {
//             if (arg_idx >= arg_count)
//                 return -1;
//             cmd->args[arg_idx++] = strdup(current->value);
//             if (!cmd->args[arg_idx - 1])
//                 return -1;
//         }
//         if (current)
//             current = current->next;
//     }
//     cmd->args[arg_idx] = NULL;
//     cmd->redirs[redir_idx].type = 0;
//     return 0;
// }

// t_command *init_command_struct(int arg_count)
// {
//     t_command *cmd = calloc(1, sizeof(t_command)); // Utiliser calloc au lieu de malloc
//     if (!cmd)
//         return (NULL);

//     // *cmd = (t_command){0};
//     cmd->pid = -1;
//     cmd->pfd[0] = -1;
//     cmd->pfd[1] = -1;
//     cmd->arg_count = arg_count;
//     cmd->args = calloc(arg_count + 1, sizeof(char *));
//     if(!cmd->args)
//     {
//         free_command(cmd);
//         return (NULL);
//     }
//     cmd->had_spaces = calloc(arg_count, sizeof(int));
//     if (!cmd->had_spaces)
//     {
//         free_command(cmd);
//         return NULL;
//     }
//     cmd->had_spaces = calloc(arg_count, sizeof(int));
//     if (!cmd->had_spaces)
//     {
//         free_command(cmd);
//         return NULL;
//     }
//     return cmd;
// }

t_command *init_command_struct(int arg_count, int redir_count)
{
    t_command *cmd = calloc(1, sizeof(t_command));
    if (!cmd)
        return NULL;

    cmd->pid = -1;
    cmd->pfd[0] = -1;
    cmd->pfd[1] = -1;
    cmd->arg_count = arg_count;
    
    cmd->args = calloc(arg_count + 1, sizeof(char *));
    if (!cmd->args)
    {
        free_command_list(cmd);
        return NULL;
    }

    cmd->redirs = calloc(redir_count + 1, sizeof(t_redirection));
    if (!cmd->redirs)
    {
        free_command_list(cmd);
        return NULL;
    }

    cmd->had_spaces = calloc(arg_count, sizeof(int));
    if (!cmd->had_spaces)
    {
        free_command_list(cmd);
        return NULL;
    }

    return cmd;
}

int is_redirection(t_token_type type)
{
    return (type == '<' || type == '>' || type == 'A' || type == 'H');
}

int is_argument(t_token_type type)
{
    return (type == 'S' || type == '"' || type == '\'');
}

int add_redir(t_command *cmd, t_token *token, int *redir_i)
{
    if (!cmd || !cmd->redirs)
        return 1;

    cmd->redirs[*redir_i].type = token->type;
    if(token->type)
    {
        cmd->redirs[*redir_i].file = ft_strdup(token->next->value);
        if (!cmd->redirs[*redir_i].file)
            return (1);
    }
    (*redir_i)++;
    return (0);
}

int add_argument(t_command *cmd, t_token *token, int *arg_idx)
{
    if (!cmd || !cmd->args)
        return 1;  
    cmd->args[*arg_idx] = ft_strdup(token->value);
    if (!cmd->args[*arg_idx])
        return(1);
    cmd->had_spaces[*arg_idx] = token->had_space;
    (*arg_idx)++;
    return (0);
}

t_command *create_command_from_tokens_range(t_token *start, t_token *end)
{
    if (!start)
        return NULL;
    int arg_count;
    int redir_count;
    int arg_i;
    int redir_i;
    t_token *current;

    current = start;
    arg_count = ((redir_count = arg_i = redir_i = 0));
    while (current && current != end && current->type != '|')
    {
        if (is_redirection(current->type))
        {
            redir_count++;
            if (current->next)
                current = current->next;
        }
        else if (is_argument(current->type))
            arg_count++;
        if (current)
            current = current->next;
    }
    t_command *cmd = init_command_struct(arg_count, redir_count);
    if (!cmd)
        return (NULL);
    current = start; 
    while (current && current != end && current->type != '|')
    {
        if (is_redirection(current->type))
        {
            if (add_redir(cmd, current, &redir_i))
                return (free_command_list(cmd) ,NULL);
            if (current->next)
                current = current->next;
        }
        else if (is_argument(current->type))
        {
            if (add_argument(cmd, current, &arg_i))
                return (free_command_list(cmd), NULL);
        }
        current = current->next;
    }
    cmd->args[arg_i] = NULL;
    cmd->redirs[redir_i].type = 0;
    cmd->arg_count = arg_i;
    return cmd;
}

t_token *get_next_pipe_token(t_token *start) 
{
    while (start && start->type != '|')
        start = start->next;
    return start;
}

t_command *link_commands(t_command *first_cmd, t_command *new_cmd)
{
    t_command *last_cmd = first_cmd;

    if (!first_cmd)
        return new_cmd;
    while (last_cmd->next)
        last_cmd = last_cmd->next;
    last_cmd->next = new_cmd;
    new_cmd->prev = last_cmd;
    return first_cmd;
}

t_token *find_next_command(t_token *current)
{
    while (current && current->type != '|')
        current = current->next;
    return (current ? current->next : NULL);
}

t_command *parse_pipe_sequence(t_token *tokens)
{
    t_token *current = tokens;
    t_command *first_cmd = NULL;
    t_token *cmd_end;
    t_command *new_cmd;

    while (current)
    {
        cmd_end = current;
        while (cmd_end && cmd_end->type != '|')
            cmd_end = cmd_end->next;
        new_cmd = create_command_from_tokens_range(current, cmd_end);
        if (!new_cmd)
        {
            free_command_list(first_cmd);
            return (NULL);
        }
        first_cmd = link_commands(first_cmd, new_cmd);
        if(!first_cmd)
            return (free_command_list(new_cmd), NULL);
        current = find_next_command(current);
    }
    return first_cmd;
}

t_command *allocate_command(int arg_count, int redir_count)
{
    t_command *new_cmd = malloc(sizeof(t_command));
    *new_cmd = (t_command){0};
    new_cmd->args = malloc(sizeof(char *) * (arg_count + 1));
    new_cmd->had_spaces = malloc(sizeof(int) * arg_count);
    new_cmd->arg_count = arg_count;
    new_cmd->redirs = malloc(sizeof(t_redirection) * (redir_count + 1));
    *new_cmd->redirs = (t_redirection){0};
    for (int i = 0; i <= redir_count; i++)
        new_cmd->redirs[i].heredoc_fd = -1;
    return new_cmd;
}

t_token *find_pipe_token(t_token *start)
{
    t_token *current = start;
    while (current && current->type != '|')
        current = current->next;
    return current;
}

int handle_line_for_loop(char *line, t_ctx *ctx)
{
    if (!line || !*line)
        return 1;
    add_history(line);
    t_token *tokens = tokenize_input(line);
    if (!tokens)
        return (ft_fprintf(2, "Error: tokenization failed\n"), 1);
    if (expand_proc(&tokens, ctx) == -1)
        return (free_tokens(tokens), 1);
    t_command *cmd = parse_pipe_sequence(tokens);
    free_tokens(tokens);
    if (!cmd)
        return 1;
    if (exec_loop(ctx, cmd))
        return (free_command_list(cmd), 1);
    free_command_list(cmd);
    return 0;
}

int	process(t_ctx *ctx)
{
	char	*line;

	while (1)
	{
		line = readline("MiniBG> ");
		if (line == NULL)
		{
			write(1, "exit\n", 5);
            rl_clear_history();
            if(g_var_global)
                ctx->exit_status = 130;
			return(ctx->exit_status);
		}
		handle_line_for_loop(line, ctx);
        free(line);
	}
	return (ctx->exit_status);
}
