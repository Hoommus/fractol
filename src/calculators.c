/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculators.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 14:19:03 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/29 17:54:13 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractol_tpool.h>
#include "fractol_common.h"

void	precalculate_factors(struct s_fractal *restrict fractal,
							struct s_rgba_map *restrict pixels)
{
	struct s_input	*input;

	input = &fractal->input;
	pixels->larger_dimension = pixels->width > pixels->height ? pixels->width : pixels->height;
	pixels->larger_dimension_half = pixels->larger_dimension / 2.0;
	pixels->larger_dimension_quarter = pixels->larger_dimension / 4.0;
	input->x_min = input->factor_shift_x / input->factor_scale_x;
	input->x_max = (pixels->width - input->factor_shift_x) / input->factor_scale_x;
	input->y_min = input->factor_shift_y / input->factor_scale_y;
	input->y_max = (pixels->height - input->factor_shift_x) / input->factor_scale_x;
	input->factor_cx = (input->mouse_cx - pixels->larger_dimension_half) / pixels->width;
	input->factor_cy = (input->mouse_cy - pixels->larger_dimension_half) / pixels->height;
	input->factor_scale_x = pixels->larger_dimension_quarter + input->scroll_depth;
	input->factor_scale_y = input->factor_scale_x;
	input->factor_shift_x = pixels->larger_dimension_half + input->shift_x;
	input->factor_shift_y = pixels->larger_dimension_half + input->shift_y;
	input->factor_cx /= ceil(input->factor_scale_x / 1000.0);
	input->factor_cy /= ceil(input->factor_scale_y / 1000.0);
}

void	calculate_fractal_avx(struct s_fractal *fractal,
	struct s_rgba_map *pixels, void *display_pixels)
{
	static struct s_fractal	*current;
	const t_fract_calc		func = current ? current->calc_avx : NULL;
	int32_t					x;
	int32_t					y;

	if (current != fractal)
	{
		current = fractal;
		ft_memcpy((void *)&func, &(current->calc), sizeof(current->calc));
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
	const t_fract_calc		func = current ? current->calc : NULL;
	int32_t					x;
	int32_t					y;

	if (current != fractal)
	{
		current = fractal;
		ft_memcpy((void *)&func, &(current->calc), sizeof(current->calc));
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

void	calculate_fractal_threaded(struct s_fractal *fractal,
									struct s_rgba_map *pixels,
									void *display_pixels,
									uint32_t threads_quantity)
{
	uint32_t				start;
	uint32_t				size;
	uint32_t				total_size;

	start = 0;
	size = pixels->height * pixels->width / threads_quantity;
	total_size = pixels->height * pixels->width;
	precalculate_factors(fractal, pixels);
	while (start < total_size)
	{
		if (total_size - start < size)
			tpool_add_task(&(t_task){fractal, pixels, start,
							total_size - start, 0, 0, 0}, false);
		else
			tpool_add_task(&(t_task){fractal, pixels, start, size, 0, 0, 0}, false);
		start += size;
	}
	tpool_runnwait();
	__builtin_memcpy(display_pixels, pixels->map,
					 pixels->height * pixels->width * sizeof(uint32_t));
}
