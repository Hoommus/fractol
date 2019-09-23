/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 17:59:44 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/23 17:58:43 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"

#define SCANCODE key.keysym.scancode

static inline uint32_t	sdl_keydown(SDL_Scancode scancode,
								struct s_fractal *fractal)
{
	uint32_t	feedback;

	feedback = UI_FEEDBACK_REDRAW;
	if (scancode == SDL_SCANCODE_LEFT)
		fractal->input.shift_x -= 5.0 * (floor(fractal->input.scroll_depth / 800.0) + 1.0);
	if (scancode == SDL_SCANCODE_RIGHT)
		fractal->input.shift_x += 5.0 * (floor(fractal->input.scroll_depth / 800.0) + 1.0);
	if (scancode == SDL_SCANCODE_DOWN)
		fractal->input.shift_y -= 5.0 * (floor(fractal->input.scroll_depth / 800.0) + 1.0);
	if (scancode == SDL_SCANCODE_UP)
		fractal->input.shift_y += 5.0 * (floor(fractal->input.scroll_depth / 800.0) + 1.0);
	if (scancode == SDL_SCANCODE_A)
		feedback |= UI_FEEDBACK_AVX;
	if (scancode == SDL_SCANCODE_L)
		fractal->input.locked = !fractal->input.locked;
	if (scancode == SDL_SCANCODE_R)
		fractal->gradient_map->is_reverse = !fractal->gradient_map->is_reverse;
	if (scancode == SDL_SCANCODE_EQUALS || scancode == SDL_SCANCODE_KP_PLUS ||
		scancode == SDL_SCANCODE_MINUS || scancode == SDL_SCANCODE_KP_MINUS)
	{
		if (scancode == SDL_SCANCODE_EQUALS || scancode == SDL_SCANCODE_KP_PLUS)
			fractal->max_iterations += !!(fractal->max_iterations < UINT32_MAX);
		if (scancode == SDL_SCANCODE_MINUS || scancode == SDL_SCANCODE_KP_MINUS)
			fractal->max_iterations -= !!(fractal->max_iterations > 0);
		fractal->gradient_map->max_iterations = fractal->max_iterations;
		grad_cache_colors(fractal->gradient_map);
	}
	return (feedback);
}

uint32_t				poll_events(SDL_Window *window,
									struct s_fractal *fractal,
									struct s_rgba_map *pixels)
{
	SDL_Event	event;
	uint32_t	feedback;

	feedback = UI_FEEDBACK_NEUTRAL;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT ||
			(event.type == SDL_KEYDOWN && event.SCANCODE == SDL_SCANCODE_ESCAPE) ||
			(event.type == SDL_KEYDOWN && event.SCANCODE == SDL_SCANCODE_Q))
		{
			SDL_DestroyWindow(window);
			exit(0);
		}
		if (event.window.windowID != SDL_GetWindowID(window))
			return (feedback);
		if (event.type == SDL_KEYDOWN)
			feedback |= sdl_keydown(event.key.keysym.scancode, fractal);
		if (event.type == SDL_WINDOWEVENT &&
			(event.window.event == SDL_WINDOWEVENT_RESIZED ||
			event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED))
		{
			SDL_GetWindowSize(window, &(pixels->width), &(pixels->height));
			free(pixels->map);
			free(pixels->map_metadata);
			pixels->map = ft_memalloc(sizeof(uint32_t) * pixels->width * pixels->height);
			pixels->map_metadata = ft_memalloc(sizeof(uint32_t) * pixels->width * pixels->height);
			feedback |= UI_FEEDBACK_REDRAW;
		}
		if (event.type == SDL_MOUSEMOTION)
		{
			fractal->input.mouse_cx = event.motion.x;
			fractal->input.mouse_cy = event.motion.y;
			fractal->input.mouse_x = event.motion.x;
			fractal->input.mouse_y = event.motion.y;
			feedback |= UI_FEEDBACK_REDRAW;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_GetMouseState(&(fractal->input.mouse_cx), &(fractal->input.mouse_cy));
			feedback |= UI_FEEDBACK_REDRAW;
		}
		if (event.type == SDL_MOUSEWHEEL && !fractal->input.locked)
		{
			fractal->input.scroll_depth += event.wheel.y * (5.0 + round(fractal->input.scroll_depth / 1000.0));
			feedback |= UI_FEEDBACK_REDRAW;
		}
	}
	return (feedback);
}
