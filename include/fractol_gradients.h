/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_gradients.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 12:38:28 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/01 18:59:24 by vtarasiu         ###   ########.fr       */
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

typedef struct		s_gradient_point
{
	uint32_t				color;
	uint32_t				iteration;
	double					location;
	struct s_gradient_point	*next;
}					t_gradient_point;

struct				s_gradient
{
	bool					is_reverse;
	enum e_gradient_type	type;
	uint32_t				max_iterations;
	uint32_t				points_quantity;
	uint32_t				*interpolated_colors_cache;
	t_gradient_point		**points_array;
	t_gradient_point		*points_list;
};

t_gradient_point	*grad_create_point(uint32_t color, uint32_t location);

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