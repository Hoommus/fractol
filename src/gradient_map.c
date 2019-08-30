/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gradient_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 20:02:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/28 20:36:55 by vtarasiu         ###   ########.fr       */
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

struct s_gradient_point		*create_point_for(struct s_gradient *gradient, uint32_t color, uint32_t location)
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

struct s_gradient			*create_gradient(struct s_gradient_point *list)
{
	return((struct s_gradient *)(list = NULL));
}
