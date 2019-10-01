/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 18:24:34 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/29 20:59:40 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "fractals.h"
#include "ft_printf.h"

#define SET1(a)   _mm256_set1_pd((a))

static inline void	julia_init_registers(struct s_avx_data *restrict data,
											uint32_t x,
											uint32_t y)
{
	const struct s_fractal	*fract = data->fractal;

	data->iters = _mm256_setzero_pd();
	data->iter = _mm256_set1_pd(fract->max_iterations - 1);
	data->creal = _mm256_set_pd(
		(x - 0. - fract->input.factor_shift_x) / (fract->input.factor_scale_x),
		(x - 1. - fract->input.factor_shift_x) / (fract->input.factor_scale_x),
		(x - 2. - fract->input.factor_shift_x) / (fract->input.factor_scale_x),
		(x - 3. - fract->input.factor_shift_x) / (fract->input.factor_scale_x));
	data->cimg  = _mm256_set1_pd(((y) - fract->input.factor_shift_y) / (fract->input.factor_scale_y));
	data->cx = _mm256_set1_pd(fract->input.factor_cx);
	data->cy = _mm256_set1_pd(fract->input.factor_cy);
	data->iters_mask = _mm256_set1_pd(1);
	data->iters = _mm256_set1_pd(fract->max_iterations - 1);
}

uint32_t			julia_avx2(const struct s_fractal *restrict fract,
								struct s_rgba_map *restrict pixels,
								uint32_t x,
								uint32_t y)
{
	double				i[4];
	struct s_avx_data	d;

	d.fractal = fract;
	julia_init_registers(&d, x, y);
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
		if (!_mm256_movemask_pd(d.iters_mask) || _mm256_movemask_pd(d.iter))
			break ;
	}
	_mm256_store_pd(i, d.iters);
	colorize_pixels(pixels, fract->gradient_map, 4,
					x + 0, y, (int)i[0],
					x + 1, y, (int)i[1],
					x + 2, y, (int)i[2],
					x + 3, y, (int)i[3]);
	return (0);
}

uint32_t			julia_pixel(const struct s_fractal *restrict fract,
								 struct s_rgba_map *restrict pixels,
								 uint32_t x,
								 uint32_t y)
{
	struct s_classic_data	data;
	double					tmp;
	uint32_t				iter;

	iter = fract->max_iterations;
	data = (struct s_classic_data) {
		.creal = ((float)x - fract->input.factor_shift_x) / (fract->input.factor_scale_x),
		.cimg = ((float)y - fract->input.factor_shift_y) / (fract->input.factor_scale_y),
		.cx = fract->input.factor_cx,
		.cy = fract->input.factor_cy
	};
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
