/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 11:12:22 by fzayani           #+#    #+#             */
/*   Updated: 2024/11/23 12:15:00 by fzayani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/stat.h>



extern char				**environ;

# define PROMPT "\033[1;34mMiniBG>\033[0m "

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef struct s_env_var
{
	char				*name;
	char				*value;
	struct s_env_var	*next;
}						t_env_var;
typedef struct s_ctx
{
    t_env_var *env_vars;  // Liste des variables d'environnement
	unsigned char exit_status;
	int	num_pipes;
	char *oldpwd;
	char *pwd;
} t_ctx;

typedef struct s_var
{
    char **env;
	t_env_var *env_vars;
	struct s_env_var	*next;
	struct s_env_var	*name;
	struct s_env_var	*value;

} t_var;

typedef enum token_type
{
	TOKEN_COMMAND,
	TOKEN_ARGUMENT,
	TOKEN_HEREDOC,
	TOKEN_REDIRECT_INPUT,  // <
	TOKEN_REDIRECT_OUTPUT, // >
	TOKEN_REDIRECT_APPEND, // >>
	TOKEN_PIPE,            // |
	TOKEN_FILENAME,	// Ajoute ce type pour les fichiers après redirection
	TOKEN_REDIRECT_HEREDOC, // <<
	TOKEN_END,
	STRING,
	SINGLE_QUOTE,
	DOUBLEQUOTE
}						t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	struct s_token		*next;
}						t_token;


t_ctx *initialize_ctx(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	init_sig(void);
int main(int argc __attribute__((unused)), char **argv __attribute__((unused)),
         char **envp);
char	**get_environment(char **envp);
t_env_var *build_env_vars(char **envp);
int	loop(t_var *myEnv);
int read_and_exec(t_var *myEnv);
int handle_line(char *line, t_var *myEnv, t_ctx *ctx);
t_token *parse_command_line(char *line, t_ctx *ctx);



#endif