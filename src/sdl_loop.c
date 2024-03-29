/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_loop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 18:58:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/29 15:51:57 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include "fractol_data.h"
#include "fractals.h"
#include <stdnoreturn.h>
#include <time.h>
#include <sys/time.h>

static TTF_Font *g_font;
static const char *g_font_names[] = {
		"resources/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf",
		NULL
};

static TTF_Font *choose_font(void) {
	int i;
	TTF_Font *font;

	i = 0;
	while (g_font_names[i]) {
		if (access(g_font_names[i], F_OK | R_OK) == 0 &&
			(font = TTF_OpenFont(g_font_names[i], 14)))
			return (font);
		i++;
	}
	return (NULL);
}

void render_metadata(SDL_Window *window,
					 struct s_fractal *fractal,
					 struct s_rgba_map *pixels) {
	SDL_Surface *metadata_surface;
	char str[1024];
	SDL_Rect rect;

	if (!g_font)
		g_font = choose_font();
	if (!g_font) {
		dprintf(2, "No font, lol\n");
		return;
	}
	ft_bzero(str, sizeof(str));
	snprintf(str, sizeof(str),
			 "(%d, %d, i = %d)\ncolor: %.8x\nscale: %lf\n"
			 "x [%lf, %lf]\ny [%lf, %lf]\ncx = %lf\ncy = %lf\n",
			 fractal->input.mouse_x,
			 fractal->input.mouse_y,
			 pixels->map_metadata[fractal->input.mouse_y * pixels->width + fractal->input.mouse_x].iteration,
			 pixels->map[fractal->input.mouse_y * pixels->width + fractal->input.mouse_x],
			 fractal->input.scroll_depth,
			 fractal->input.x_min,
			 fractal->input.x_max,
			 fractal->input.y_min,
			 fractal->input.y_max,
			 fractal->input.factor_cx,
			 fractal->input.factor_cy);
	TTF_SetFontHinting(g_font, TTF_HINTING_NORMAL);
	metadata_surface = TTF_RenderText_Blended_Wrapped(g_font, str,
													  (SDL_Color) {0xFF, 0xFF, 0xFF, 0}, pixels->width / 3);
	rect = (SDL_Rect) {0, 0, pixels->width / 4, metadata_surface->h + 10};
	SDL_FillRect(SDL_GetWindowSurface(window), &rect, 0x88000000);
	SDL_BlitSurface(metadata_surface, NULL, SDL_GetWindowSurface(window), NULL);
	SDL_FreeSurface(metadata_surface);
}

noreturn void sdl_game_loop(SDL_Window *window,
							struct s_fractal *fractal,
							struct s_rgba_map *pixels,
							const struct s_options *restrict options) {
	void *window_pixels;
	struct timeval start;
	struct timeval end;
	uint32_t ret;

	fractal->input.is_avx = true;
	while (true) {
		window_pixels = SDL_GetWindowSurface(window)->pixels;
		SDL_Delay(10);
		ret = poll_events(window, fractal, pixels, options);
		if (!(ret & UI_FEEDBACK_REDRAW))
			continue;
		if ((ret & UI_FEEDBACK_AVX))
			fractal->input.is_avx = !fractal->input.is_avx;
		if (!fractal->input.locked) {
			if (options->opts & OPTION_VERBOSE)
				gettimeofday(&start, NULL);
			if (options->opts & OPTION_THREADED)
				calculate_fractal_threaded(fractal, pixels, window_pixels, options->threads);
			else if (fractal->input.is_avx)
				calculate_fractal_avx(fractal, pixels, window_pixels);
			else
				calculate_fractal(fractal, pixels, window_pixels);
			if (options->opts & OPTION_VERBOSE) {
				gettimeofday(&end, NULL);
				printf("%s: %ld s %ld us\n",
						  fractal->input.is_avx ? "avx" : "classic",
						  end.tv_sec - start.tv_sec, ABS(end.tv_usec - start.tv_usec));
			}
		}
		render_metadata(window, fractal, pixels);
		SDL_UpdateWindowSurface(window);
	}
}
