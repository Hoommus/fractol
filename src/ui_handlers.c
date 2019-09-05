/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_handlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 17:59:44 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/02 19:34:18 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"

#define SCANCODE key.keysym.scancode

uint32_t			poll_events(SDL_Window *window,
						struct s_fractal *fractal,
						struct s_rgba_map *pixels)
{
	static int	mouse_pressed_x;
	static int	mouse_pressed_y;
	static bool	is_mouse_pressed;
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
			SDL_Quit();
			TTF_Quit();
			exit(0);
		}
		if (event.window.windowID != SDL_GetWindowID(window))
			return (feedback);
		if (event.type == SDL_WINDOWEVENT &&
			(event.window.event == SDL_WINDOWEVENT_RESIZED ||
			event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED))
		{
			pixels->width = event.window.data1;
			pixels->height = event.window.data2;
			free(pixels->map);
			pixels->map = ft_memalloc(sizeof(uint32_t) * pixels->width * pixels->height);
			feedback |= UI_FEEDBACK_REDRAW;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			is_mouse_pressed = true;
			SDL_GetMouseState(&mouse_pressed_x, &mouse_pressed_y);
			feedback |= UI_FEEDBACK_MOUSE_DOWN;
		}
		if (event.type == SDL_MOUSEBUTTONUP)
		{
			is_mouse_pressed = false;
			feedback |= UI_FEEDBACK_MOUSE_UP;
		}
		if (is_mouse_pressed && event.type == SDL_MOUSEMOTION && !fractal->input.fractal_locked)
		{
			fractal->input.shift_x += (mouse_pressed_x > event.motion.x ? 1 : -1) * (event.motion.x) / 100.0;
			fractal->input.shift_y += (mouse_pressed_y > event.motion.y ? 1 : -1) * (event.motion.y) / 100.0;;
			mouse_pressed_x = event.motion.x;
			mouse_pressed_y = event.motion.y;
			feedback |= UI_FEEDBACK_REDRAW;
		}
		if (!is_mouse_pressed && event.type == SDL_MOUSEMOTION && !fractal->input.fractal_locked)
		{
			fractal->input.mouse_x = event.motion.x;
			fractal->input.mouse_y = event.motion.y;
			feedback |= UI_FEEDBACK_REDRAW;
		}
		if (event.type == SDL_KEYDOWN && event.SCANCODE == SDL_SCANCODE_A)
			feedback |= UI_FEEDBACK_AVX;
		if (event.type == SDL_MOUSEWHEEL && !fractal->input.fractal_locked)
		{
			fractal->input.scroll_depth += event.wheel.y * 5.0;
			feedback |= UI_FEEDBACK_REDRAW;
		}
		if (event.type == SDL_KEYDOWN && event.SCANCODE == SDL_SCANCODE_L)
			fractal->input.fractal_locked = !fractal->input.fractal_locked;
		if (event.type == SDL_KEYDOWN && (event.SCANCODE == SDL_SCANCODE_EQUALS || event.SCANCODE == SDL_SCANCODE_KP_PLUS))
		{
			fractal->max_iterations += fractal->max_iterations < UINT32_MAX ? 1 : 0;
			feedback |= UI_FEEDBACK_REDRAW;
		}
		if (event.type == SDL_KEYDOWN && (event.SCANCODE == SDL_SCANCODE_MINUS || event.SCANCODE == SDL_SCANCODE_KP_MINUS))
		{
			fractal->max_iterations -= fractal->max_iterations > 0 ? 1 : 0;
			feedback |= UI_FEEDBACK_REDRAW;
		}
		if (event.type == SDL_KEYDOWN && (event.SCANCODE == SDL_SCANCODE_R))
		{
			fractal->gradient_map->is_reverse = !fractal->gradient_map->is_reverse;
			feedback |= UI_FEEDBACK_REDRAW;
		}
	}
	return (feedback);
}
