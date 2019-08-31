/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_data.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 18:59:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/30 21:08:32 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_DATA_H
# define FRACTOL_DATA_H

# include <stdint.h>
# include <stddef.h>

struct				s_fractal;
struct				s_rgba_map;

typedef uint32_t	(*t_fract_calc)(const struct s_fractal *restrict, struct s_rgba_map *restrict, uint32_t, uint32_t);

typedef struct		s_fractal
{
	uint64_t			flags;
	uint32_t			max_iterations;

	t_fract_calc		calculator;
	t_fract_calc		calculator_avx;

	void				*color_function;

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

enum				e_gradient_type
{
	GRADIENT_LINEAR,
	GRADIENT_RADIAL,
	GRADIENT_REFLECTED,
	GRADIENT_ANGLED
};

typedef struct		s_gradient_point
{
	uint32_t				color;
	uint8_t					location;
	struct s_gradient_point	*next;
}					t_gradient_point;

struct				s_gradient
{
	double					scale;
	double					angle; // gradient map won't use this, probably
	bool					is_reverse;
	enum e_gradient_type	type;
	t_gradient_point		*points;
};

struct				s_command
{
	const char			*name;
	struct s_fractal	temp_late;
};

struct				s_pixel
{
	uint32_t	color;
	uint32_t	iteration;
};

struct				s_rgba_map
{
	short		width;
	short		height;

	short		larger_dimension;
	float		larger_dimension_half;
	float		larger_dimension_quarter;

	uint32_t	*map;
} __attribute__((aligned(32)));

#endif
