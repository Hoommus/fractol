/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 18:58:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/02 19:31:45 by vtarasiu         ###   ########.fr       */
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
			(font = TTF_OpenFont(g_font_names[i], 24)))
			return (font);
		i++;
	}
	return (NULL);
}

static void			render_metadata(SDL_Window *window,
									   __unused struct s_fractal *fractal,
									   struct s_rgba_map *pixels)
{
	SDL_Surface		*metadata_surface;
	SDL_Renderer	*renderer;
	SDL_Texture		*texture;
	SDL_Rect		rectangle;

	if (!g_font)
		g_font = choose_font();
	if (!g_font)
	{
		dprintf(2, "No font, lol\n");
		return ;
	}
	metadata_surface = TTF_RenderText_Solid(g_font, "hello", (SDL_Color){0xFF, 0xFF, 0xFF, 0});
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);
	texture = SDL_CreateTextureFromSurface(renderer, metadata_surface);
	SDL_FreeSurface(metadata_surface);
	rectangle.x = pixels->width - 200;
	rectangle.y = pixels->height - 160;
	rectangle.w = 200;
	rectangle.h = 160;
	SDL_RenderCopy(renderer, texture, NULL, &rectangle);
	SDL_RenderPresent(renderer);
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

noreturn void	game_loop(SDL_Window *window, struct s_fractal *fractal, struct s_rgba_map *pixels)
{
	uint32_t	ret;
	bool		is_avx;
	bool		is_mouse_pressed;

	is_avx = true;
	while (true)
	{
		ret = poll_events(window, fractal, pixels);
		if ((ret & UI_FEEDBACK_REDRAW))
		{
			if ((ret & UI_FEEDBACK_AVX))
				is_avx = !is_avx;
			is_mouse_pressed = (ret & UI_FEEDBACK_MOUSE_DOWN);
			struct timeval	start;
			struct timeval	end;
			gettimeofday(&start, NULL);
			if (is_avx)
				calculate_fractal_avx(fractal, pixels, SDL_GetWindowSurface(window)->pixels);
			else
				calculate_fractal(fractal, pixels, SDL_GetWindowSurface(window)->pixels);
			gettimeofday(&end, NULL);
			printf("%s: %ld s %d us\n", is_avx ? "avx" : "classic", end.tv_sec - start.tv_sec, abs(end.tv_usec - start.tv_usec));
			SDL_UpdateWindowSurface(window);
			render_metadata(window, fractal, pixels);
		}
		SDL_Delay(16);
	}
}

#pragma clang diagnostic pop
