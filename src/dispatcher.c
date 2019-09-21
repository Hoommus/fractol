/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 21:52:16 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/20 18:58:28 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include "fractol_gradients.h"
#include "fractals.h"

static const struct s_command	g_dispatchable[] =
{
	{
		.name = "mandelbrot",
		.temp_late = {
			0,
			100,
			&mandel_pixel,
			&mandel_avx2,
			NULL,
			{0},
		}
	},
	{
		.name = "julia",
		.temp_late = {
			0,
			50,
			&mandel_pixel,
			&mandel_avx2,
			NULL,
			{0},
		}
	},
	{
		.name = NULL
	}
};

static inline struct s_rgba_map	*init_common(struct s_fractal *fractal)
{
	struct s_rgba_map	*pixels;

	pixels = ft_memalloc(sizeof(struct s_rgba_map));
	pixels->height = 1200;
	pixels->width = 1200;
	pixels->map = ft_memalloc(sizeof(uint32_t) * pixels->width * pixels->height + 4);
	pixels->map_metadata = ft_memalloc(sizeof(struct s_pixel_meta) * pixels->width * pixels->height + 4);
	fractal->max_iterations = 100;
	fractal->gradient_map = grad_create_from(GRADIENT_LINEAR, fractal->max_iterations,
		4,
	COLOR_GOLDEN_YELLOW, 0, COLOR_WARLOCK_PURPLE, 30, COLOR_BLOOD_RED, 70, COLOR_ULTRAMARINE, fractal->max_iterations);
	fractal->gradient_map->is_reverse = false;
	grad_cache_colors(fractal->gradient_map);
	return (pixels);
}

static int						forknrun_sdl(const struct s_command *cmd, const struct s_options *options)
{
	struct s_fractal	fractal;
	struct s_rgba_map	*pixels;
	SDL_Window			*window;

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

int								forknrun_mlx(const struct s_command *cmd, __unused const struct s_options *options)
{
	struct s_fractal	fractal;
	struct s_rgba_map	*pixels;
	void				*mlx_ptr;
	void				*mlx_window;

	fractal = cmd->temp_late;
	pixels = init_common(&(fractal));
	if (!(mlx_ptr = mlx_init()))
		exit(ft_dprintf(2, "mlx init failed\n"));
	signal(SIGINT, &quit);
	mlx_window = mlx_new_window(mlx_ptr, 1200, 1200, "Good ol' mlx Fract 'ol");
	mlx_game_loop(mlx_ptr, mlx_window, &fractal, pixels);
	return (0);
}

int dispatch(const char **argv, const struct s_options *options)
{
	int		i;
	int		status;

	status = -1;
	i = -1;
	while (g_dispatchable[++i].name)
	{
		if (ft_strcmp(argv[0], g_dispatchable[i].name) == 0)
			status = forknrun_sdl(g_dispatchable + i, options);
	}
	return (status);
}
