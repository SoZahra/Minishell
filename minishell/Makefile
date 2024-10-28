# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/03 11:43:58 by fzayani           #+#    #+#              #
#    Updated: 2024/10/28 14:51:13 by fzayani          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# NAME		=	minishell

# SRCS_FILES	=	main.c read.c signal.c parsing.c lexer.c lexer_utils.c pipe.c pipe_utils.c\
# 				free.c pipe_utils_2.c env.c

# SRCS_PATH	=	src/
# OBJS_PATH	=	.objs/
# INC_PATH	=	include/

# ENV_DIR = $(SRC_PATH)/env
# LEXER_DIR = $(SRC_PATH)/lexer
# LOOP_DIR = $(SRC_PATH)/loop
# PIPE_DIR = $(SRC_PATH)/pipe
# SIGNALS_DIR = $(SRC_PATH)/signals
# UTILS_DIR = $(SRC_PATH)/utils

# LIBFT_PATH	=	./libft/
# LIBFT_LIB	=	$(LIBFT_PATH)libft.a

# SRCS		=	$(addprefix $(SRCS_PATH), $(SRCS_FILES)) \
# 				$(addprefix $(ENV_DIR)/, $(ENV_FILES)) \
#         		$(addprefix $(LEXER_DIR)/, $(LEXER_FILES)) \
# 				$(addprefix $(LOOP_DIR)/, $(LOOP_FILES)) \
# 				$(addprefix $(SIGNALS_DIR)/, $(SIGNALS_FILES)) \
# 				$(addprefix $(UTILS_DIR)/, $(UTILS_FILES)) \
#         		$(addprefix $(PIPE_DIR)/, $(PIPE_FILES))

# OBJS		=	$(addprefix $(OBJS_PATH), $(SRCS:$(SRCS_PATH)%.c=%.o))

# CC			=	cc
# CFLAGS		=	-Wall -Werror -Wextra -I$(INC_PATH) -I$(LIBFT_PATH) -g3
# LFLAGS		=	-L$(LIBFT_PATH) -lft -lreadline
# RM			=	rm -rf

# # Colors
# GREEN		=	\033[32m
# YELLOW		=	\033[33m
# RED			=	\033[31m
# RESET		=	\033[0m

# all:	$(NAME)

# $(NAME):	$(OBJS) $(LIBFT_LIB)
# 	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(NAME)
# 	@echo "$(GREEN)"
# 	@echo "    __  __ _       _     _          _ _"
# 	@echo "   |  \\/  (_)     (_)   | |        | | |"
# 	@echo "   | \\  / |_ _ __  _ ___| |__   ___| | |"
# 	@echo "   | |\\/| | | '_ \\| / __| '_ \\ / _ \\ | |"
# 	@echo "   | |  | | | | | | \\__ \\ | | |  __/ | |"
# 	@echo "   |_|  |_|_|_| |_|_|___/_| |_|\\___|_|_|"
# 	@echo "                                        "
# 	@echo "                                        "
# 	@echo "$(RESET)"
# 	@echo "$(GREEN)[DONE] Minishell compiled!$(RESET)"

# $(OBJS_PATH)%.o:	$(SRCS_PATH)%.c
# 	mkdir -p $(OBJS_PATH)
# 	$(CC) $(CFLAGS) -c $< -o $@
# 	@echo "$(YELLOW)Compiling: $<$(RESET)"

# $(LIBFT_LIB):
# 	@make -C $(LIBFT_PATH) --silent

# clean:
# 	$(RM) $(OBJS_PATH)
# 	@make clean -C $(LIBFT_PATH)
# 	@echo "$(RED)[DONE] Objects files deleted...$(RESET)"

# fclean:	clean
# 	$(RM) $(NAME)
# 	@make fclean -C $(LIBFT_PATH)
# 	@echo "$(RED)[DONE] Minishell deleted...$(RESET)"

# re:	fclean all
# 	@echo "$(GREEN)[DONE] Minishell recompiled!$(RESET)"

# .PHONY:	all clean fclean re


NAME		=	minishell

# Fichiers sources organisés par répertoires
SRC_PATH	=	src/
ENV_DIR		=	$(SRC_PATH)env
LOOP_DIR	=	$(SRC_PATH)loop
LEXER_DIR	=	$(SRC_PATH)lexer
PIPE_DIR	=	$(SRC_PATH)pipe
SIGNALS_DIR	=	$(SRC_PATH)signals
UTILS_DIR	=	$(SRC_PATH)utils

# Liste des fichiers source
SRCS_FILES	=	main.c \
				env/env.c env/utils_env.c\
				loop/parsing.c loop/read.c loop/handle.c loop/read_utils.c\
				lexer/lexer.c lexer/lexer_utils.c lexer/handle.c lexer/tokens.c\
				pipe/exc_pipe.c pipe/pipe.c pipe/pipe_utils.c pipe/pipe_utils_2.c\
				signals/signal.c \
				utils/error.c utils/free.c

SRCS		=	$(addprefix $(SRC_PATH), $(SRCS_FILES))

# Fichiers objets générés dans le dossier .objs/
OBJS_PATH	=	.objs/
OBJS		=	$(SRCS:$(SRC_PATH)%.c=$(OBJS_PATH)%.o)

# Include et libft
INC_PATH	=	include/
LIBFT_PATH	=	./libft/
LIBFT_LIB	=	$(LIBFT_PATH)libft.a

# Compilation
CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra -I$(INC_PATH) -I$(LIBFT_PATH) -g3
LFLAGS		=	-L$(LIBFT_PATH) -lft -lreadline
RM			=	rm -rf

# Couleurs
GREEN		=	\033[32m
YELLOW		=	\033[33m
RED			=	\033[31m
RESET		=	\033[0m

# Règle principale
all:	$(NAME)

$(NAME):	$(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(NAME)
	@echo "$(GREEN)[DONE] Minishell compiled!$(RESET)"

# Compilation des fichiers .c en .o dans .objs/
$(OBJS_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(dir $@) # Crée le sous-dossier si nécessaire
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLOW)Compiling: $<$(RESET)"

# Compilation de la libft
$(LIBFT_LIB):
	@make -C $(LIBFT_PATH) --silent

# Nettoyage des objets
clean:
	$(RM) $(OBJS_PATH)
	@make clean -C $(LIBFT_PATH)
	@echo "$(RED)[DONE] Objects files deleted...$(RESET)"

# Nettoyage complet
fclean: clean
	$(RM) $(NAME)
	@make fclean -C $(LIBFT_PATH)
	@echo "$(RED)[DONE] Minishell deleted...$(RESET)"

# Recompilation complète
re: fclean all
	@echo "$(GREEN)[DONE] Minishell recompiled!$(RESET)"

.PHONY: all clean fclean re

