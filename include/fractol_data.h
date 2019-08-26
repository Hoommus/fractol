/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_data.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 18:59:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/26 17:56:51 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_DATA_H
# define FRACTOL_DATA_H

# include <stdint.h>
# include <stddef.h>

struct			s_fractal;
struct			s_rgba_map;

typedef uint32_t	(*t_fract_calc)(struct s_fractal *, struct s_rgba_map *, uint32_t, uint32_t);

struct			s_fractal
{
	uint64_t			flags;
	uint32_t			max_iterations;

	t_fract_calc		calculator;
	t_fract_calc		calculator_avx;

	void				*color_function;

	struct				s_input
	{
		int			mouse_x;
		int			mouse_y;

		int			shift_x;
		int			shift_y;

		double		scroll_depth;
		int			scroll_last_direction;
	}					input;
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

#endif
