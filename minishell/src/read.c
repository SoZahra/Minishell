/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:37:16 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/11 14:35:36 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	loop(void)
// {
// 	char *input;
// 	t_token *tokens;

// 	while (1)
// 	{
// 		input = readline(PROMPT);
// 		if (!input)
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if (*input)
// 			add_history(input);

// 		tokens = lexer(input);
// 		if (!tokens)
// 		{ // Si une erreur est survenue pendant l'analyse
// 			free(input);
// 			continue ; // Retourne au début de la boucle
// 		}
//         print_tokens(tokens);
// 		// Traitement des tokens (à implémenter)
// 		free_tokens(tokens);
// 		free(input);
// 	}
// }


void process_tokens(t_token *tokens)
{
    // Si la liste des tokens est vide, retourner
    if (!tokens) {
        printf("No tokens to process.\n");
        return;
    }
    // Convertir les tokens en tableau de chaînes
    int count = 0;
    t_token *current = tokens;
    while (current) {
        count++;
        current = current->next;
    }
    // Afficher le nombre de tokens
    printf("Number of tokens: %d\n", count);
    // Allouer un tableau pour les arguments
    char **args = malloc((count + 1) * sizeof(char *));
    if (!args) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    current = tokens;
    for (int i = 0; i < count; i++) {
        args[i] = strdup(current->value);  // Copier la valeur du token
        current = current->next;
    }
    args[count] = NULL;  // Le dernier élément doit être NULL
    // Afficher les arguments pour débogage
    for (int i = 0; i < count; i++) {
        printf("args[%d]: %s\n", i, args[i]);
    }
    // Créer un processus fils pour exécuter la commande
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        free(args);
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {  // Processus fils
        // Exécuter la commande
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            free(args);
            exit(EXIT_FAILURE);
        }
    } else {  // Processus père
        int status;
        waitpid(pid, &status, 0);  // Attendre que le fils se termine

        // Afficher le statut de la commande
        if (WIFEXITED(status)) {
            printf("Command exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Command terminated abnormally\n");
        }
    }
    // Libérer la mémoire
    for (int i = 0; i < count; i++) {
        free(args[i]);
    }
    free(args);
}

void loop(void)
{
    char *input;
    t_token *tokens;

    while (1) {
        input = readline(PROMPT);
        if (!input) {
            printf("exit\n");
            break;
        }
        if (*input) {
            add_history(input);
        }

        // Commande pour activer l'affichage des tokens
        if (strcmp(input, "test") == 0) {
            printf("Enter command to test lexer (or type 'exit' to return to prompt):\n");
            while (1) {
                char *test_input = readline("Test> ");
                if (!test_input || strcmp(test_input, "exit") == 0) {
                    free(test_input);
                    break;
                }

                tokens = lexer(test_input);
                if (!tokens) {
                    printf("Lexer error: invalid syntax\n");
                } else {
                    print_tokens(tokens);
                    free_tokens(tokens);
                }
                free(test_input);
            }
        } else {
            // Tokenisation de l'entrée normale
            tokens = lexer(input);
            if (!tokens) {
                free(input);
                continue;
            }

            // Traitement des tokens (à implémenter)
            process_tokens(tokens);
            free_tokens(tokens);
        }

        free(input);
    }
}
