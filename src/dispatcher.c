/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 21:52:16 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/02 19:33:18 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include "fractals.h"

static const struct s_command	g_dispatchable[] =
{
	{
		.name = "mandelbrot",
		.temp_late = {
			0,
			50,
			&mandel_pixel,
			&mandel_avx2,
			NULL,
			{0},
			NULL
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
			NULL
		}
	},
	{
		.name = NULL
	}
};

void							calculate_fractal_avx(struct s_fractal *fractal,
	struct s_rgba_map *pixels, void *sdl_pixels)
{
	static struct s_fractal	*current;
	const t_fract_calc		func = current ? current->calculator_avx : NULL;
	int32_t					x;
	int32_t					y;

	if (current != fractal)
	{
		current = fractal;
		ft_memcpy((void *)&func, &(current->calculator), sizeof(current->calculator));
	}
	pixels->larger_dimension = pixels->width > pixels->height ? pixels->width : pixels->height;
	pixels->larger_dimension_half = pixels->larger_dimension / 2.0;
	pixels->larger_dimension_quarter = pixels->larger_dimension / 4.0;
	y = 0;
	while (y < pixels->height)
	{
		x = 0;
		while (x < pixels->width)
		{
			func(current, pixels, x, y);
			x += 4;
		}
		y += 1;
	}
	__builtin_memcpy(sdl_pixels, pixels->map,
				pixels->height * pixels->width * sizeof(uint32_t));
}

void							calculate_fractal(struct s_fractal *fractal,
	struct s_rgba_map *pixels, void *sdl_pixels)
{
	static struct s_fractal	*current;
	const t_fract_calc		func = current ? current->calculator : NULL;
	int32_t					x;
	int32_t					y;

	if (current != fractal)
	{
		current = fractal;
		ft_memcpy((void *)&func, &(current->calculator), sizeof(current->calculator));
	}
	x = -1;
	while (++x < pixels->width)
	{
		y = -1;
		while (++y < pixels->height)
			func(current, pixels, x, y);
	}
	__builtin_memcpy(sdl_pixels, pixels->map,
				pixels->height * pixels->width * sizeof(uint32_t));
}

int								forknrun(const struct s_command *cmd,
	const char __unused **argv,
	__unused void *display)
{
	struct s_fractal	fractal;
	struct s_rgba_map	*pixels;
	SDL_Surface			*surface;
	SDL_Window			*window;

	pixels = ft_memalloc(sizeof(struct s_rgba_map));
	pixels->height = 1600;
	pixels->width = 1400;
	pixels->map = ft_memalloc(sizeof(uint32_t) * pixels->width * pixels->height + 4);
	pixels->map_metadata = ft_memalloc(sizeof(struct s_pixel_meta) * pixels->width * pixels->height + 4);
	fractal = cmd->temp_late;
	fractal.input.scroll_depth = 1.0;
	fractal.input.shift_x = 0.0;
	fractal.input.shift_y = 0.0;
//	fractal.gradient_map = grad_create_from(GRADIENT_LINEAR, fractal.max_iterations,
//		2, 0x00, 1, 0xFFFFFF00, fractal.max_iterations);
	fractal.gradient_map = grad_create_from(GRADIENT_LINEAR, fractal.max_iterations,
		3,
		COLOR_GOLDEN_YELLOW, 1,
		COLOR_MACHARIUS_SOLAR_ORANGE, 2 * fractal.max_iterations / 3,
		COLOR_WARLOCK_PURPLE, fractal.max_iterations);
	fractal.gradient_map->is_reverse = true;
	grad_cache_colors(fractal.gradient_map);
	window = SDL_CreateWindow("Good ol' Fract 'ol",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		pixels->width,
		pixels->height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
		exit(ft_dprintf(2, "window is null\n"));
	surface = SDL_GetWindowSurface(window);
	if (surface == NULL)
		exit(ft_dprintf(2, "surface is null\n"));
	//calculate_fractal(&fractal, pixels, surface->pixels);
	SDL_UpdateWindowSurface(window);
	game_loop(window, &fractal, pixels);
	return (0);
}

int								dispatch(const char **argv, void *display)
{
	int		i;
	int		status;

	status = -1;
	i = -1;
	while (g_dispatchable[++i].name)
	{
		if (ft_strcmp(argv[0], g_dispatchable[i].name) == 0)
			status = forknrun(g_dispatchable + i, argv + 1, display);
	}
	return (status);
}
