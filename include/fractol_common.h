/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_common.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 22:09:50 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/20 18:42:30 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_COMMON_H
# define FRACTOL_COMMON_H

# include <mlx.h>
# include <stddef.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdnoreturn.h>
# include <stdarg.h>
# include "libft.h"
# include "ft_printf.h"
# include "fractol_data.h"

# include <SDL.h>
# include <SDL_ttf.h>

#define UI_FEEDBACK_REDRAW       0x0001U
#define UI_FEEDBACK_AVX          0x0002U
#define UI_FEEDBACK_NEUTRAL      0x0100U
#define UI_FEEDBACK_MOUSE_DOWN   0x0010U
#define UI_FEEDBACK_MOUSE_UP     0x0020U

enum				e_options
{
	OPTION_HELP = 1,
	OPTION_AVX = 4,
	OPTION_OPENCL = 8,
	OPTION_CUDA = 16,
	OPTION_NO_GUI = 512,
	OPTION_NO_USER_INPUT = 1024,
	OPTION_THREADED = 2048,
	OPTION_SDL = 4096,
	OPTION_MLX = 8192,
};

struct				s_options
{
	uint64_t		opts;
	uint8_t			threads;
	uint32_t		width;
	uint32_t		height;
	double			cx;
	double			cy;
	double			scale;
	const char		*infile;
	const char		*outfile;
};

int dispatch(const char **argv, const struct s_options *options);
uint32_t			poll_events(SDL_Window *window,
								struct s_fractal *fractal,
								struct s_rgba_map *pixels);
void				render_metadata(SDL_Window *window,
								struct s_fractal *fractal,
								struct s_rgba_map *pixels);

noreturn void sdl_game_loop(SDL_Window *window, struct s_fractal *fractal, struct s_rgba_map *pixels,
							const struct s_options *options);
noreturn void		mlx_game_loop(void *mlx_ptr,
								void *mlx_window,
								struct s_fractal *fractal,
								struct s_rgba_map *pixels);

noreturn void		quit(int status);

#endif
