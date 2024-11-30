# NAME		=	minishell

# # Fichiers sources organisés par répertoires
# SRC_PATH	=	src/
# ENV_DIR		=	$(SRC_PATH)env
# LOOP_DIR	=	$(SRC_PATH)loop
# LEXER_DIR	=	$(SRC_PATH)lexer
# PIPE_DIR	=	$(SRC_PATH)pipe
# SIGNALS_DIR	=	$(SRC_PATH)signals
# UTILS_DIR	=	$(SRC_PATH)utils

# # Liste des fichiers source
# SRCS_FILES	=	main.c \
# 				# env/env.c env/utils_env.c env/expand.c\
# 				# loop/parsing.c loop/read.c loop/handle.c loop/read_utils.c\
# 				# lexer/lexer.c lexer/lexer_utils.c lexer/handle.c lexer/tokens.c\
# 				# pipe/exc_pipe.c pipe/pipe.c pipe/pipe_utils.c pipe/pipe_utils_2.c\
# 				# signals/signal.c \
# 				# utils/error.c utils/free.c

# SRCS		=	$(addprefix $(SRC_PATH), $(SRCS_FILES))

# # Fichiers objets générés dans le dossier .objs/
# OBJS_PATH	=	.objs/
# OBJS		=	$(SRCS:$(SRC_PATH)%.c=$(OBJS_PATH)%.o)

# # Include et libft
# INC_PATH	=	include/
# LIBFT_PATH	=	./libft/
# LIBFT_LIB	=	$(LIBFT_PATH)libft.a

# # Compilation
# CC			=	cc
# CFLAGS		=	-Wall -Werror -Wextra -I$(INC_PATH) -I$(LIBFT_PATH) -g3
# LFLAGS		=	-L$(LIBFT_PATH) -lft -lreadline
# RM			=	rm -rf

# # Couleurs
# GREEN		=	\033[32m
# YELLOW		=	\033[33m
# RED			=	\033[31m
# RESET		=	\033[0m

# # Règle principale
# all:	$(NAME)

# $(NAME):	$(OBJS) $(LIBFT_LIB)
# 	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(NAME)
# 	@echo "$(GREEN)[DONE] Minishell compiled!$(RESET)"

# # Compilation des fichiers .c en .o dans .objs/
# $(OBJS_PATH)%.o: $(SRC_PATH)%.c
# 	@mkdir -p $(dir $@) # Crée le sous-dossier si nécessaire
# 	$(CC) $(CFLAGS) -c $< -o $@
# 	@echo "$(YELLOW)Compiling: $<$(RESET)"

# # Compilation de la libft
# $(LIBFT_LIB):
# 	@make -C $(LIBFT_PATH) --silent

# # Nettoyage des objets
# clean:
# 	$(RM) $(OBJS_PATH)
# 	@make clean -C $(LIBFT_PATH)
# 	@echo "$(RED)[DONE] Objects files deleted...$(RESET)"

# # Nettoyage complet
# fclean: clean
# 	$(RM) $(NAME)
# 	@make fclean -C $(LIBFT_PATH)
# 	@echo "$(RED)[DONE] Minishell deleted...$(RESET)"

# # Recompilation complète
# re: fclean all
# 	@echo "$(GREEN)[DONE] Minishell recompiled!$(RESET)"

# .PHONY: all clean fclean re

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
SRCS_FILES	=	test.c \
				# env/env.c env/utils_env.c env/expand.c\
				# loop/parsing.c loop/read.c loop/handle.c loop/read_utils.c\
				# lexer/lexer.c lexer/lexer_utils.c lexer/handle.c lexer/tokens.c\
				# pipe/exc_pipe.c pipe/pipe.c pipe/pipe_utils.c pipe/pipe_utils_2.c\
				# signals/signal.c \
				# utils/error.c utils/free.c

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
CFLAGS 		+= -I/opt/homebrew/opt/readline/include
LFLAGS		=	-L$(LIBFT_PATH) -lft -lreadline
LFLAGS		+= -L/opt/homebrew/opt/readline/lib -lreadline
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
