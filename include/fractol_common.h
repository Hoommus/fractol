/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_common.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 22:09:50 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/28 12:22:43 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_COMMON_H
# define FRACTOL_COMMON_H

# if __has_include("mlx.h")
#  define CONFIG_HAS_MLX
#  include <mlx.h>
# endif

# include <stddef.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>
# include <errno.h>
# include <stdnoreturn.h>
# include <stdarg.h>
# include <signal.h>
# include "libft.h"
# include "ft_printf.h"
# include "fractol_data.h"

# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>

#define UI_FEEDBACK_REDRAW       0x0001U
#define UI_FEEDBACK_AVX          0x0002U
#define UI_FEEDBACK_NEUTRAL      0x0100U
#define UI_FEEDBACK_MOUSE_DOWN   0x0010U
#define UI_FEEDBACK_MOUSE_UP     0x0020U

enum e_options {
	OPTION_HELP = 1,
	OPTION_AVX = 1 << 1,
	OPTION_OPENCL = 1 << 2,
	OPTION_CUDA = 1 << 3,
	OPTION_NO_GUI = 1 << 9,
	OPTION_NO_USER_INPUT = 1 << 10,
	OPTION_THREADED = 1 << 11,
	OPTION_VERBOSE = 1 << 12,
};

struct s_options {
	uint64_t opts;
	uint8_t threads;
	uint32_t width;
	uint32_t height;
	double cx;
	double cy;
	double scale;
	const char *infile;
	const char *outfile;
};

int dispatch(const char **argv, const struct s_options *options);

uint32_t
poll_events(SDL_Window *window, struct s_fractal *fractal, struct s_rgba_map *pixels, const struct s_options *options);

void render_metadata(SDL_Window *window,
					 struct s_fractal *fractal,
					 struct s_rgba_map *pixels);

noreturn void sdl_game_loop(SDL_Window *window, struct s_fractal *fractal,
							struct s_rgba_map *pixels,
							const struct s_options *restrict options);

#ifdef CONFIG_HAS_MLX
noreturn void		mlx_game_loop(void *mlx_ptr, void *mlx_window, struct s_fractal *fractal, struct s_rgba_map *pixels,
							const struct s_options *options);
#endif

noreturn void quit(int status);

#endif
