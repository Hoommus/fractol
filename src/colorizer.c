/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colorizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 19:38:52 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/01 17:36:00 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_gradients.h"
#include "fractol_common.h"

/*
** Function takes arguments in (x, y, color) tuples. arg_points must contain a
** number of such tuples in variable argument list.
** TODO: make this comment better
*/

void		colorize_pixels(struct s_rgba_map *pixels,
							struct s_gradient *gradient_map,
							int arg_points,
							...)
{
	int			i;
	uint32_t	x;
	uint32_t	y;
	uint32_t	iteration;
	va_list		args;

	va_start(args, arg_points);
	i = 0;
	while (i < arg_points)
	{
		x = va_arg(args, uint32_t);
		y = va_arg(args, uint32_t);
		iteration = va_arg(args, int);
		if (gradient_map && gradient_map->interpolated_colors_cache)
			pixels->map[y * pixels->width + x] =
				gradient_map->interpolated_colors_cache[gradient_map->is_reverse ? gradient_map->max_iterations - iteration : iteration];
		else
			pixels->map[y * pixels->width + x] = grad_get_iter_color(gradient_map, iteration);
		pixels->map_metadata[y * pixels->width + x].iteration = iteration;
		i++;
	}
	va_end(args);
}
