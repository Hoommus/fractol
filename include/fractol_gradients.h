/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_gradients.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 12:38:28 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/07 18:08:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_GRADIENTS_H
# define FRACTOL_GRADIENTS_H

#include "fractol_data.h"

enum				e_gradient_type
{
	GRADIENT_LINEAR,
	GRADIENT_RADIAL,
	GRADIENT_REFLECTED,
	GRADIENT_ANGLED
};

typedef struct		s_hsv
{
	float	h;
	float	s;
	float	v;
}					t_hsvl_color;

typedef struct		s_gradient_point
{
	uint32_t				rgba;
	t_hsvl_color			hsvl;
	uint32_t				iteration;
	uint8_t					opacity;
	double					location;
	struct s_gradient_point	*next;
}					t_gradient_point;

struct				s_gradient
{
	bool					is_reverse;
	enum e_gradient_type	type;
	uint32_t				max_iterations;
	uint32_t				points_quantity;
	uint32_t				*colors_cache;
	t_gradient_point		**points_array;
	t_gradient_point		*points_list;
};

struct s_hsv		*rgb2hsv(uint32_t rgba, struct s_hsv *restrict dst);
uint32_t			hsv2rgb(const struct s_hsv *restrict hsv);

t_gradient_point	*grad_create_point_for(struct s_gradient *gradient,
											uint32_t color,
											uint32_t location);

struct s_gradient	*grad_create_from(enum e_gradient_type type,
										uint32_t max_iterations,
										int points_quantity,
										...);

struct s_gradient	*grad_cache_colors(struct s_gradient *gradient);

uint32_t			grad_get_iter_color(struct s_gradient *gradient,
												uint32_t iteration);

#endif
