/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractals.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 13:48:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/25 12:43:38 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTALS_H
# define FRACTALS_H

# include <stdint.h>
# include <stddef.h>
# include <limits.h>
# include <stdbool.h>
# include <math.h>

struct			s_fractal;
struct			s_rgba_map;

typedef uint32_t	(*t_fract_calc)(struct s_fractal *, struct s_rgba_map *, uint32_t, uint32_t);

struct			s_fractal
{
	uint32_t			max_iterations;

	t_fract_calc		calculator;

	struct				s_input
	{
		int			mouse_x;
		int			mouse_y;

		int			winch_x;
		int			winch_y;

		int			shift_x;
		int			shift_y;
	}					input_feedback;
	struct s_rgba_map	*source;
};

struct			s_command
{
	const char			*name;
	struct s_fractal	temp_late;
};

struct			s_pixel
{
	uint32_t	color;
	uint32_t	iteration;
};

struct			s_rgba_map
{
	short		width;
	short		height;

	uint32_t	*map;
} __attribute__((aligned(32)));





uint32_t		mandel_pixel(struct s_fractal *fract, struct s_rgba_map *pixels,
	uint32_t x, uint32_t y);


#endif
