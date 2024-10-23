/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:37:16 by fzayani           #+#    #+#             */
/*   Updated: 2024/10/23 16:17:46 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
        while (cmd)
		{
            printf("%s ", cmd->value);
            cmd = cmd->next;
        }
        printf("\n");
    }
    for (int j = 0; j <= num_pipes; j++)
	{
        if (j < num_pipes)
		{
            if (pipe(pipe_fd) == -1)
			{
                perror("Échec de pipe");
                exit(EXIT_FAILURE);
            }
        }
        if ((pid = fork()) == -1)
		{
            perror("Échec de fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)// Processus enfant
        {
            if (fd_in != 0) {
                dup2(fd_in, STDIN_FILENO); // Rediriger l'entrée
            }
            if (j < num_pipes) {
                dup2(pipe_fd[1], STDOUT_FILENO); // Rediriger la sortie vers le pipe
            }
            close(pipe_fd[1]); // Fermer le descripteur d'écriture du pipe
            close(pipe_fd[0]); // Fermer le descripteur de lecture du pipe
            t_token *cmd = &cmd_tokens[j];// Préparer les arguments pour la commande actuelle
            char **args = prepare_args(cmd);
            if (args == NULL || args[0] == NULL) {
                fprintf(stderr, "Erreur lors de la préparation des arguments pour la commande\n");
                exit(EXIT_FAILURE);
            }
            printf("Exécution de la commande : %s\n", args[0]);// Debug: Afficher les arguments
            for (int k = 0; args[k] != NULL; k++) {
                printf("Argument %d : %s\n", k, args[k]);
            }
            if (execvp(args[0], args) == -1) {
                perror("Échec de exec");
                free(args); // Libérer la mémoire avant de sortir
                exit(EXIT_FAILURE);
            }
        }
        else// Processus parent
        {
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
	int pid;
	int status;
    t_token *tokens;

	status = 0;
    while (1)
    {
        line = readline(PROMPT); // lire une ligne de commande
        if (line == NULL) // Gestion du Ctrl+D
        {
            write(1, "exit\n", 5); // Afficher un message de sortie propre
            break; // Quitter la boucle
        }
        if (*line) // Si la ligne n'est pas vide
        {
            add_history(line);
            tokens = parse_command_line(line);
            if (tokens)
            {
                if (check_consecutive_pipes(tokens) == -1)
                {
                    free_tokens(tokens);
                    continue;
                }
                print_tokens(tokens);
                if (contains_pipe(tokens))// Procesus de commande avec pipes ou exécution simple
                    process_pline(tokens, env);
                else
                {
                    // exec(tokens, env);
                    pid = fork(); // tous les autres process tel que exec sont des enfants
                    // fprintf(stderr, "test666");
                    if (pid == 0)
                    {
                        exec(tokens, env);
                        exit(0);
                    }
                    waitpid(pid, &status, 0);
                }
                free_tokens(tokens);
            }
        }
        free(line); // Libérer la ligne après usage
    }
}
