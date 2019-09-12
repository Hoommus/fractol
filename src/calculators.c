/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculators.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:19:03 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/12 19:54:47 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"

void	precalculate_factors(struct s_fractal *fractal, struct s_rgba_map *pixels)
{
	pixels->larger_dimension = pixels->width > pixels->height ? pixels->width : pixels->height;
	pixels->larger_dimension_half = pixels->larger_dimension / 2.0;
	pixels->larger_dimension_quarter = pixels->larger_dimension / 4.0;
	fractal->input.factor_cx = (fractal->input.mouse_cx - pixels->larger_dimension_half) / pixels->width;
	fractal->input.factor_cy = (fractal->input.mouse_cy - pixels->larger_dimension_half) / pixels->height;
	fractal->input.factor_scale_x = pixels->larger_dimension_quarter + fractal->input.scroll_depth;
	fractal->input.factor_scale_y = fractal->input.factor_scale_x;
	fractal->input.factor_shift_x = pixels->larger_dimension_half + fractal->input.shift_x;
	fractal->input.factor_shift_y = pixels->larger_dimension_half + fractal->input.shift_y;
	fractal->input.factor_cx /= ceil(fractal->input.factor_scale_x / 1000.0);
	fractal->input.factor_cy /= ceil(fractal->input.factor_scale_y / 1000.0);
}

void	calculate_fractal_avx(struct s_fractal *fractal,
	struct s_rgba_map *pixels, void *display_pixels)
{
	static struct s_fractal	*current;
	const t_fract_calc		func = current ? current->calculator_avx : NULL;
	int32_t					x;
	int32_t					y;

	if (current != fractal)
	{
		current = fractal;
		ft_memcpy((void *)&func, &(current->calculator), sizeof(current->calculator));
	}
	precalculate_factors(fractal, pixels);
	y = 0;
	while (y < pixels->height)
	{
		x = 0;
		while (x < pixels->width)
		{
			func(current, pixels, x, y);
			x += 4;
		}
		y += 1;
	}
	__builtin_memcpy(display_pixels, pixels->map,
					 pixels->height * pixels->width * sizeof(uint32_t));
}

void	calculate_fractal(struct s_fractal *fractal,
		struct s_rgba_map *pixels, void *display_pixels)
{
	static struct s_fractal	*current;
	const t_fract_calc		func = current ? current->calculator : NULL;
	int32_t					x;
	int32_t					y;

	if (current != fractal)
	{
		current = fractal;
		ft_memcpy((void *)&func, &(current->calculator), sizeof(current->calculator));
	}
	precalculate_factors(fractal, pixels);
	x = -1;
	while (++x < pixels->width)
	{
		y = -1;
		while (++y < pixels->height)
			func(current, pixels, x, y);
	}
	__builtin_memcpy(display_pixels, pixels->map,
					 pixels->height * pixels->width * sizeof(uint32_t));
}
