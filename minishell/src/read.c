/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:37:16 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/11 18:13:12 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void process_tokens(t_token *tokens) {
    t_token *current = tokens;
    int token_count = 0;
    pid_t pid;
    int status;

    int saved_stdout = dup(STDOUT_FILENO); // Sauvegarder la sortie standard
    int saved_stdin = dup(STDIN_FILENO);   // Sauvegarder l'entrée standard

    // Compter le nombre de tokens pour allouer dynamiquement `args`
    while (current) {
        token_count++;
        current = current->next;
    }

    // Allouer dynamiquement un tableau pour les arguments
    char **args = malloc((token_count + 1) * sizeof(char *));
    if (!args) {
        perror("malloc failed");
        return;
    }

    // Réinitialiser `current` pour commencer à ajouter des arguments
    current = tokens;
    int i = 0;
    while (current) // Préparer les arguments et gérer les redirections
    {
        if (current->type == TOKEN_COMMAND) {
            args[i++] = current->value; // Ajouter la commande/argument
        } else if (current->type == TOKEN_REDIRECT_OUTPUT)
        {
            current = current->next; // Vérifier s'il y a un token suivant
            if (!current || current->type != TOKEN_FILENAME) {
                fprintf(stderr, "syntax error near unexpected token `newline'\n");
                break;
            }
            int fd = open(current->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("open failed");
                free(args);
                return;
            }
            dup2(fd, STDOUT_FILENO); // Rediriger la sortie standard vers le fichier
            close(fd);
        }
        else if (current->type == TOKEN_REDIRECT_INPUT)
        {
            current = current->next;// Vérifier s'il y a un token suivant
            if (!current || current->type != TOKEN_FILENAME) {
                fprintf(stderr, "syntax error near unexpected token `newline'\n");
                break;
            }
            int fd = open(current->value, O_RDONLY);
            if (fd == -1) {
                perror("open failed");
                free(args);
                return;
            }
            dup2(fd, STDIN_FILENO); // Rediriger l'entrée standard
            close(fd);
        }
        else if (current->type == TOKEN_REDIRECT_APPEND)
        {
            // Vérifier s'il y a un token suivant
            current = current->next;
            if (!current || current->type != TOKEN_FILENAME)
            {
                fprintf(stderr, "syntax error near unexpected token `newline'\n");
                break;
            }
            int fd = open(current->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                perror("open failed");
                free(args);
                return;
            }
            dup2(fd, STDOUT_FILENO); // Rediriger la sortie en mode append
            close(fd);
        } else if (current->type == TOKEN_REDIRECT_HEREDOC) {
            // Vérifier s'il y a un token suivant (le délimiteur du heredoc)
            current = current->next;
            if (!current || current->type != TOKEN_FILENAME) {
                fprintf(stderr, "syntax error near unexpected token `newline'\n");
                break;
            }
        }// Ici tu devrais gérer le << (heredoc) en fonction de ton implémentation
        current = current->next;
    }
    args[i] = NULL; // Terminer le tableau d'arguments avec NULL
    pid = fork();// Créer un processus enfant pour exécuter la commande
    if (pid == -1) {
        perror("fork failed");
        free(args);  // Libérer la mémoire
        return;
    }
    else if (pid == 0)
    {
        if (execvp(args[0], args) == -1)// Code du processus enfant : exécuter la commande
        {
            perror("execvp failed");
            free(args);  // Libérer la mémoire avant de quitter
            exit(EXIT_FAILURE); // Si l'exécution échoue, quitter avec une erreur
        }
    } else {
        // Code du processus parent : attendre que le processus enfant termine
        waitpid(pid, &status, 0);
        printf("Command exited with status %d\n", WEXITSTATUS(status));
    }

    // Restaurer la sortie standard et l'entrée standard
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);

    // Fermer les descripteurs de fichiers sauvegardés
    close(saved_stdout);
    close(saved_stdin);

    // Libérer la mémoire allouée pour `args`
    free(args);
}

void loop(void)
{
    char *input;
    t_token *tokens;

    while (1)
    {
        input = readline(PROMPT);
        if (!input) {
            printf("exit\n");
            break;
        }
        if (*input)
            add_history(input);
        if (strcmp(input, "test") == 0)// Commande pour activer l'affichage des tokens
        {
            printf("Enter command to test lexer (or type 'exit' to return to prompt):\n");
            while (1)
            {
                char *test_input = readline("Test> ");
                if (!test_input || strcmp(test_input, "exit") == 0)
                {
                    free(test_input);
                    break;
                }
                tokens = lexer(test_input);
                if (!tokens)
                    printf("Lexer error: invalid syntax\n");
                else
                {
                    print_tokens(tokens);
                    free_tokens(tokens);
                }
                free(test_input);
            }
        }
        else// Tokenisation de l'entrée normale
        {
            tokens = lexer(input);
            if (!tokens) {
                free(input);
                continue;
            }
            process_tokens(tokens);// Traitement des tokens (à implémenter)
            free_tokens(tokens);
        }
        free(input);
    }
}
