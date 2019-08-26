/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 18:58:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/26 19:08:21 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include "fractol_data.h"
#include "fractals.h"

void	game_loop(SDL_Window *window, struct s_fractal *fractal, struct s_rgba_map *pixels)
{
	uint32_t	ret;

	while (true)
	{
		ret = poll_events(window, fractal, pixels);
//		if (!ret)
//			return ;
		if ((ret & UI_FEEDBACK_REDRAW) && (ret & UI_FEEDBACK_AVX))
			calculate_fractal_avx(fractal, pixels, SDL_GetWindowSurface(window)->pixels);
		else if ((ret & UI_FEEDBACK_REDRAW))
			calculate_fractal(fractal, pixels, SDL_GetWindowSurface(window)->pixels);
		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);
	}
}
