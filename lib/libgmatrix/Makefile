# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/17 13:09:18 by ohakola           #+#    #+#              #
#    Updated: 2020/08/16 16:19:27 by ohakola          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
NAME = libmatrix.a
LIBFT = ../libft
DIR_SRC = ./
DIR_OBJ = temp
SOURCES = ml_matrix_new.c \
			ml_matrix_add.c \
			ml_matrix_sub.c \
			ml_matrix_mul.c \
			ml_matrix_id.c \
			ml_matrix_set_all.c \
			ml_scale_matrix.c \
			ml_matrix_scale.c \
			ml_matrix_set_vals.c \
			ml_dimensions_equal.c \
			ml_matrix_add_double.c \
			ml_matrix_sub_double.c \
			ml_matrix_mul_double.c \
			ml_matrix_mul_vector.c \
			ml_putint_fd.c \
			ml_putint.c \
			ml_puterror.c \
			ml_matrix_free.c \
			ml_vector_free.c \
			ml_vector_new.c \
			ml_vector4_new.c \
			ml_vector_scale.c \
			ml_vector_set_all.c \
			ml_vector_set_vals.c \
			ml_vector_to_matrix.c \
			ml_putvector.c \
			ml_vector_mag.c \
			ml_vector_sub.c \
			ml_vector_add.c \
			ml_vector_dot.c \
			ml_vector_cross.c \
			ml_vector_normalize.c \
			ml_view_matrix.c \
			ml_projection_matrix.c \
			ml_vector_forward.c \
			ml_rotation_matrix.c \
			ml_matrix_inverse_4x4.c \
			ml_matrix_mul_vector_lst.c \
			ml_vector_dup.c \
			ml_putmatrix.c
SRCS = $(addprefix $(DIR_SRC)/,$(SOURCES))
OBJS = $(addprefix $(DIR_OBJ)/,$(SOURCES:.c=.o))
INCLUDES = -I ./ -I $(LIBFT)/incl
LIBFTFLAGS = -L $(LIBFT) -lft
FLAGS = -Wall -Wextra -Werror

all: $(DIR_OBJ) $(NAME)

$(NAME): $(OBJS)
	@ar rc $(NAME) $(OBJS)

$(DIR_OBJ):
	@mkdir -p temp

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@$(CC) $(FLAGS) $(LIBMATRIXFLAGS) $(INCLUDES) -c -o $@ $<

test:
	@make -C ./
	@$(CC) $(FLAGS) -o test test.c $(OBJS)
	./test
	@/bin/rm test
	@make fclean

clean:
	@/bin/rm -f $(OBJS)
	@/bin/rm -rf $(DIR_OBJ)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: all, $(DIR_OBJ), clean, fclean, re
