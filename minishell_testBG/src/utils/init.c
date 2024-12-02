/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 15:21:14 by fzayani           #+#    #+#             */
/*   Updated: 2024/12/02 15:21:26 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int initialize_ctx(t_ctx *ctx)
{
    if (!ctx)
    {
        perror("Failed to allocate memory for t_ctx");
        return 1;
    }// Initialiser les champs de la structure
    ctx->env_vars = NULL;           // Liste des variables d'environnement initialement vide
    ctx->exit_status = 0;           // Le code de sortie est initialisé à 0 (succès)
    ctx->num_pipes = 0;             // Initialiser le nombre de pipes à 0
    ctx->oldpwd = NULL;             // Initialiser l'ancien répertoire à NULL
    ctx->pwd = getcwd(NULL, 0);     // Obtenir le répertoire actuel
    if (!ctx->pwd)
    {
        perror("Failed to get current working directory");
        free(ctx);                  // Libérer la mémoire si une erreur survient
        return 1;
    }

    return 0;
}