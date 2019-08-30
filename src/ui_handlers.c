/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_handlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/26 17:59:44 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/28 18:33:00 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"

#define SCANCODE key.keysym.scancode

uint32_t		poll_events(SDL_Window *window, struct s_fractal *fractal, struct s_rgba_map *pixels)
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
			SDL_Quit();
			exit(0);
		}
//		if (event.window.windowID != SDL_GetWindowID(window))
//			return (feedback);
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
		else if (event.type == SDL_MOUSEMOTION && !fractal->input.fractal_locked)
		{
			fractal->input.mouse_x = event.motion.x;
			fractal->input.mouse_y = event.motion.y;
			feedback |= UI_FEEDBACK_REDRAW;
		}
		else if (event.type == SDL_KEYDOWN && event.SCANCODE == SDL_SCANCODE_A)
			feedback |= UI_FEEDBACK_AVX;
		else if (event.type == SDL_MOUSEWHEEL && !fractal->input.fractal_locked)
			fractal->input.scroll_depth += (event.wheel.y < 0 ? -1 : 1) * event.wheel.y / 100.0;
		else if (event.type == SDL_KEYDOWN && event.SCANCODE == SDL_SCANCODE_L)
			fractal->input.fractal_locked = !fractal->input.fractal_locked;
	}
	return (feedback);
}
