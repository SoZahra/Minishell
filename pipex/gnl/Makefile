# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llarrey <llarrey@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/27 09:26:52 by llarrey           #+#    #+#              #
#    Updated: 2024/09/18 14:46:10 by llarrey          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = gnl.a

SRCS = get_next_line_bonus.c\
		get_next_line_utils_bonus.c		

OBJS			= $(SRCS:.c=.o)

CC				= cc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror -fPIC

all:			$(NAME)

$(NAME):		$(OBJS)
				ar rcs $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re 