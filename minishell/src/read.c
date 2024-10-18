/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:37:16 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/18 14:46:58 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void process_tokens(t_token *tokens)
// {
//     t_token *current = tokens;
//     int token_count = 0;
//     pid_t pid;
//     int status;

//     int saved_stdout = dup(STDOUT_FILENO); // Sauvegarder la sortie standard
//     int saved_stdin = dup(STDIN_FILENO);   // Sauvegarder l'entrée standard
//     while (current)// Compter le nombre de tokens pour allouer dynamiquement `args`
//     {
//         token_count++;
//         current = current->next;
//     }
//     char **args = malloc((token_count + 1) * sizeof(char *)); // Allouer dynamiquement un tableau pour les arguments
//     if (!args) {
//         perror("malloc failed");
//         return;
//     }
//     current = tokens; // Réinitialiser `current` pour commencer à ajouter des arguments
//     int i = 0;
//     while (current) // Préparer les arguments et gérer les redirections
//     {
//         if (current->type == TOKEN_COMMAND) {
//             args[i++] = current->value; // Ajouter la commande/argument
//         }
//         else if (current->type == TOKEN_REDIRECT_OUTPUT || current->type == TOKEN_REDIRECT_APPEND)
//         {
//             current = current->next; // Vérifier s'il y a un token suivant
//             if (!current || current->type != TOKEN_FILENAME) {
//                 fprintf(stderr, "bash: syntax error near unexpected token `newline'\n");
//                 free(args);
//                 return;
//             }
//             int flags = O_WRONLY | O_CREAT | (current->type == TOKEN_REDIRECT_OUTPUT ? O_TRUNC : O_APPEND);
//             int fd = open(current->value, flags, 0644);
//             if (fd == -1) {
//                 perror("open failed");
//                 free(args);
//                 return;
//             }
//             dup2(fd, STDOUT_FILENO); // Rediriger la sortie standard vers le fichier
//             close(fd);
//         }
//         else if (current->type == TOKEN_REDIRECT_INPUT)
//         {
//             current = current->next; // Vérifier s'il y a un token suivant
//             if (!current || current->type != TOKEN_FILENAME) {
//                 fprintf(stderr, "bash: syntax error near unexpected token `newline'\n");
//                 free(args);
//                 return;
//             }
//             int fd = open(current->value, O_RDONLY);
//             if (fd == -1) {
//                 perror("open failed");
//                 free(args);
//                 return;
//             }
//             dup2(fd, STDIN_FILENO); // Rediriger l'entrée standard
//             close(fd);
//         }
//         else if (current->type == TOKEN_REDIRECT_HEREDOC)
//         {
//             current = current->next; // Vérifier s'il y a un token suivant (le délimiteur du heredoc)
//             if (!current || current->type != TOKEN_FILENAME) {
//                 fprintf(stderr, "bash: syntax error near unexpected token `newline'\n");
//                 free(args);
//                 return;
//             }
//             char *delimiter = current->value;// Ici, on gère le heredoc
//             char *line = NULL;
//             size_t len = 0;
//             FILE *heredoc_file = tmpfile(); // Créer un fichier temporaire pour stocker le heredoc
//             if (!heredoc_file) {
//                 perror("tmpfile failed");
//                 free(args);
//                 return;
//             }
//             printf("> "); // Afficher le prompt pour le heredoc
//             while (getline(&line, &len, stdin) != -1) {
//                 if (strcmp(line, delimiter) == 0) {
//                     break; // Sortir si on rencontre le délimiteur
//                 }
//                 fprintf(heredoc_file, "%s", line); // Écrire la ligne dans le fichier temporaire
//                 printf("> ");
//             }
//             free(line);
//             rewind(heredoc_file); // Revenir au début du fichier temporaire
//             int fd = fileno(heredoc_file); // Obtenir le descripteur de fichier
//             dup2(fd, STDIN_FILENO); // Rediriger l'entrée standard vers le fichier temporaire
//             // Ne pas fermer fd ici, car il est maintenant utilisé comme stdin
//         }
//         current = current->next;
//     }
//     args[i] = NULL; // Terminer le tableau d'arguments avec NULL
//     pid = fork(); // Créer un processus enfant pour exécuter la commande
//     if (pid == -1) {
//         perror("fork failed");
//         free(args);  // Libérer la mémoire
//         return;
//     }
//     else if (pid == 0)
//     {
//         if (execvp(args[0], args) == -1) // Code du processus enfant : exécuter la commande
//         {
//             perror("execvp failed");
//             free(args);  // Libérer la mémoire avant de quitter
//             exit(EXIT_FAILURE); // Si l'exécution échoue, quitter avec une erreur
//         }
//     }
//     else
//     {
//         waitpid(pid, &status, 0); // Code du processus parent : attendre que le processus enfant termine
//         printf("Command exited with status %d\n", WEXITSTATUS(status));
//     }
//     dup2(saved_stdout, STDOUT_FILENO); // Restaurer la sortie standard et l'entrée standard
//     dup2(saved_stdin, STDIN_FILENO);
//     close(saved_stdout); // Fermer les descripteurs de fichiers sauvegardés
//     close(saved_stdin);
//     free(args); // Libérer la mémoire allouée pour `args`
// }

// char **prepare_args(t_token *tokens)
// {
//     int count = 0;
//     t_token *current = tokens;

//     // Compter les tokens
//     while (current && current->type != TOKEN_PIPE)
//     {
//         count++;
//         current = current->next;
//     }
//     // Allouer le tableau d'arguments
//     char **args = malloc((count + 1) * sizeof(char *));
//     if (!args) {
//         perror("malloc failed");
//         return NULL;
//     }
//     // Remplir le tableau d'arguments
//     current = tokens;
//     int i = 0;
//     while (current && current->type != TOKEN_PIPE) {
//         args[i++] = current->value;
//         current = current->next;
//     }
//     args[i] = NULL; // Terminer par NULL
//     return args;
// }

// void process_tokens(t_token *tokens)
// {
//     t_token *current = tokens;
//     int saved_stdout = dup(STDOUT_FILENO); // Sauvegarder la sortie standard
//     int saved_stdin = dup(STDIN_FILENO);   // Sauvegarder l'entrée standard

//     int num_pipes = 0; // Pour compter les pipes dans les tokens
//     t_token *token_list[256]; // Pour stocker les sous-commandes

//     // Séparer les commandes par pipe
//     t_token *start = current;
//     int i = 0;

//     while (current) {
//         if (current->type == TOKEN_PIPE) {
//             token_list[i++] = start;
//             token_list[i++] = NULL; // Marquer la fin de la commande actuelle
//             start = current->next;  // La commande suivante commence après le pipe
//             num_pipes++;
//         }
//         current = current->next;
//     }
//     token_list[i] = start; // Ajouter la dernière commande après le dernier pipe
//     token_list[i + 1] = NULL; // Marquer la fin de la dernière commande

//     // Gérer les pipes
//     int pipefd[2];
//     int fd_in = 0;
//     pid_t pid;
//     int status;

//     for (int j = 0; j <= num_pipes; j++) {
//         if (pipe(pipefd) == -1) {
//             perror("pipe failed");
//             exit(EXIT_FAILURE);
//         }
//         if ((pid = fork()) == -1) {
//             perror("fork failed");
//             exit(EXIT_FAILURE);
//         }
//         if (pid == 0) { // Processus enfant
//             dup2(fd_in, STDIN_FILENO); // Rediriger l'entrée
//             if (j != num_pipes) { // Si ce n'est pas la dernière commande
//                 dup2(pipefd[1], STDOUT_FILENO); // Rediriger la sortie vers le pipe
//             }
//             close(pipefd[0]);
//             close(pipefd[1]);

//             // Préparer les arguments pour la commande actuelle
//             t_token *cmd_tokens = token_list[j];
//             char **args = prepare_args_p(cmd_tokens); // Convertir la liste de tokens en tableau d'arguments

//             if (execvp(args[0], args) == -1) {
//                 perror("execvp failed");
//                 free(args);
//                 exit(EXIT_FAILURE);
//             }
//         } else { // Processus parent
//             waitpid(pid, &status, 0);
//             close(pipefd[1]); // Le parent ne doit pas écrire dans le pipe
//             fd_in = pipefd[0]; // Le prochain processus lira à partir de ce pipe
//         }
//     }

//     dup2(saved_stdout, STDOUT_FILENO); // Restaurer la sortie standard et l'entrée standard
//     dup2(saved_stdin, STDIN_FILENO);
//     close(saved_stdout); // Fermer les descripteurs de fichiers sauvegardés
//     close(saved_stdin);
// }



void process_tokens(t_token *cmd_tokens, int num_pipes)
{
    int pipe_fd[2];
    int fd_in = 0; // Descripteur d'entrée pour les pipes
    pid_t pid;

    // Debug: Affichage des commandes et du nombre de pipes
    printf("Number of pipes: %d\n", num_pipes);
    for (int k = 0; k <= num_pipes; k++) {
        t_token *cmd = &cmd_tokens[k];
        printf("Command %d: ", k);
        while (cmd) {
            printf("%s ", cmd->value);
            cmd = cmd->next;
        }
        printf("\n");
    }

    for (int j = 0; j <= num_pipes; j++) {
        if (j < num_pipes) {
            if (pipe(pipe_fd) == -1) {
                perror("Échec de pipe");
                exit(EXIT_FAILURE);
            }
        }
        if ((pid = fork()) == -1) {
            perror("Échec de fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) { // Processus enfant
            if (fd_in != 0) {
                dup2(fd_in, STDIN_FILENO); // Rediriger l'entrée
            }
            if (j < num_pipes) {
                dup2(pipe_fd[1], STDOUT_FILENO); // Rediriger la sortie vers le pipe
            }
            close(pipe_fd[1]); // Fermer le descripteur d'écriture du pipe
            close(pipe_fd[0]); // Fermer le descripteur de lecture du pipe

            // Préparer les arguments pour la commande actuelle
            t_token *cmd = &cmd_tokens[j];
            char **args = prepare_args(cmd);
            if (args == NULL || args[0] == NULL) {
                fprintf(stderr, "Erreur lors de la préparation des arguments pour la commande\n");
                exit(EXIT_FAILURE);
            }

            // Debug: Afficher les arguments
            printf("Exécution de la commande : %s\n", args[0]);
            for (int k = 0; args[k] != NULL; k++) {
                printf("Argument %d : %s\n", k, args[k]);
            }
            if (execvp(args[0], args) == -1) {
                perror("Échec de exec");
                free(args); // Libérer la mémoire avant de sortir
                exit(EXIT_FAILURE);
            }
        } else { // Processus parent
            close(pipe_fd[1]); // Fermer le descripteur d'écriture du pipe dans le parent
            waitpid(pid, NULL, 0); // Attendre que l'enfant termine
            if (fd_in != 0) {
                close(fd_in); // Fermer l'ancien pipe d'entrée
            }
            fd_in = pipe_fd[0]; // Le prochain processus lira à partir de ce pipe
        }
    }
}

void loop(char **env)
{
    char *line;
    t_token *tokens;

    while (1)
    {
        // Afficher le prompt
        printf("MiniBG🌝> ");
        line = readline(NULL); // lire une ligne de commande

        if (line && *line) // Si la ligne n'est pas vide
        {
            tokens = parse_command_line(line);
            if (tokens)
            {
                if (check_consecutive_pipes(tokens) == -1)
                {
                    free_tokens(tokens);
                    continue;
                }
                print_tokens(tokens);
                // Procesus de commande avec pipes ou exécution simple
                if (contains_pipe(tokens))
                    process_pline(tokens, env);
                else
                    exec(tokens, env);
                free_tokens(tokens);
            }
        }
        free(line); // Libérer la ligne après usage
    }
}

// void loop(void)
// {
//     char *input;
//     t_token *tokens;

//     while (1)
//     {
//         input = readline(PROMPT); // Affiche le prompt et lit l'entrée
//         if (!input) {
//             printf("exit\n");
//             break; // Quitte la boucle si l'entrée est nulle (CTRL+D)
//         }
//         if (*input)
//             add_history(input); // Ajoute l'entrée à l'historique
//         if (strcmp(input, "test") == 0) // Mode test du lexer
//         {
//             printf("Enter command to test lexer (or type 'exit' to return to prompt):\n");
//             while (1)
//             {
//                 char *test_input = readline("Test> ");
//                 if (!test_input || strcmp(test_input, "exit") == 0)
//                 {
//                     free(test_input);
//                     break; // Quitte le mode test si 'exit' est tapé
//                 }
//                 tokens = lexer(test_input); // Analyse les tokens
//                 if (!tokens)
//                     printf("Lexer error: invalid syntax\n");
//                 else
//                 {
//                     print_tokens(tokens); // Affiche les tokens
//                     free_tokens(tokens); // Libère les tokens
//                 }
//                 free(test_input);
//             }
//         }
//         else // Traitement des commandes normales
//         {
//             tokens = lexer(input); // Analyse les tokens de l'entrée
//             if (!tokens) {
//                 free(input);
//                 continue; // Ignore si l'analyse échoue
//             }

//             // Compte le nombre de pipes dans l'entrée
//             int num_pipes = 0;
//             t_token *current = tokens;
//             while (current) {
//                 if (strcmp(current->value, "|") == 0) {
//                     num_pipes++;
//                 }
//                 current = current->next;
//             }

//             process_tokens(tokens, num_pipes); // Traite les tokens avec le nombre de pipes
//             free_tokens(tokens); // Libère les tokens
//         }
//         free(input); // Libère la mémoire de l'entrée
//     }
// }
