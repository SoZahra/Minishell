# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymanchon <ymanchon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 16:21:19 by ymanchon          #+#    #+#              #
#    Updated: 2024/12/28 13:35:08 by ymanchon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ################ #
#*    COULEURS    *#
# ################ #

CLASSIC	= \e[0m
WHITE	= \e[38;2;238;232;213m
BOLD	= \e[1m
# BLACK	= \e[38;2;0;0;0m
BLACK	= \e[30m
RED   	= \e[31m
GREEN  	= \e[32m
BLUE 	= \e[38;2;0;36;148m
PURPLE 	= \e[38;2;211;54;130m
CYAN   	= \e[36m
YELLOW 	= \e[38;2;250;189;0m

# ############### #
#*   VARIABLES   *#
# ############### #

NAME = minishell

CC = @cc

SRCS =	./src/builtins/cd.c					\
		./src/builtins/echo.c				\
		./src/builtins/env.c				\
		./src/builtins/exit.c				\
		./src/builtins/gestion_builtins.c	\
		./src/builtins/pwd.c				\
		./src/builtins/unset.c				\
		./src/builtins/utils_export.c		\
		\
		./src/env/build_env_list.c			\
		\
		./src/exec/exec_builtins.c			\
		./src/exec/exec_simple.c			\
		./src/exec/generic_exec.c			\
		./src/exec/generic_exec2.c			\
		./src/exec/handle_builtins.c		\
		./src/exec/utils_parse_command.c	\
		./src/exec/open_fct.c				\
		./src/exec/open_fct_utils.c			\
		./src/exec/heredoc.c				\
		./src/exec/fd_utils.c				\
		./src/exec/redirections.c			\
		./src/exec/check_and_exit.c			\
		./src/exec/handle_export.c			\
		\
		./src/jokers/skibidi_split.c		\
		\
		./src/loop/loop.c					\
		./src/loop/parse_command.c			\
		./src/loop/utils_parse_2.c			\
		./src/loop/token_utils.c			\
		./src/loop/commands_lst.c			\
		./src/loop/redirections.c			\
		./src/loop/utils.c					\
		./src/loop/proc.c					\
		./src/loop/proc_utils.c				\
		./src/loop/join_utils.c				\
		./src/loop/expand.c					\
		./src/loop/env_utils.c				\
		\
		./src/signaux/handle_signal.c		\
		\
		./src/tokens/add_tokens.c			\
		\
		./src/utils/free.c					\
		./src/utils/free2.c					\
		./src/utils/free_ctx.c				\
		./src/utils/free_cmd.c				\
		./src/utils/ft_fprintf.c			\
		./src/utils/init.c					\
		./src/utils/list.c					\
		./src/utils/utils.c					\
		\
		./src/exec_utils.c					\
		./src/exec.c						\
		./src/main.c

LMAKE = @make --no-print-directory -C

OBJS_DIR = objs

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.obj)

DEPS = $(OBJS:%.obj=%.d)

INCLUDES = -I ./include/ -I $(LIBFT_P)/ -I .

CFLAGS = -Wall -Wextra -Werror -Wno-unused-result -MMD -O3 -fPIE -g3 #

LIBFT_P = ./libft

LIB = $(LIBFT_P)/libft.a \

TLIB = -ltermcap -lreadline \

# ############## #
#*    REGLES    *#
# ############## #

#pre_comp
all: libft_comp   $(NAME) #mischief

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) $(TLIB) -o $@
	@echo "$(BOLD)$(CYAN)Exécutable $(NAME) créé avec succès!$(CLASSIC)"

$(OBJS_DIR)/%.obj: %.c
	@mkdir -p $(@D)
	@echo "$(GREEN)🗸 Compilation $(BOLD)$(YELLOW)$<$(CLASSIC)"
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(BOLD)$(RED)"
	rm $(LIBFT_P)/$(OBJS_DIR) -rf
	rm $(OBJS_DIR) -rf
	@echo "$(CLASSIC)"

fclean: clean
	@echo "$(BOLD)$(RED)"
	rm $(LIB) -f
	rm $(NAME) -f
	@echo "$(BOLD)$(GREEN)Tout a été supprimé... 🗑️\n$(CLASSIC)"

re: fclean all

.PHONY: all clean fclean re
-include $(DEPS)

# ############## #
#*    AUTRES    *#
# ############## #

# Permet de compiler la libft avant $(NAME)
libft_comp:
	$(LMAKE) $(LIBFT_P)

pre_comp:
	@echo "$(BOLD)"
	@echo "$(PURPLE)"
	@echo "\t\t╔╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╗"
	@sleep 0.008
	@echo "\t\t╠╬╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╬╣"
	@sleep 0.008
	@echo "\t\t╠╣                                               ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣                                               ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣     __    __     __     __   __     __        ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣    /\ '-./  \   /\ \   /\ '-.\ \   /\ \       ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣    \ \ \-./\ \  \ \ \  \ \ \-.  \  \ \ \      ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣     \ \_\ \ \_\  \ \_\  \ \_\/'\_\  \ \_\     ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣      \/_/  \/_/   \/_/   \/_/ \/_/   \/_/     ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣                                               ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣     ______   __         ______     ______     ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣    /\  ___\ /\ \       /\  __ \   /\  == \    ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣    \ \  __\ \ \ \____  \ \ \/\ \  \ \  _-/    ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣     \ \_\    \ \_____\  \ \_____\  \ \_\      ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣      \/_/     \/_____/   \/_____/   \/_/      ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣                                               ╠╣"
	@sleep 0.008
	@echo "\t\t╠╣                                               ╠╣"
	@sleep 0.008
	@echo "\t\t╠╬╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╦╬╣"
	@sleep 0.008
	@echo "\t\t╚╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╩╝"
	@sleep 0.008
	@echo "$(CLASSIC)"
	@echo "$(YELLOW)"

mischief:
	@echo "$(WHITE)$(BLACK)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@$(YELLOW)%(/(############(/(%$(BLACK)@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@$(YELLOW)((############################(($(BLACK)@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@@@@@@@@@$(YELLOW)/########%%%%%%%%%%%%%%%%%%%%##########/$(BLACK)@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@@@@@@$(YELLOW)/######%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#######/&$(BLACK)@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@@@$(YELLOW)######%#%&@@@@@@@@@@&%%%%%%%%%%%%%%%%%%%%%%%%######($(BLACK)@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@$(YELLOW)(####%@@@@@@@@@@@&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%%######/$(BLACK)@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@$(YELLOW)%####@@@@@@@@@@@&&&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%%#######$(BLACK)@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@$(YELLOW)/##%@@@@@@@@@@&&&&&&&&&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%%###%%/$(BLACK)@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@$(YELLOW)(##&@@@@@@@&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%%%%%%%%%%%%%%%%%##%%%($(BLACK)@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@$(YELLOW)(%%&@@@@@&&&&&&&&&&&&&&&&&&&&&.*,*,,&&&&&&&%%%%%%%%%%%%%%%%%%%%%%($(BLACK)@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@$(YELLOW)(##&@@&&&&&&&&&&&&&&&&&&&&&&&&.,.(%%#.&&&%#####%%%%%%%%%%%%%%%%%%&#/$(BLACK)@@"
	@sleep 0.01
	@echo "@@@@@@@@@$(YELLOW)&##%&&&&&&&&&&&&&&&&&&&&&&&&&&.,....,%%&%%####&&&&&%%%%%%%%%%%%%%%&&#$(BLACK)@@"
	@sleep 0.01
	@echo "@@@@@@@@@$(YELLOW)###&&&&&&&&&&&&&&&&&&&&&&&&&%,,,$(WHITE)#&@&&&&$(YELLOW).###%&&&%(/(((%%%%%%%%%%%%%&&#/$(BLACK)@"
	@sleep 0.01
	@echo "@@@@@@@@@$(YELLOW)(#%&&&&&&&&&&&&&&&&&&&&&&&&(,,*.*** $(WHITE)@@@$(YELLOW)&#(%%%%(/%%%%%%%%%%%%%%%%%%&&#/$(BLACK)@"
	@sleep 0.01
	@echo "@@@@@@@@@$(YELLOW)(#%&&&&&&&&&&&&&&&&&&/#&&&,,,,,$(BLUE)@*  *$(WHITE),@@@$(YELLOW)&*#%%/%%/%%%%%%%%%%%%%%%%%&&#/$(BLACK)@"
	@sleep 0.01
	@echo "@@@@@@@@@$(YELLOW)###,..*%&&&&&&&&&(.&&&.(&.,,,* $(BLUE)(   #$(WHITE).@@@$(YELLOW)&((%%%%%(%%%%%%%%%%%%%%%%%&&#($(BLACK)@"
	@sleep 0.01
	@echo "@@@@@@@@@@$(YELLOW)(#,..,,,,,, #&&&&,.&&&.,,,,($(WHITE)%@@$(YELLOW), ,$(WHITE)@@@@@$(YELLOW)&,#%%%%%/&#%%%%%%%%%%%%%%%&%($(BLACK)@@"
	@sleep 0.01
	@echo "@@@@@@@@@@$(YELLOW)##.&&&#,$(WHITE)%%/$(YELLOW)..,,,,.,.&%%.,,%.$(WHITE)@@@@@@@@@@$(YELLOW)&,##%%%%#%&#%%%%%%%%%%%%%%%&#($(BLACK)@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@$(YELLOW)/#&&%(#&$(WHITE)@@ $(BLUE)/  %$(YELLOW)*/*#%%%%%%%%#.$(WHITE)&&&&&&&$(YELLOW)*(#%%%%%##$(WHITE)&&$(YELLOW)@#%%%%%%%%%%%%%%%/$(BLACK)@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@$(YELLOW)/%%###,$(WHITE)@@@@@@@@&$(YELLOW)/%%%%%%%%%%%########%%%%%%(&$(WHITE)@@@@$(YELLOW)#%%%%%%%%%%%%%%/$(BLACK)@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@$(YELLOW)##%%%##**$(WHITE)%&&$(YELLOW)(,#%%%%%%%%%%%%%%%%%%%######/$(WHITE)@#@@@$(YELLOW)(&%%%%%%%%%##%#($(BLACK)@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@$(YELLOW)/#%%%%%%%%%%%%%%%%%%%%%%%%%%########/$(WHITE)@@@@(@@$(YELLOW)%%%%%%%%%%###/$(BLACK)@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@@$(YELLOW)/#%%%%%%%%(#&&%#(%%#############/$(WHITE)@@@@@@@($(YELLOW)%%%%%%%%%###(&$(BLACK)@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@@@@$(YELLOW)/##%%%%#%&&%%%#(#%#######%#/$(WHITE)(@@@@@@@&($(YELLOW)&%%%%%%%###/$(BLACK)@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@$(YELLOW)((#(/#/##(/%%%%%%####((###(#$(WHITE)@@@@#@@@@/$(YELLOW)&&%##%%%###/&$(BLACK)@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@@$(YELLOW)%%%%###%%%%%%(%%%%%%%@@@@%$(WHITE)((%&&%###$(YELLOW)%#%##/#\/#\\#$(BLACK)@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@$(YELLOW)(%&&%%%%%%%%%%##%%%%%%%%%&&&/%#%#(#%#%####/%$(BLACK)@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@@@@@@@$(YELLOW)(%%%%(%&&&&&%%%%%%%&&%////(##&$(BLACK)@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@@@@@$(YELLOW)(%%%/&&&&&&&&&&&%%%%%%&%#$(BLACK)@@@@@#$(BLACK)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@@@$(YELLOW)%(%%##&&&%&&&&&&&&&%%%%%%%($(BLACK)@@@@@@#$(BLACK)@@@&@@@@@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@$(YELLOW)/%%%%##&&&&%&&&&&%&&&&%%%%%%/$(BLACK)@@@@@@#$(BLACK)@@@@($(BLACK)@@@@@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@$(YELLOW)(&&&%%%/&&&&%%%&##&&&&&%%#(%#($(BLACK)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@%@@@$(YELLOW)/&&%%%%/&&&%%%%/&&&%%%(#%%/&$(BLACK)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@(@@@(@@@@$(YELLOW)/%%%%#/%%%%#(&&%%%%/%#($(BLACK)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@/@@@@(@@@$(YELLOW)/##%%%%%(%&&%%%%###($(BLACK)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@#@$(YELLOW)%/%&&&#/%/%%#%/(&$(BLACK)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
	@sleep 0.01
	@echo "@@@@@@@@@@@@@@@$(YELLOW)&/##(($(BLACK)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n$(CLASSIC)"
