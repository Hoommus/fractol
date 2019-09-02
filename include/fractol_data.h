/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_data.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 18:59:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/01 12:40:30 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_DATA_H
# define FRACTOL_DATA_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include "fractol_gradients.h"

# define COLOR_RED_MASK   0xFF000000U
# define COLOR_GREEN_MASK 0x00FF0000U
# define COLOR_BLUE_MASK  0x0000FF00U
# define COLOR_ALPHA_MASK 0x000000FFU

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
		bool		fractal_locked;
		int			mouse_x;
		int			mouse_y;

		double		shift_x;
		double		shift_y;

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
	short				width;
	short				height;

	short				larger_dimension;
	float				larger_dimension_half;
	float				larger_dimension_quarter;

	uint32_t			*map;
	struct s_pixel_meta	*map_metadata;
} __attribute__((aligned(32)));

enum				e_color
{
	COLOR_WHITE  = 0xFFFFFF00,
	COLOR_100_BLACK  = 0x00000000,
	COLOR_RED    = 0xFF000000,
	COLOR_GREEN  = 0x00FF0000,
	COLOR_BLUE   = 0x0000FF00,
	COLOR_YELLOW = 0x00FFFF00,
	COLOR_CHAOS_BLACK = 0x00152200,
	COLOR_ULTRAMARINE = 0x120A8F00,
	COLOR_ULTRAMARINES_BLUE = 0x0066B300,
	COLOR_GOLDEN_YELLOW = 0xFFC20E00,
	COLOR_MACHARIUS_SOLAR_ORANGE = 0xB9402300,
	COLOR_BLOOD_RED = 0xD2223E00,
	COLOR_DARK_ANGEL_GREEN = 0x19553C00,
	COLOR_HAWK_TURQUOISE = 0x00819400,
	COLOR_LICHE_PURPLE = 0x2C2D8B00,
	COLOR_WARLOCK_PURPLE = 0xA43E8B00,
};

#endif
