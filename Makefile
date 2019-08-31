# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/08/21 13:49:23 by vtarasiu          #+#    #+#              #
#    Updated: 2019/08/30 20:43:00 by vtarasiu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
NAME = fractol

CFLAGS = -mavx -mavx2 -Wall -Wextra -Werror \
         -O2 -fsanitize=address

LIB_DIR = lib/

PRINTF_DIR = $(LIB_DIR)printf/
LIBPRINTF_NAME = libftprintf.a
PRINTF_PATH = $(PRINTF_DIR)$(LIBPRINTF_NAME)

LIBFT_DIR  = $(LIB_DIR)libft/
LIBFT_NAME = libft.a
LIBFT_PATH = $(LIBFT_DIR)$(LIBFT_NAME)

LIBPNG_DIR = $(LIB_DIR)libspng/
LIBPNG_NAME_STATIC  = libspng_static.a
LIBPNG_NAME_DYNAMIC = libspng.dylib
LIBPNG_PATH = $(LIBPNG_DIR)$(LIBPNG_NAME_STATIC)

FRACTOL_SRC_DIR = src/
FRACTOL_SRC = main.c \
              dispatcher.c \
              mandelbrot.c \
              game_loop.c \
              ui_handlers.c \
              colorizer.c \
              gradient_map.c \
              quit.c

HEADERS = fractals.h \
          fractol_png.h

INCLUDES = -I include -I $(PRINTF_DIR)/include -I $(LIBFT_DIR) -I $(LIBPNG_DIR) \
           -I /Library/Frameworks/SDL2.framework/Versions/A/Headers/
LIBRARIES = -lft -lftprintf -L$(PRINTF_DIR) -L$(LIBFT_DIR) -L$(LIBPNG_DIR) \
            -framework OpenGL -framework AppKit -L/usr/local/lib/ \
            -F /Library/Frameworks -framework SDL2

OBJ_DIR = obj/
OBJ = $(addprefix $(OBJ_DIR), $(FRACTOL_SRC:.c=.o))
SRC = $(addprefix $(FRACTOL_SRC_DIR), $(FRACTOL_SRC))

NEEDS_SYNC = false


all: $(NAME)

$(NAME): $(OBJ)
	@echo "    CC $(NAME)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -I $(INCLUDE) $(LIBRARIES)

# TODO: add header dependencies
$(OBJ_DIR)%.o: $(FRACTOL_SRC_DIR)%.c $(LIBPNG_PATH) $(LIBFT_PATH) $(PRINTF_PATH) #| $(OBJ_DIR)
	@if ! [ -d $(OBJ_DIR) ] ; then \
        mkdir -p $(OBJ_DIR) ; \
    fi
	@echo "    CC $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

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
	make -C $(LIBPNG_DIR)/ clean

re: fclean lclean all

love:
	@echo "Not all."

.PHONY: all clean fclean re libs

