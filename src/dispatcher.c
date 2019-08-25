/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 21:52:16 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/25 15:34:56 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include "fractals.h"

static const struct s_command	g_dispatchable[] =
{
	{
		.name = "mandelbrot",
		.temp_late = {
			100,
			&mandel_pixel,
			{0},
			NULL
		}
	},
	{
		.name = "julia",
		.temp_late = {
			100,
			&mandel_pixel,
			{0},
			NULL
		}
	},
	{
		.name = NULL
	}
};

void							calculate_fractal(struct s_fractal *fractal, struct s_rgba_map *pixels)
{
	static struct s_fractal	*current;
	int32_t					i;
	int32_t					j;

	if (current != fractal)
		current = fractal;
	i = -1;
	while (++i < pixels->width)
	{
		j = -1;
		while (++j < pixels->height)
		{
			current->calculator(current, pixels, i, j);
		}
	}
}

#define SCANCODE key.keysym.scancode

int								forknrun(const struct s_command *cmd, const char __unused **argv, __unused void *display)
{
	struct s_fractal	fractal;
	struct s_rgba_map	*pixels;
	SDL_Surface			*surface;
	SDL_Window			*window;
	//bool				is_interactive;

	pixels = ft_memalloc(sizeof(struct s_rgba_map));
	pixels->height = 1000;
	pixels->width = 1000;
	pixels->map = ft_memalloc(sizeof(uint32_t) * pixels->width * pixels->height);
	fractal = cmd->temp_late;
	window = SDL_CreateWindow("mandelbrot",
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
							pixels->width,
							pixels->height, SDL_WINDOW_OPENGL);
	calculate_fractal(&fractal, pixels);

	if (window == NULL)
		ft_dprintf(2, "window is null\n");
	surface = SDL_GetWindowSurface(window);
	if (surface == NULL)
		ft_dprintf(2, "surface is null\n");
	ft_dprintf(2, "h = %d; w = %d\n", surface->h, surface->w);
	ft_memcpy(surface->pixels, pixels->map, surface->h * surface->w * 4);
	SDL_UpdateWindowSurface(window);
	bool is_running = true;
	SDL_Event event;
	while (is_running)
	{
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT ||
			(SDL_KEYDOWN == event.type && event.SCANCODE == SDL_SCANCODE_ESCAPE) ||
			(SDL_KEYDOWN == event.type && event.SCANCODE == SDL_SCANCODE_Q))
				is_running = false;
		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);
	}
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
