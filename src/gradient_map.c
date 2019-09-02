/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gradient_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 20:02:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/02 15:47:36 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include "fractol_gradients.h"

struct s_gradient_point		*grad_create_point(uint32_t color, uint32_t location)
{
	struct s_gradient_point	*point;

	point = ft_memalloc(sizeof(struct s_gradient_point));
	point->color = color;
	point->location = location;
	return (point);
}

struct s_gradient_point		*grad_create_point_for(struct s_gradient *gradient,
													  uint32_t color,
													  uint32_t location)
{
	struct s_gradient_point	*point;
	struct s_gradient_point	*list;
	struct s_gradient_point	*direct;

	point = grad_create_point(color, location);
	list = gradient->points_list;
	while (list)
	{
		if (list->location < location && list->next && list->next->location >= location)
		{
			direct = list->next;
			list->next = point;
			point->next = direct;
			return (point);
		}
		else if (list->location < location && !list->next)
		{
			list->next = point;
			return (point);
		}
		list = list->next;
	}
	return (gradient->points_list = point);
}

/*
** Accepts 'tuples' of (uint32_t color, uint32t location) as arguments for
** vararg.
** The points_quantity specifies number of such tuples provided.
*/

struct s_gradient			*grad_create_from(enum e_gradient_type type,
												uint32_t max_iterations,
													int points_quantity,
																	...)
{
	va_list				args;
	struct s_gradient	*gradient;

	va_start(args, points_quantity);
	gradient = ft_memalloc(sizeof(struct s_gradient));
	gradient->type = type;
	gradient->max_iterations = max_iterations;
	gradient->points_quantity = points_quantity;
	while (points_quantity)
	{
		grad_create_point_for(gradient, va_arg(args, uint32_t), va_arg(args, uint32_t) / (double)max_iterations);
		points_quantity--;
	}
	va_end(args);
	return(gradient);
}

struct s_gradient			*grad_cache_colors(struct s_gradient *gradient)
{
	uint32_t	i;
	uint32_t	*cache;

	if (gradient->interpolated_colors_cache)
		ft_memdel((void **)&(gradient->interpolated_colors_cache));
	cache = ft_memalloc(sizeof(uint32_t) * gradient->max_iterations + 4);
	i = 0;
	while (i < gradient->max_iterations)
	{
		cache[i] = grad_get_iter_color(gradient, i);
		printf("%d: %x\n", i, cache[i]);
		i++;
	}
	gradient->interpolated_colors_cache = cache;
	return (gradient);
}

// void interpolate()
// void interpolate_next()
// void get_color_for_position(struct s_gradient *gradient, double position)
// int get_position_for_iteration(struct s_gradient *gradient, int max_iter)

#define COLOR_GET_RED(x) (((x) & COLOR_RED_MASK) >> 24U)
#define COLOR_GET_GREEN(x) (((x) & COLOR_GREEN_MASK) >> 16U)
#define COLOR_GET_BLUE(x) (((x) & COLOR_BLUE_MASK) >> 8U)
#define COLOR_GET_ALPHA(x) (((x) & COLOR_ALPHA_MASK))

union			u_color
{
	uint32_t	color_int;
	uint8_t		color_rgba[4];
};

uint32_t					grad_get_iter_color(struct s_gradient *gradient,
	uint32_t iteration)
{
	union u_color			color;
	double					location;
	const t_gradient_point	*list = gradient->points_list;

	if (gradient->interpolated_colors_cache && iteration <= gradient->max_iterations)
		return (gradient->interpolated_colors_cache[iteration]);
	location = (double)iteration / (double)gradient->max_iterations; // location percentage
	while (list && list->next)
	{
		if (list->next && location > list->location && location < list->next->location)
		{
			color.color_rgba[0] = (uint8_t)((COLOR_GET_RED(list->next->color) - (COLOR_GET_RED(list->color)))
				* location + COLOR_GET_RED(list->color));
			color.color_rgba[1] = (uint8_t)((COLOR_GET_GREEN(list->next->color) - (COLOR_GET_GREEN(list->color)))
				* location + COLOR_GET_GREEN(list->color));
			color.color_rgba[2] = (uint8_t)((COLOR_GET_BLUE(list->next->color) - (COLOR_GET_BLUE(list->color)))
				* location + COLOR_GET_BLUE(list->color));
			color.color_rgba[3] = 0;
			return (color.color_int);
		}
		list = list->next;
	}
	return (list == NULL || list->next == NULL ? 0 : list->next->color);
}
