/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractals.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 13:48:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/30 20:38:34 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTALS_H
# define FRACTALS_H

# include <limits.h>
# include <stdbool.h>
# include <math.h>

# include <immintrin.h>

# include "fractol_data.h"

# define FLAG_INTERACTIVE      1
# define FLAG_NEEDS_EXPORT     2
# define FLAG_FORCE_AVX        4

struct			s_avx_meta
{
	__m256d			iterator;
	__m256d			creal;
	__m256d			cimg;
	__m256d			tmp;
	__m256d			sqr_real;
	__m256d			sqr_img;
	__m256d			cx;
	__m256d			cy;
	__m256d			iterations_mask;
	__m256d			iterations;
} __attribute__((aligned(32)));

struct			s_classic_meta
{
	double		creal;
	double		cimg;
	double		cx;
	double		cy;
	double		sqr_real;
	double		sqr_img;
};

uint32_t		mandel_pixel(const struct s_fractal *restrict fract,
							struct s_rgba_map *pixels,
							uint32_t x,
							uint32_t y);
uint32_t		mandel_avx2(const struct s_fractal *restrict fract,
							struct s_rgba_map *restrict pixels,
							uint32_t x,
							uint32_t y);

void			colorize_pixels(struct s_rgba_map *pixels,
								struct s_gradient *map,
								int arg_points,
								...);

void			calculate_fractal_avx(struct s_fractal *fractal,
				struct s_rgba_map *pixels, void *sdl_pixels);
void			calculate_fractal(struct s_fractal *fractal,
								struct s_rgba_map *pixels, void *sdl_pixels);


#endif
