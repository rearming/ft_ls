# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sleonard <sleonard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/05 16:40:18 by sleonard          #+#    #+#              #
#    Updated: 2019/11/08 21:04:35 by sleonard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#    PROJECT    #
NAME = ft_ls

#    DIRECTORIES    #
SRC_DIR = ./sources/
LIB_DIR = ./libft/
LIB_PRINTF = ./libft/ft_printf/
OBJ_DIR = ./obj/
SRC_INC_DIR = ./includes/
LIB_INC_DIR = ./libft/includes

#    FLAGS    #
CFLGS = -Wall -Werror -Wextra -O2
CFLGS_DBG = -g -fsanitize=address
LFLGS = -L$(LIB_DIR) -lft -L$(LIB_PRINTF) -lftprintf
CC = gcc

#    LIBFT    #
LIBFT = $(LIB_DIR)/libft.a

#    SOURCES AND HEADERS    #
SRC_FILES = coloring.c compare_functions.c dir_tree_getter.c dir_tree_utils.c \
	error.c filestruct_functions.c format_print.c format_print_utils.c ft_ls.c \
	ls_recursive.c options_parser.c rights_field_getter.c size_print_utils.c \
	args_processor.c utils.c
SRCS = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJS = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

#    INCLUDES    #
INCL = -I$(SRC_INC_DIR) -I$(LIB_INC_DIR)

#    RULES    #
.PHONY: all clean fclean re

all: $(OBJ_DIR) $(NAME)

-include: $(DEPS)
$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(CFLGS) -c -o $@ $< $(INCL)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT):
	$(MAKE) -C $(LIB_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLGS) -o $(NAME) $(OBJS) $(INCL) $(LFLGS)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIB_DIR) clean

fclean: clean
	rm -rf $(NAME)
	$(MAKE) -C $(LIB_DIR) fclean

re: fclean all

#    AUTO GENERATED BY 21 MAKEFILE GENERATOR (21MFG)    #
#         https://github.com/ltanailov/21mfg.git        #
