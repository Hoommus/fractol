/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colorizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 19:38:52 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/30 14:23:35 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"

/*
** Function takes arguments in (x, y, color) tuples. arg_points must contain a
** number of such tuples in variable argument list.
** TODO: make this comment better
*/

void		colorize_pixels(struct s_rgba_map *pixels,
							struct s_gradient *map,
							int arg_points,
							...)
{
	int			i;
	uint32_t	x;
	uint32_t	y;
	uint32_t	iteration;
	va_list		args;

	map = NULL; // later
	va_start(args, arg_points);
	i = 0;
//	printf("painting ");
	while (i < arg_points)
	{
		x = va_arg(args, uint32_t);
		y = va_arg(args, uint32_t);
		iteration = va_arg(args, int);
//		printf("x: % 3d y: % 3d with %x ", x, y, iteration);
		pixels->map[y * pixels->width + x] = 0x00 - (((iteration + 1) % 18) * 0x08080808);
		i++;
	}
//	printf("\n");
//	fflush(stdout);
	va_end(args);
}

