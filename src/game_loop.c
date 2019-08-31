/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 18:58:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/30 20:47:27 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include "fractol_data.h"
#include "fractals.h"
#include <stdnoreturn.h>
#include <time.h>
#include <sys/time.h>

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
			if ((ret & UI_FEEDBACK_MOUSE_DOWN))
				is_mouse_pressed = true;
			if ((ret & UI_FEEDBACK_MOUSE_UP))
				is_mouse_pressed = false;
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
		}
		SDL_Delay(16);
	}
}

#pragma clang diagnostic pop
