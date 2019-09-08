/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gradient_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 20:02:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/07 18:41:13 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include "fractol_common.h"
#include "fractol_gradients.h"

struct s_gradient_point		*grad_create_point(uint32_t color, uint32_t location, uint32_t max)
{
	struct s_gradient_point	*point;

	point = ft_memalloc(sizeof(struct s_gradient_point));
	point->rgba = color;
	point->location = (double)location / max;
	rgb2hsv(color, &(point->hsvl));
	printf("%06x - hsv(%f, %f, %f)\n", hsv2rgb(&(point->hsvl)), point->hsvl.h, point->hsvl.s, point->hsvl.v);
	return (point);
}

struct s_gradient_point		*grad_create_point_for(struct s_gradient *gradient,
													  uint32_t color,
													  uint32_t location)
{
	struct s_gradient_point	*point;
	struct s_gradient_point	*list;
	struct s_gradient_point	*direct;

	point = grad_create_point(color, location, gradient->max_iterations);
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
		grad_create_point_for(gradient, va_arg(args, uint32_t), va_arg(args, uint32_t));
		points_quantity--;
	}
	va_end(args);
	return(gradient);
}

struct s_gradient			*grad_cache_colors(struct s_gradient *gradient)
{
	uint32_t	i;
	uint32_t	*cache;

	if (gradient->colors_cache)
		ft_memdel((void **)&(gradient->colors_cache));
	cache = ft_memalloc(sizeof(uint32_t) * gradient->max_iterations + 4);
	i = 0;
	while (i < gradient->max_iterations)
	{
		cache[i] = grad_get_iter_color(gradient, i);
		i++;
	}
	gradient->colors_cache = cache;
	return (gradient);
}

static struct s_hsv			*inter_linear(const struct s_hsv *restrict left,
										const struct s_hsv *restrict right,
										double location,
										struct s_hsv *result)
{
	double		d;

	d = right->h - left->h;
	if (ABS(d) > 180)
	{
		if (d < 0)
			d += 360.0;
		result->h = fmod(left->h + d * location, 360.0);
	}
	else
		result->h = left->h + fabs(d) * location;
	result->h += result->h < 0 ? 360.0 : 0;
	result->s = left->s + (right->s - left->s) * location;
	result->v = left->v + (right->v - left->v) * location;
	return (result);
}

uint32_t					grad_get_iter_color(struct s_gradient *gradient,
	uint32_t iteration)
{
	double					l;
	const t_gradient_point	*list = gradient->points_list;
	struct s_hsv			hsvl_inter;

	if (gradient->colors_cache && iteration <= gradient->max_iterations)
		return (gradient->colors_cache[iteration]);
	l = (double)iteration / (double)gradient->max_iterations;
	while (list && list->next)
	{
		if (list->location == l)
			return (hsv2rgb(rgb2hsv(list->rgba, &hsvl_inter)));
		if (list->next && l > list->location && l <= list->next->location)
			return (hsv2rgb(inter_linear(&(list->hsvl), &(list->next->hsvl), l, &hsvl_inter)));
		list = list->next;
	}
	if (list == NULL)
		return (0);
	else if (list->next == NULL)
		return (list->rgba);
	return (list->next->rgba);
}
