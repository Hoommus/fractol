/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 18:58:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/07 16:35:05 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include "fractol_data.h"
#include "fractals.h"
#include <stdnoreturn.h>
#include <time.h>
#include <sys/time.h>

static TTF_Font		*g_font;
static const char	*g_font_names[] = {
	"~/Library/Fonts/UbuntuMono-Regular.ttf",
	"/Library/Fonts/Andale Mono.ttf",
	"/Library/Fonts/Arial.ttf",
	"/Library/Fonts/Courier New Bold.ttf",
	NULL
};

static TTF_Font		*choose_font(void)
{
	int			i;
	TTF_Font	*font;

	i = 0;
	while (g_font_names[i])
	{
		if (access(g_font_names[i], F_OK | R_OK) == 0 &&
			(font = TTF_OpenFont(g_font_names[i], 16)))
			return (font);
		i++;
	}
	return (NULL);
}

static void			render_metadata(SDL_Window *window,
									   __unused struct s_fractal *fractal,
									   struct s_rgba_map __unused *pixels)
{
	SDL_Surface			*metadata_surface;
	SDL_Surface			*gradient_test_surface;

	if (!g_font)
		g_font = choose_font();
	if (!g_font)
	{
		dprintf(2, "No font, lol\n");
		return ;
	}
	TTF_SetFontHinting(g_font, TTF_HINTING_NORMAL);
	metadata_surface = TTF_RenderText_Blended(g_font, "hello world", (SDL_Color){0xFF, 0xFF, 0xFF, 0});
	SDL_BlitSurface(metadata_surface, NULL, SDL_GetWindowSurface(window), NULL);
	SDL_FreeSurface(metadata_surface);
	gradient_test_surface = SDL_CreateRGBSurface(0, pixels->width, 50, 32, 0, 0, 0, 0);
	uint32_t			i;

	i = 0;
	uint32_t			j = 0;

	while (i < fractal->max_iterations)
	{
		((uint32_t *) gradient_test_surface->pixels)[(j)] =
			grad_get_iter_color(fractal->gradient_map, i);
		((uint32_t *) gradient_test_surface->pixels)[(j + 1) * pixels->width] =
			grad_get_iter_color(fractal->gradient_map, i);
		((uint32_t *) gradient_test_surface->pixels)[(j + 2) * pixels->width] =
			grad_get_iter_color(fractal->gradient_map, i);
		((uint32_t *) gradient_test_surface->pixels)[(j + 3) * pixels->width] =
			grad_get_iter_color(fractal->gradient_map, i);
		((uint32_t *) gradient_test_surface->pixels)[(j + 4) * pixels->width] =
			grad_get_iter_color(fractal->gradient_map, i);
		((uint32_t *) gradient_test_surface->pixels)[(j + 5) * pixels->width] =
			grad_get_iter_color(fractal->gradient_map, i);
		((uint32_t *) gradient_test_surface->pixels)[(j + 6) * pixels->width] =
			grad_get_iter_color(fractal->gradient_map, i);
		((uint32_t *) gradient_test_surface->pixels)[(j + 7) * pixels->width] =
			grad_get_iter_color(fractal->gradient_map, i);
		j = i;
		i++;
	}
	SDL_BlitSurface(gradient_test_surface, NULL, SDL_GetWindowSurface(window), NULL);
	SDL_FreeSurface(gradient_test_surface);
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

noreturn void	game_loop(SDL_Window *window, struct s_fractal *fractal, struct s_rgba_map *pixels)
{
	uint32_t	ret;
	bool		is_avx;

	is_avx = true;
	while (true)
	{
		ret = poll_events(window, fractal, pixels);
		if ((ret & UI_FEEDBACK_REDRAW))
		{
			if ((ret & UI_FEEDBACK_AVX))
				is_avx = !is_avx;
			struct timeval	start;
			struct timeval	end;
			gettimeofday(&start, NULL);
			if (is_avx)
				calculate_fractal_avx(fractal, pixels, SDL_GetWindowSurface(window)->pixels);
			else
				calculate_fractal(fractal, pixels, SDL_GetWindowSurface(window)->pixels);
			gettimeofday(&end, NULL);
			printf("%s: %ld s %d us\n", is_avx ? "avx" : "classic", end.tv_sec - start.tv_sec, abs(end.tv_usec - start.tv_usec));
			render_metadata(window, fractal, pixels);
			SDL_UpdateWindowSurface(window);
		}
		SDL_Delay(16);
	}
}

#pragma clang diagnostic pop
