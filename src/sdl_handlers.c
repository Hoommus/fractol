/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 17:59:44 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/29 15:57:36 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"

#define SCANCODE key.keysym.scancode

static inline uint32_t	sdl_keydown(SDL_Scancode scancode,
									struct s_fractal *restrict fractal,
									const struct s_options *restrict options)
{
	uint32_t	feedback;

	feedback = UI_FEEDBACK_REDRAW;
	if (scancode == SDL_SCANCODE_LEFT)
		fractal->input.shift_x -= 5.0 * (floor(fractal->input.scroll_depth / 800.0) + 1.0);
	else if (scancode == SDL_SCANCODE_RIGHT)
		fractal->input.shift_x += 5.0 * (floor(fractal->input.scroll_depth / 800.0) + 1.0);
	else if (scancode == SDL_SCANCODE_DOWN)
		fractal->input.shift_y -= 5.0 * (floor(fractal->input.scroll_depth / 800.0) + 1.0);
	else if (scancode == SDL_SCANCODE_UP)
		fractal->input.shift_y += 5.0 * (floor(fractal->input.scroll_depth / 800.0) + 1.0);
	else if (scancode == SDL_SCANCODE_A)
		feedback |= UI_FEEDBACK_AVX;
	else if (scancode == SDL_SCANCODE_L && !(options->opts & OPTION_NO_USER_INPUT))
		fractal->input.locked = !fractal->input.locked;
	else if (scancode == SDL_SCANCODE_R)
		fractal->gradient_map->is_reverse = !fractal->gradient_map->is_reverse;
	else if (scancode == SDL_SCANCODE_EQUALS || scancode == SDL_SCANCODE_KP_PLUS ||
		scancode == SDL_SCANCODE_MINUS || scancode == SDL_SCANCODE_KP_MINUS)
	{
		if (scancode == SDL_SCANCODE_EQUALS || scancode == SDL_SCANCODE_KP_PLUS)
			fractal->max_iterations += !!(fractal->max_iterations < UINT32_MAX);
		if (scancode == SDL_SCANCODE_MINUS || scancode == SDL_SCANCODE_KP_MINUS)
			fractal->max_iterations -= !!(fractal->max_iterations > 0);
		fractal->gradient_map->max_iterations = fractal->max_iterations;
		grad_cache_colors(fractal->gradient_map);
	}
	else if (scancode >= SDL_SCANCODE_1 && scancode <= SDL_SCANCODE_0)
	{
		fractal->gradient_map = grad_from_table(scancode - 30);
		fractal->gradient_map->max_iterations = fractal->max_iterations;
		grad_cache_colors(fractal->gradient_map);
	}
	return (feedback);
}

uint32_t
poll_events(SDL_Window *restrict window,
	struct s_fractal *restrict fractal,
	struct s_rgba_map *restrict pixels,
	const struct s_options *restrict options)
{
	SDL_Event	e;
	uint32_t	feedback;

	feedback = UI_FEEDBACK_NEUTRAL;
	while (SDL_PollEvent(&e))
		if (e.type == SDL_QUIT ||
			(e.type == SDL_KEYDOWN && e.SCANCODE == SDL_SCANCODE_ESCAPE) ||
			(e.type == SDL_KEYDOWN && e.SCANCODE == SDL_SCANCODE_Q))
		{
			SDL_DestroyWindow(window);
			exit(0);
		}
		else if (e.window.windowID != SDL_GetWindowID(window))
			return (feedback);
		else if (e.type == SDL_KEYDOWN)
			feedback |= sdl_keydown(e.key.keysym.scancode, fractal, options);
		else if (e.type == SDL_WINDOWEVENT &&
				(e.window.event == SDL_WINDOWEVENT_RESIZED ||
				e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED))
		{
			SDL_GetWindowSize(window, &(pixels->width), &(pixels->height));
			free(pixels->map);
			free(pixels->map_metadata);
			pixels->map = ft_memalloc(sizeof(uint32_t) * pixels->width * pixels->height);
			pixels->map_metadata = ft_memalloc(sizeof(uint32_t) * pixels->width * pixels->height);
			feedback |= UI_FEEDBACK_REDRAW;
		}
		else if (e.type == SDL_MOUSEMOTION)
		{
			fractal->input.mouse_cx = e.motion.x;
			fractal->input.mouse_cy = e.motion.y;
			fractal->input.mouse_x = e.motion.x;
			fractal->input.mouse_y = e.motion.y;
			feedback |= UI_FEEDBACK_REDRAW;
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_GetMouseState(&(fractal->input.mouse_cx), &(fractal->input.mouse_cy));
			feedback |= UI_FEEDBACK_REDRAW;
		}
		else if (e.type == SDL_MOUSEWHEEL && !fractal->input.locked)
		{
			fractal->input.scroll_depth += e.wheel.y * (5.0 + round(fractal->input.scroll_depth / 500.0));
			feedback |= UI_FEEDBACK_REDRAW;
		}
	return (feedback);
}
