# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/08/21 13:49:23 by vtarasiu          #+#    #+#              #
#    Updated: 2019/09/29 20:35:30 by vtarasiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
NAME = fractol

CFLAGS = -mavx -mavx2 -Wall -Wextra -Werror \
         -g -O2 #-fsanitize=address #

LIB_DIR = lib/

PRINTF_DIR = $(LIB_DIR)printf/
LIBPRINTF_NAME = libftprintf.a
PRINTF_PATH = $(PRINTF_DIR)$(LIBPRINTF_NAME)

LIBFT_DIR  = $(LIB_DIR)libft/
LIBFT_NAME = libft.a
LIBFT_PATH = $(LIBFT_DIR)$(LIBFT_NAME)

FRACTOL_SRC_DIR = src/
FRACTOL_SRC = calculators.c \
              colorizer.c \
              dispatcher.c \
              gradients.c \
              gradient_map.c \
              main.c \
              quit.c \
              rgb_to_hsv.c \
              sdl_handlers.c \
              sdl_loop.c \
              thread_pool.c \
              mlx_loop.c

#FRACTOL_SRC += mlx_loop.c

FRACTOL_FRACTALS_DIR = fractals/
FRACTOL_FRACTALS_SRC = mandelbrot.c \
                       julia.c \
                       julia_abs.c \
                       burning_ship.c

HEADERS = fractals.h \
          fractol_png.h \
          fractol_data.h \
          fractol_tpool.h \
          fractol_common.h \
          fractol_gradients.h

INCLUDES = -I include -I $(PRINTF_DIR)/include -I $(LIBFT_DIR) -I /usr/include/SDL2
LIBRARIES = -lm -lpthread -L$(PRINTF_DIR) -L$(LIBFT_DIR) -lSDL2 -lSDL2_ttf

OBJ_DIR = obj/
OBJ = $(addprefix $(OBJ_DIR), $(FRACTOL_SRC:.c=.o)) \
      $(addprefix $(OBJ_DIR)$(FRACTOL_FRACTALS_DIR), $(FRACTOL_FRACTALS_SRC:.c=.o))

SRC = $(addprefix $(FRACTOL_SRC_DIR), $(FRACTOL_SRC)) \
      $(addprefix $(OBJ_DIR)$(FRACTOL_FRACTALS_DIR), $(FRACTOL_FRACTALS_SRC))

NEEDS_SYNC = false

all: $(NAME)

$(NAME): $(OBJ) $(PRINTF_PATH) $(LIBFT_PATH)
	@echo "    CC $(NAME)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT_PATH) $(PRINTF_PATH) $(INCLUDES) $(LIBRARIES)


$(OBJ_DIR)%.o: $(FRACTOL_SRC_DIR)%.c | $(OBJ_DIR)
	@echo "    CC $<"
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/$(FRACTOL_FRACTALS_DIR)

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

$(LIBPNG_PATH):
	@echo "cmake $(LIBPNG_DIR)"
	@cmake $(LIBPNG_DIR) 2>/dev/null
	@echo "make -C $(LIBPNG_DIR)"
	@make -C $(LIBPNG_DIR) 2>/dev/null

$(LIBFT_PATH):
	make -C $(LIBFT_DIR)

$(PRINTF_PATH):
	make -C $(PRINTF_DIR)

clean:
	/bin/rm -f $(OBJ)
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	/bin/rm -f $(NAME)

lclean:
	make -C $(LIBFT_DIR)/ fclean
	make -C $(PRINTF_DIR)/ fclean

re: fclean lclean all

love:
	@echo "Not all."

.PHONY: all clean fclean re libs

