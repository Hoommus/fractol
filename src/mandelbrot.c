/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 18:46:14 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/25 15:54:13 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"
#include "ft_printf.h"

uint32_t		mandel_pixel(struct s_fractal *fract, struct s_rgba_map *pixels, uint32_t x, uint32_t y)
{
	double		creal;
	double		cimg;
	double		tmp;
	double		sqrzreal;
	double		sqrzimg;
	int32_t		iter;

	iter = fract->max_iterations;
	creal = (x - pixels->width / 2.0) / (pixels->width / 4.0);
	cimg = (y - pixels->height / 2.0) / (pixels->height / 4.0);
	//dprintf(2, "(creal + cimg) = %f %f\n", creal, cimg);
	sqrzreal = 0;
	sqrzimg = 0;

	double cx = creal + (float)fract->input_feedback.mouse_x / pixels->width;
	double cy = cimg;

	while (iter >= 0 && sqrzreal + sqrzimg < 4.0)
	{
		sqrzreal = creal * creal;
		sqrzimg = cimg * cimg;
		tmp = 2.0 * creal * cimg;
		creal = sqrzreal - sqrzimg + cx;
		cimg = tmp + cy;
		iter--;
	}
	//dprintf(2, "iter = %d\n", iter);
	return (pixels->map[y * pixels->width + x] =
		0x00 + (((iter + 1) % 18) * 0x08080808));
}

