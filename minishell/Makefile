# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/03 11:43:58 by fzayani           #+#    #+#              #
#    Updated: 2024/10/23 11:33:57 by fzayani          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# NAME		=	minishell

# SRCS_FILES	=	main.c read.c signal.c parsing.c lexer.c lexer_utils.c pipe.c pipe_utils.c\
# 				free.c pipe_utils_2.c env.c

# SRCS_PATH	=	src/
# OBJS_PATH	=	.objs/
# INC_PATH	=	include/

# LIBFT_PATH	=	./libft/
# LIBFT_LIB	=	$(LIBFT_PATH)libft.a

# SRCS		=	$(addprefix $(SRCS_PATH), $(SRCS_FILES))
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

SRCS_FILES	=	main.c read.c signal.c parsing.c lexer.c lexer_utils.c pipe.c pipe_utils.c\
				free.c pipe_utils_2.c env.c

SRCS_PATH	=	src/
OBJS_PATH	=	.objs/
INC_PATH	=	include/

LIBFT_PATH	=	./libft/
LIBFT_LIB	=	$(LIBFT_PATH)libft.a

SRCS		=	$(addprefix $(SRCS_PATH), $(SRCS_FILES))
OBJS		=	$(addprefix $(OBJS_PATH), $(SRCS:$(SRCS_PATH)%.c=%.o))

CC			=	cc
CFLAGS		=	-Wall -Werror -Wextra -I$(INC_PATH) -I$(LIBFT_PATH) -g3
LFLAGS		=	-L$(LIBFT_PATH) -lft -lreadline
RM			=	rm -rf

# Colors
GREEN		=	\033[32m
YELLOW		=	\033[33m
RED			=	\033[31m
RESET		=	\033[0m

all:	$(NAME)

$(NAME):	$(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(NAME)
	@echo "$(GREEN)"
	@echo "    __  __ _       _     _          _ _"
	@echo "   |  \\/  (_)     (_)   | |        | | |"
	@echo "   | \\  / |_ _ __  _ ___| |__   ___| | |"
	@echo "   | |\\/| | | '_ \\| / __| '_ \\ / _ \\ | |"
	@echo "   | |  | | | | | | \\__ \\ | | |  __/ | |"
	@echo "   |_|  |_|_|_| |_|_|___/_| |_|\\___|_|_|"
	@echo "                                        "
	@echo "                                        "
	@echo "$(RESET)"
	@echo "$(GREEN)[DONE] Minishell compiled!$(RESET)"

$(OBJS_PATH)%.o:	$(SRCS_PATH)%.c
	mkdir -p $(OBJS_PATH)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(YELLOW)Compiling: $<$(RESET)"

$(LIBFT_LIB):
	@make -C $(LIBFT_PATH) --silent

clean:
	$(RM) $(OBJS_PATH)
	@make clean -C $(LIBFT_PATH)
	@echo "$(RED)[DONE] Objects files deleted...$(RESET)"

fclean:	clean
	$(RM) $(NAME)
	@make fclean -C $(LIBFT_PATH)
	@echo "$(RED)[DONE] Minishell deleted...$(RESET)"

re:	fclean all
	@echo "$(GREEN)[DONE] Minishell recompiled!$(RESET)"

.PHONY:	all clean fclean re

