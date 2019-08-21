# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/08/21 13:49:23 by vtarasiu          #+#    #+#              #
#    Updated: 2019/08/21 20:01:04 by vtarasiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
NAME = fractol

CFLAGS = -Wall -Wextra -Werror \
         -g

LIB_DIR = lib

PRINTF_DIR = $(LIB_DIR)/printf
LIBPRINTF_NAME = libftprintf.a

LIBFT_DIR  = $(LIB_DIR)/libft
LIBFT_NAME = libft.a

LIBPNG_DIR = $(LIB_DIR)/libspng
LIBPNG_NAME_STATIC  = libspng_static.a
LIBPNG_NAME_DYNAMIC = libspng.dylib

LIBNAMES = $(LIBPNG_NAME_STATIC) $(LIBFT_NAME) $(LIBPRINTF_NAME)
STATIC_LIBS = $(addprefix lib/, $(LIBNAMES))

FRACTOL_SRC_DIR = src/
FRACTOL_SRC = main.c

HEADERS = fractals.h \
          fractol_png.h

INCLUDES = -I include -I $(PRINTF_DIR)/include -I $(LIBFT_DIR) -I $(LIBPNG_DIR)
LIBRARIES = -lmlx -lft -lspng -lftprintf -L$(PRINTF_DIR) -L$(LIBFT_DIR) -L$(LIBPNG_DIR)

OBJ_DIR = obj/
OBJ = $(addprefix $(OBJ_DIR), $(FRACTOL_SRC:.c=.o))
SRC = $(addprefix $(FRACTOL_SRC_DIR), $(FRACTOL_SRC))

NEEDS_SYNC = false


all: $(NAME)

$(NAME): $(OBJ)
	@echo "    CC $(NAME)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -I $(INCLUDE) $(LIBRARIES)

$(OBJ_DIR)%.o: $(FRACTOL_SRC_DIR)%.c | $(STATIC_LIBS)
	@if ! [ -d $(OBJ_DIR) ] ; then \
	    mkdir -p $(OBJ_DIR) ; \
	fi
	@echo "    CC $<"
	@$(CC) $(FLAGS) $(INCLUDES) -o $@ -c $<

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

prepare: submodules
	@mkdir -p $(OBJ_DIR)

submodules:
	@if ! [ -d $(PRINTF_DIR) ] ; then \
	    echo "printf not found." ; \
	    NEEDS_SYNC = true ; \
	fi

	@if ! [ -d $(LIBFT_DIR) ] ; then \
	    echo "libft not found." ; \
	    NEEDS_SYNC = true ; \
	fi

	@if ! [ -d $(LIBPNG_DIR) ] ; then \
	    echo "libspng not found." ; \
	    NEEDS_SYNC = true ; \
	fi

ifeq ($(NEEDS_SYNC), true)
	@echo "Some libraries were not found\nYou did --recursive clone, right?"
	@echo "cloning submodules..."
	git submodule update --init
	git submodule sync
	@echo "cloning complete."
endif

lib/$(LIBPNG_NAME_STATIC):
	@echo "cmake $(LIBPNG_DIR)/"
	@cmake $(LIBPNG_DIR)/ 2>/dev/null
	@echo "make -C $(LIBPNG_DIR)/"
	@make -C $(LIBPNG_DIR)/ 2>/dev/null

lib/$(LIBPRINTF_NAME):
	make -C $(LIBFT_DIR)/

lib/$(LIBFT_NAME):
	make -C $(PRINTF_DIR)/

clean:
	/bin/rm -f $(OBJ)
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	/bin/rm -f $(NAME)

lclean:
	make -C $(LIBFT_DIR)/ fclean
	make -C $(PRINTF_DIR)/ fclean
	make -C $(LIBPNG_DIR)/ clean

re: fclean lclean all

love:
	@echo "Not all."

.PHONY: all clean fclean re prepare libs

