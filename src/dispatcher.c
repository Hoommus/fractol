/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 21:52:16 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/10/05 18:39:14 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include "fractol_gradients.h"
#include "fractals.h"

static const struct s_command g_dispatchable[] =
		{
				{
						.name = "mandelbrot",
						.temp_late = {
								0,
								100,
								&mandel_pixel,
								&mandel_avx2,
								NULL,
								{0, .locked = 0}
						}
				},
				{
						.name = "julia",
						.temp_late = {
								0,
								100,
								&julia_pixel,
								&julia_avx2,
								NULL,
								{0, .locked = 0}
						}
				},
				{
						.name = "ship",
						.temp_late = {
								0,
								100,
								&ship_pixel,
								&ship_avx2,
								NULL,
								{0, .locked = 0}
						}
				},
				{
						.name = "julia_abs",
						.temp_late = {
								0,
								100,
								&julia_abs_pixel,
								&julia_abs_avx2,
								NULL,
								{0, .locked = 0}
						}
				},
				{
						.name = NULL
				}
		};

static inline struct s_rgba_map *init_common(struct s_fractal *fractal) {
	struct s_rgba_map *pixels;

	pixels = calloc(1, sizeof(struct s_rgba_map));
	pixels->height = 1200;
	pixels->width = 1200;
	pixels->map = calloc(1, sizeof(uint32_t) * pixels->width * pixels->height + 4);
	pixels->map_metadata = calloc(1, sizeof(struct s_pixel_meta) * pixels->width * pixels->height + 4);
	grad_table_init();
	fractal->gradient_map = grad_from_table(0);
	grad_cache_colors(fractal->gradient_map);
	return (pixels);
}

static int forknrun_sdl(const struct s_command *cmd, const struct s_options *options) {
	struct s_fractal fractal;
	struct s_rgba_map *pixels;
	SDL_Window *window;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		exit(ft_dprintf(2, "hello from fukken SDL: %s\n", SDL_GetError()));
	signal(SIGINT, &quit);
	TTF_Init();
	fractal = cmd->temp_late;
	pixels = init_common(&(fractal));
	window = SDL_CreateWindow("Good ol' Fract 'ol",
							  SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED,
							  pixels->width,
							  pixels->height,
							  SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
		exit(ft_dprintf(2, "window is null\n"));
	calculate_fractal(&fractal, pixels, SDL_GetWindowSurface(window)->pixels);
	SDL_UpdateWindowSurface(window);
	sdl_game_loop(window, &fractal, pixels, options);
	return (0);
}

int forknrun_mlx(const struct s_command *cmd,
				 const struct s_options *restrict options) {
	return (void *) cmd == (void *) options;
}

int dispatch(const char **argv, const struct s_options *options) {
	int i;
	int status;

	status = -1;
	i = -1;
	if (argv[0] != NULL) {
		while (g_dispatchable[++i].name) {
			if (ft_strcmp(argv[0], g_dispatchable[i].name) == 0) {
				if (options->opts & OPTION_SDL)
					status = forknrun_sdl(g_dispatchable + i, options);
				else
					status = forknrun_mlx(g_dispatchable + i, options);
			}
		}
	} else
		status = -2;
	if (status < 0) {
		if (status == -1)
			ft_dprintf(2, "fractol: no fractal named `%s'\n", argv[0]);
		ft_dprintf(2, "Please, specify one of the following fractals:\n");
		i = -1;
		while (g_dispatchable[++i].name)
			ft_dprintf(2, "\t%s\n", g_dispatchable[i].name);
	}
	return (status);
}
