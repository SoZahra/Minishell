/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:37:16 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/08 11:43:56 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void loop(void)
// {
// 	char *input;

// 	while(1)
// 	{
// 		input = readline("MiniBG> ");
// 		if(!input)
// 		{
// 			printf("exit\n");
// 			break;
// 		}
// 		if(*input)
// 			add_history(input);
// 		printf("You entered: %s\n", input);
// 		free(input);
// 	}
// }

void loop(void)
{
    char *input;
    t_token *tokens;

    while (1)
    {
        input = readline(PROMPT);
        if (!input)
        {
            printf("exit\n");
            break;
        }
        if (*input)
            add_history(input);

        tokens = lexer(input);
        process_tokens(tokens);

        free(input);
        free(tokens);  // N'oublie pas de libérer les tokens aussi
    }
}

