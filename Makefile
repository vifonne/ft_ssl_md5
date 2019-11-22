# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vifonne <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/11 10:20:14 by vifonne           #+#    #+#              #
#    Updated: 2019/11/22 16:44:39 by vifonne          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ft_ssl

SRC_DIR		=	srcs
SRC			=	main.c				\
				md5.c				\
				error.c				\
				sha256.c			\
				commons.c			\
				commons_utils.c		\
				bytes.c				\
				options.c			\
				options_launchers.c	\
				utils.c
OBJ_DIR		=	.obj
OBJ			=	$(SRC:.c=.o)
DEP			=	$(OBJ:.o=.d)
INC_DIR		=	includes

CC			=	gcc -g
CFLAGS		=	-Wall -Wextra -Werror -I ./libft -I $(INC_DIR)

LIBFT		=	./libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(addprefix $(OBJ_DIR)/,$(OBJ))
	$(CC) -o $@ $^

$(LIBFT):
	$(MAKE) -C ./libft

-include $(addprefix $(OBJ_DIR)/,$(DEP))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

$(OBJ_DIR):
	@/bin/mkdir $@

clean:
	@echo "Cleaning $(NAME)"
	@$(MAKE) -C ./libft clean
	@/bin/rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Full clean $(NAME)"
	@$(MAKE) -C ./libft fclean
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
