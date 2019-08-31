/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 18:46:14 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/30 21:16:12 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"
#include "ft_printf.h"

#include <unistd.h>
#include <stdio.h>

uint32_t		mandel_avx2(const struct s_fractal *restrict fract,
							struct s_rgba_map *restrict pixels,
							uint32_t x,
							uint32_t y)
{
	const __m256d	twos_lol = _mm256_set1_pd(2.0);
	const __m256d	criteria = _mm256_set1_pd(4.0);
	const __m256d	decrement = _mm256_set1_pd(1.0);
	double			i[4];
	__m256d			iterator;
	__m256d			creal;
	__m256d			cimg;
	__m256d			tmp;
	__m256d			sqr_real;
	__m256d			sqr_img;
	__m256d			cx;
	__m256d			cy;
	__m256d			iterations_mask;
	__m256d			iterations = _mm256_setzero_pd();

	iterator = _mm256_set1_pd(fract->max_iterations);
	creal = _mm256_set_pd((((float)x + 0.0) - pixels->larger_dimension_half + fract->input.shift_x) / (pixels->larger_dimension_quarter + fract->input.scroll_depth),
						  (((float)x - 1.0) - pixels->larger_dimension_half + fract->input.shift_x) / (pixels->larger_dimension_quarter + fract->input.scroll_depth),
						  (((float)x - 2.0) - pixels->larger_dimension_half + fract->input.shift_x) / (pixels->larger_dimension_quarter + fract->input.scroll_depth),
					 	  (((float)x - 3.0) - pixels->larger_dimension_half + fract->input.shift_x) / (pixels->larger_dimension_quarter + fract->input.scroll_depth));
	cimg  = _mm256_set1_pd((((float)(y))    - pixels->larger_dimension_half + fract->input.shift_y) / (pixels->larger_dimension_quarter + fract->input.scroll_depth));
	cx = _mm256_sub_pd(creal, _mm256_set1_pd((double)fract->input.mouse_x / (double)pixels->width));
	cy = _mm256_add_pd(cimg, _mm256_set1_pd((double)fract->input.mouse_y / (double)pixels->height));
	while (true)
	{
		sqr_real = _mm256_mul_pd(creal, creal);
		sqr_img = _mm256_mul_pd(cimg, cimg);
		tmp = _mm256_mul_pd(_mm256_mul_pd(creal, cimg), twos_lol);
		creal = _mm256_add_pd(_mm256_sub_pd(sqr_real, sqr_img), cx);
		cimg = _mm256_add_pd(tmp, cy);
		iterations_mask = _mm256_add_pd(sqr_real, sqr_img) < criteria;
		if (!_mm256_movemask_pd(iterations_mask) || _mm256_movemask_pd(iterator))
		{
			_mm256_store_pd(i, iterations);
			break ;
		}
		iterations = _mm256_blendv_pd(iterations, iterator, iterations_mask);
		iterator = _mm256_sub_pd(iterator, decrement);
	}
	colorize_pixels(pixels, NULL, 4, x + 0, y, (int)i[0],
									 x + 1, y, (int)i[1],
									 x + 2, y, (int)i[2],
									 x + 3, y, (int)i[3]);
	return (0);
}

uint32_t		mandel_pixel(const struct s_fractal *restrict fract,
							struct s_rgba_map *restrict pixels,
							uint32_t x,
							uint32_t y)
{
	struct s_classic_meta	meta;
	double					tmp;
	uint32_t				iter;

	iter = fract->max_iterations;
	meta.creal = (x - pixels->larger_dimension_half) / (pixels->larger_dimension_quarter) / fract->input.scroll_depth;
	meta.cimg  = (y - pixels->larger_dimension_half) / (pixels->larger_dimension_quarter) / fract->input.scroll_depth;
	meta.sqr_real = 0.0;
	meta.sqr_img = 0.0;
	meta.cx = meta.creal - (float)fract->input.mouse_x / pixels->width;
	meta.cy = meta.cimg + (float)fract->input.mouse_y / pixels->height;
	while (iter > 0 && meta.sqr_real + meta.sqr_img < 4.0)
	{
		meta.sqr_real = meta.creal * meta.creal;
		meta.sqr_img = meta.cimg * meta.cimg;
		tmp = 2.0 * meta.creal * meta.cimg;
		meta.creal = meta.sqr_real - meta.sqr_img + meta.cx;
		meta.cimg = tmp + meta.cy;
		iter--;
	}
	return (pixels->map[y * pixels->width + x] =
		0x00 - (((iter + 1) % 18) * 0x08080808));
}
