/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 21:52:16 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/30 13:58:02 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <complex.h>
#include "fractol_common.h"
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
			NULL
		}
	},
	{
		.name = "julia",
		.temp_late = {
			0,
			100,
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
	pixels->height = 1000;
	pixels->width = 1000;
	pixels->map = ft_memalloc(sizeof(uint32_t) * pixels->width * pixels->height);
	fractal = cmd->temp_late;
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
	calculate_fractal(&fractal, pixels, surface->pixels);
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
