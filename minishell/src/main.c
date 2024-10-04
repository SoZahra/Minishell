/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:32:37 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/04 18:17:41 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(void)
{
	signal(SIGINT, handle_sigint);
	loop();
	return(0);
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