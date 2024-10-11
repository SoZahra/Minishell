/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:32:37 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/11 13:56:22 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(void)
{
	signal(SIGINT, handle_sigint);
	loop();
	return(0);
}

void print_tokens(t_token *tokens)
{
    while (tokens) {
        printf("Token: '%s'\n", tokens->value);
        tokens = tokens->next;
    }
}



// test
//MiniBG> echo "Hello World" | grep Hello
// sortie espere :
/*Token type: 0, value: echo
Token type: 7, value: "
Token type: 0, value: Hello World
Token type: 7, value: "
Token type: 1, value: |
Token type: 0, value: grep
Token type: 0, value: Hello
*/