/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/22 18:46:14 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/26 17:37:11 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractals.h"
#include "ft_printf.h"

#include <unistd.h>
#include <stdio.h>

uint32_t		mandel_avx2(struct s_fractal *fract, struct s_rgba_map *pixels, uint32_t x, uint32_t y)
{
	const __m256d	twos_lol = _mm256_set1_pd(2.0);
	const __m256d	criteria = _mm256_set1_pd(4.0);
	__unused const __m256d	increment = _mm256_set1_pd(1.0);
	__unused __m256i			iterator = _mm256_set1_epi64x(0) ;
	__m256d			creal;
	__m256d			cimg;
	__m256d			tmp;
	__m256d			sqr_real;
	__m256d			sqr_img;
	__m256d			cx;
	__m256d			cy;

	double			result[4];

	int32_t			iter;

	creal = _mm256_set_pd((x + 0 - pixels->width  / 2.0) / ((pixels->width)  / 4.0),
						  (x + 1 - pixels->width  / 2.0) / ((pixels->width)  / 4.0),
						  (x + 2 - pixels->width  / 2.0) / ((pixels->width)  / 4.0),
						  (x + 3 - pixels->width  / 2.0) / ((pixels->width)  / 4.0));
	cimg  = _mm256_set_pd((y + 0 - pixels->height / 2.0) / ((pixels->height) / 4.0),
						  (y + 1 - pixels->height / 2.0) / ((pixels->height) / 4.0), 
						  (y + 2 - pixels->height / 2.0) / ((pixels->height) / 4.0), 
						  (y + 3 - pixels->height / 2.0) / ((pixels->height) / 4.0));
	cx = _mm256_movedup_pd(creal);
	cy = _mm256_movedup_pd(cimg);
	iter = fract->max_iterations;
	while (iter > 0)
	{
		sqr_real = _mm256_mul_pd(creal, creal);
		sqr_img = _mm256_mul_pd(cimg, cimg);
		tmp = _mm256_mul_pd(_mm256_mul_pd(creal, cimg), twos_lol);
		creal = _mm256_add_pd(_mm256_sub_pd(sqr_real, sqr_img), cx);
		cimg = _mm256_add_pd(tmp, cy);
		_mm256_store_pd(result, _mm256_cmp_pd(_mm256_add_pd(sqr_real, sqr_img), criteria, _CMP_LT_OS));
		//printf("%lf %lf %lf %lf\n", result[0], result[1], result[2], result[3]);
		//usleep(250000);
		iter--;
	}
	return (0);
}

uint32_t		mandel_pixel(struct s_fractal *fract, struct s_rgba_map *pixels, uint32_t x, uint32_t y)
{
	double		creal;
	double		cimg;
	double		sqr_real;
	double		sqr_img;
	double		tmp;
	int32_t		iter;

	iter = fract->max_iterations;
	creal = (x - pixels->width / 2.0) / (pixels->width / 4.0);
	cimg = (y - pixels->height / 2.0) / (pixels->height / 4.0);
	sqr_real = 0.0;
	sqr_img = 0.0;

	double cx = creal - (float)fract->input.mouse_x / pixels->width;
	double cy = cimg  + (float)fract->input.mouse_y / pixels->height;
	while (iter >= 0 && sqr_real + sqr_img < 4.0)
	{
		sqr_real  = creal * creal;
		sqr_img   = cimg * cimg;
		tmp       = 2.0 * creal * cimg;
		creal     = sqr_real - sqr_img + cx;
		cimg      = tmp + cy;
		iter--;
	}
	return (pixels->map[y * pixels->width + x] =
		0x00 + (((iter + 1) % 18) * 0x08080808));
}
