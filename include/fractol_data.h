/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_data.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 18:59:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/12 20:05:26 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_DATA_H
# define FRACTOL_DATA_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include "fractol_gradients.h"

struct				s_fractal;
struct				s_rgba_map;

typedef uint32_t	(*t_fract_calc)(const struct s_fractal *restrict, struct s_rgba_map *restrict, uint32_t, uint32_t);

typedef struct		s_fractal
{
	uint64_t			flags;
	uint32_t			max_iterations;

	t_fract_calc		calculator;
	t_fract_calc		calculator_avx;

	struct s_gradient	*gradient_map;

	struct				s_input
	{
		bool		locked;
		int			mouse_x;
		int			mouse_y;

		int			mouse_cx;
		int			mouse_cy;

		double		shift_x;
		double		shift_y;

		double		factor_cx;
		double		factor_cy;

		double		factor_scale_x;
		double		factor_scale_y;

		double		factor_shift_x;
		double		factor_shift_y;

		double		scroll_depth;
	}					input;
	struct s_rgba_map	*source;
}					t_fractal;

struct				s_command
{
	const char			*name;
	struct s_fractal	temp_late;
};

struct				s_pixel_meta
{
	uint32_t	iteration;
};

struct				s_rgba_map
{
	int					width;
	int					height;

	short				larger_dimension;
	float				larger_dimension_half;
	float				larger_dimension_quarter;

	uint32_t			*map;
	struct s_pixel_meta	*map_metadata;
};

enum				e_color
{
	COLOR_WHITE  = 0x00FFFFFF,
	COLOR_100_BLACK  = 0x0,
	COLOR_RED    = 0xFF0000,
	COLOR_GREEN  = 0x00FF00,
	COLOR_BLUE   = 0x0000FF,
	COLOR_YELLOW = 0xFFFF00,
	COLOR_CHAOS_BLACK = 0x001522,
	COLOR_ULTRAMARINE = 0x120A8F,
	COLOR_ULTRAMARINES_BLUE = 0x0066B3,
	COLOR_GOLDEN_YELLOW = 0xFFC20E,
	COLOR_MACHARIUS_SOLAR_ORANGE = 0xB94023,
	COLOR_BLOOD_RED = 0xD2223E,
	COLOR_DARK_ANGEL_GREEN = 0x19553C,
	COLOR_HAWK_TURQUOISE = 0x008194,
	COLOR_LICHE_PURPLE = 0x2C2D8B,
	COLOR_WARLOCK_PURPLE = 0xA43E8B,
};

#endif
