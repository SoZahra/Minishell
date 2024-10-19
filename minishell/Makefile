# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fzayani <fzayani@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/03 11:43:58 by fzayani           #+#    #+#              #
#    Updated: 2024/10/18 11:42:07 by fzayani          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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

all:	$(NAME)

$(NAME):	$(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJS) $(LFLAGS) -o $(NAME)
	@echo "[DONE] Minishell compiled!"

$(OBJS_PATH)%.o:	$(SRCS_PATH)%.c
	mkdir -p $(OBJS_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_LIB):
	@make -C $(LIBFT_PATH) --silent

clean:
	$(RM) $(OBJS_PATH)
	@make clean -C $(LIBFT_PATH)
	@echo "[DONE] Objects files deleted..."

fclean:	clean
	$(RM) $(NAME)
	@make fclean -C $(LIBFT_PATH)
	@echo "[DONE] Minishell deleted..."

re:	fclean all
	@echo "[DONE] Minishell recompiled..."

.PHONY:	all clean fclean re
