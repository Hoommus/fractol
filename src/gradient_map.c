/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gradient_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 20:02:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/30 18:32:44 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"

struct s_gradient_point		*create_point(uint32_t color, uint32_t location)
{
	struct s_gradient_point	*point;

	point = ft_memalloc(sizeof(struct s_gradient_point));
	point->color = color;
	point->location = location;
	return (point);
}

struct s_gradient_point		*create_point_for(struct s_gradient *gradient,
														uint32_t color,
														uint32_t location)
{
	struct s_gradient_point	*point;
	struct s_gradient_point	*list;
	struct s_gradient_point	*direct;

	point = create_point(color, location);
	list = gradient->points;
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
	return (gradient->points = point);
}

struct s_gradient			*create_gradient_from(enum e_gradient_type type, int points_quantity, ...)
{
	va_list				args;
	struct s_gradient	*gradient;

	va_start(args, points_quantity);
	gradient = ft_memalloc(sizeof(struct s_gradient));
	gradient->type = type;
	gradient->scale = 1.0;
	gradient->angle = 0.0;
	while (points_quantity)
	{
		create_point_for(gradient, va_arg(args, uint32_t), va_arg(args, uint32_t));
		points_quantity--;
	}
	va_end(args);
	return(gradient);
}
