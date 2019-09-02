/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 18:46:14 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/01 18:05:30 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"
#include "ft_printf.h"

#include <unistd.h>
#include <stdio.h>

#define SET1(a)   _mm256_set1_pd((a))
#define SUB(a, b) _mm256_sub_pd((a), (b))
#define ADD(a, b) _mm256_add_pd((a), (b))

// Seems like this function has no impact on performance.
// At least with -O2 flag.
static inline void	mandel_init_registers(struct s_avx_data *m,
										struct s_rgba_map *restrict map,
										uint32_t x,
										uint32_t y)
{
	const struct s_fractal	*fract = m->fractal;

	m->iters = _mm256_setzero_pd();
	m->iter = _mm256_set1_pd(fract->max_iterations);
	m->creal = _mm256_set_pd(
		(((double)x - 0.0) - map->larger_dimension_half + fract->input.shift_x) /
		(map->larger_dimension_quarter + fract->input.scroll_depth),
		(((double)x - 1.0) - map->larger_dimension_half + fract->input.shift_x) /
		(map->larger_dimension_quarter + fract->input.scroll_depth),
		(((double)x - 2.0) - map->larger_dimension_half + fract->input.shift_x) /
		(map->larger_dimension_quarter + fract->input.scroll_depth),
		(((double)x - 3.0) - map->larger_dimension_half + fract->input.shift_x) /
		(map->larger_dimension_quarter + fract->input.scroll_depth));
	m->cimg  = _mm256_set1_pd(((
		(double)y) - map->larger_dimension_half + fract->input.shift_y) /
		(map->larger_dimension_quarter + fract->input.scroll_depth));
	m->cx = SUB(m->creal, SET1((double)fract->input.mouse_x / map->width));
	m->cy = ADD(m->cimg, SET1((double)fract->input.mouse_y / map->height));
	m->iters_mask = _mm256_set1_pd(0.0);
}

#define MOVM(a)   _mm256_movemask_pd((a))

uint32_t				mandel_avx2(const struct s_fractal *restrict fract,
										struct s_rgba_map *restrict pixels,
										uint32_t x,
										uint32_t y)
{
	double				i[4];
	struct s_avx_data	d;

	d.fractal = fract;
	mandel_init_registers(&d, pixels, x, y);
	while (true)
	{
		d.sqr_real = _mm256_mul_pd(d.creal, d.creal);
		d.sqr_img = _mm256_mul_pd(d.cimg, d.cimg);
		d.tmp = _mm256_mul_pd(_mm256_mul_pd(d.creal, d.cimg), SET1(2.0));
		d.creal = _mm256_add_pd(_mm256_sub_pd(d.sqr_real, d.sqr_img), d.cx);
		d.cimg = _mm256_add_pd(d.tmp, d.cy);
		d.iters_mask = _mm256_add_pd(d.sqr_real, d.sqr_img) < SET1(4.0);
		d.iters = _mm256_blendv_pd(d.iters, d.iter, d.iters_mask);
		d.iter = _mm256_sub_pd(d.iter, _mm256_set1_pd(1.0));
		if (!MOVM(d.iters_mask) || _mm256_movemask_pd(d.iter))
			break ;
	}
	_mm256_store_pd(i, d.iters);
	colorize_pixels(pixels, fract->gradient_map, 4, x + 0, y, (int)i[0], x + 1, y, (int)i[1],
									x + 2, y, (int)i[2], x + 3, y, (int)i[3]);
	return (0);
}

uint32_t				mandel_pixel(const struct s_fractal *restrict fract,
							struct s_rgba_map *restrict pixels,
							uint32_t x,
							uint32_t y)
{
	struct s_classic_data	data;
	double					tmp;
	uint32_t				iter;

	iter = fract->max_iterations;
	data.creal = ((float)x - pixels->larger_dimension_half + fract->input.shift_x) /
		(pixels->larger_dimension_quarter + fract->input.scroll_depth);
	data.cimg  = ((float)y - pixels->larger_dimension_half + fract->input.shift_y) /
		(pixels->larger_dimension_quarter + fract->input.scroll_depth);
	data.sqr_real = 0.0;
	data.sqr_img = 0.0;
	data.cx = data.creal - (float)fract->input.mouse_x / pixels->width;
	data.cy = data.cimg + (float)fract->input.mouse_y / pixels->height;
	while (iter > 0 && data.sqr_real + data.sqr_img < 4.0)
	{
		data.sqr_real = data.creal * data.creal;
		data.sqr_img = data.cimg * data.cimg;
		tmp = 2.0 * data.creal * data.cimg;
		data.creal = data.sqr_real - data.sqr_img + data.cx;
		data.cimg = tmp + data.cy;
		iter--;
	}
	colorize_pixels(pixels, fract->gradient_map, 1, x, y, iter);
	return (0);
}
