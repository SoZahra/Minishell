/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:50:52 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 14:51:01 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
            }
        }
        free(line);
    }
    return ctx->exit_status;
}