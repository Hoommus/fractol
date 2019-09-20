/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_to_hsv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 16:55:22 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/09 19:15:41 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include "fractol_common.h"
#include "fractol_gradients.h"

static inline double	max(double a, double b, double c)
{
	if (a > b)
		return (a > c ? a : c);
	if (b > a)
		return (b > c ? b : c);
	return (c > b ? c : b);
}

static inline double	min(double a, double b, double c)
{
	if (a < b)
		return (a < c ? a : c);
	if (b < a)
		return (b < c ? b : c);
	return (c < b ? c : b);
}

struct s_hsv		*rgb2hsv(uint32_t argb, struct s_hsv *restrict dst)
{
	const double	r = ((double)((argb & 0x00FF0000U) >> 16U)) / 255.0f;
	const double	g = ((double)((argb & 0x0000FF00U) >> 8U)) / 255.0f;
	const double	b = ((double)((argb & 0x000000FFU))) / 255.0f;
	double			max_color;
	double			min_color;

	max_color = max(r, g, b);
	min_color = min(r, g, b);
	if (max_color == min_color && r == g && r == b)
		dst->h = 0;
	else if (max_color == r)
		dst->h = 60. * ((g - b) / (max_color - min_color));
	else if (max_color == g)
		dst->h = 60. * (2 + (b - r) / (max_color - min_color));
	else if (max_color == b)
		dst->h = 60. * (4 + (r - g) / (max_color - min_color));
	if (dst->h < 0.)
		dst->h += 360.;
	dst->h = roundf(dst->h);
	if (max_color == 0)
		dst->s = 0;
	else
		dst->s = (max_color - min_color) / max_color;
	dst->v = max_color;
	return (dst);
}

static inline uint32_t	get_rgba(double r, double g, double b)
{
	uint8_t		bytes[4];

	bytes[1] = (uint8_t)round(r * 255.);
	bytes[2] = (uint8_t)round(g * 255.);
	bytes[3] = (uint8_t)round(b * 255.);
	bytes[0] = 0;
	return (__builtin_bswap32(*((uint32_t *)bytes)));
}

uint32_t			hsv2rgb(const struct s_hsv *restrict hsv)
{
	const double		c = hsv->v * hsv->s;
	const double		sector = hsv->h / 60.0f;
	const double		x = c * (1 - fabs(fmod(sector, 2.0f) - 1));
	const double		m = hsv->v * (1 - hsv->s);

	assert(hsv->s >= 0. && hsv->s <= 1.);
	assert(hsv->v >= 0. && hsv->v <= 1.);
	assert(hsv->h >= 0. && hsv->h <= 360.);
	if (sector < 0)
		return (get_rgba(m, m, m));
	else if (sector <= 1)
		return (get_rgba(c + m, x + m, m));
	else if (sector <= 2)
		return (get_rgba(x + m, c + m, m));
	else if (sector <= 3)
		return (get_rgba(m, c + m, x + m));
	else if (sector <= 4)
		return (get_rgba(m, x + m, c + m));
	else if (sector <= 5)
		return (get_rgba(x + m, m, c + m));
	else if (sector <= 6)
		return (get_rgba(c + m, m, x + m));
	else
		return (get_rgba(m, m, m));
}
