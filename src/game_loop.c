/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 18:58:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/28 18:33:00 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include "fractol_data.h"
#include "fractals.h"
#include <stdnoreturn.h>

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
			if (is_avx)
				calculate_fractal_avx(fractal, pixels, SDL_GetWindowSurface(window)->pixels);
			else
				calculate_fractal(fractal, pixels, SDL_GetWindowSurface(window)->pixels);
			SDL_UpdateWindowSurface(window);
		}
		SDL_Delay(16);
	}
}

#pragma clang diagnostic pop
