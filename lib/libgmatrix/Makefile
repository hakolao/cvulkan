# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/17 13:09:18 by ohakola           #+#    #+#              #
#    Updated: 2020/08/16 19:35:52 by ohakola          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
NAME = libgmatrix.a
LIBFT = ../libft
DIR_SRC = ./
DIR_TESTS = ./tests
DIR_OBJ = temp
SOURCES = ml_matrix4_id.c \
			ml_matrix4_inverse.c \
			ml_matrix4_mul.c \
			ml_matrix4_mul_vec.c \
			ml_matrix4_projection.c \
			ml_matrix4_rotation.c \
			ml_matrix4_view.c \
			ml_print.c \
			ml_rad.c \
			ml_vector3_add.c \
			ml_vector3_cross.c \
			ml_vector3_dot.c \
			ml_vector3_forward.c \
			ml_vector3_mag.c \
			ml_vector3_mul.c \
			ml_vector3_normalize.c \
			ml_vector3_sub.c \
			ml_vector3_to_vec4.c

SRCS = $(addprefix $(DIR_SRC)/,$(SOURCES))
OBJS = $(addprefix $(DIR_OBJ)/,$(SOURCES:.c=.o))
INCLUDES = -I ./include -I $(LIBFT)/incl
FLAGS = -Wall -Wextra -Werror

all: $(DIR_OBJ) $(NAME)

$(NAME): $(OBJS)
	@ar rc $(NAME) $(OBJS)

$(DIR_OBJ):
	@mkdir -p temp

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

TEST_SRCS =$(wildcard $(DIR_TESTS)/*.c)

test: all
	@make -C $(LIBFT)
	$(CC) -o test_run $(TEST_SRCS) -L$(LIBFT) -lft $(FLAGS) $(INCLUDES) $(NAME)
	./test_run
	@/bin/rm -f main.o
	@/bin/rm -f test_run
	@make fclean
	@make -C $(LIBFT) fclean

clean:
	@/bin/rm -f $(OBJS)
	@/bin/rm -rf $(DIR_OBJ)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: all, $(DIR_OBJ), clean, fclean, re
