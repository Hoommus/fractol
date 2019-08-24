/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 18:46:14 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/24 17:49:49 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"
#include "ft_printf.h"

uint32_t		mandel_pixel(struct s_fractal *fract, struct s_rgba_map *pixels, uint32_t x, uint32_t y)
{
	float		creal;
	float		cimg;
	float		tmp;
	float		sqrzreal;
	float		sqrzimg;
	int32_t		iter;

	iter = fract->max_iterations;
	creal = x;
	cimg = y;
	while (iter >= 0)
	{
		sqrzreal = creal * creal;
		sqrzimg = cimg * cimg;
		tmp = 2.0 * sqrzreal * sqrzimg;
		creal = sqrzreal - sqrzimg + x;
		cimg = tmp + y;
		if ((creal * creal + cimg * cimg) > 4)
			break ;
		iter--;
	}
	return (pixels->map[y * pixels->width + x] =
		(creal * creal + cimg * cimg) > 4 ? 0 : 0xAAFFFF44);
}

