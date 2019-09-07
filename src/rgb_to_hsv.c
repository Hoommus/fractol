/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_to_hsv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 16:55:22 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/07 16:16:34 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include "fractol_common.h"
#include "fractol_gradients.h"

static inline float	max(float a, float b, float c)
{
	if (a > b)
		return (a > c ? a : c);
	if (b > a)
		return (b > c ? b : c);
	return (c > b ? c : b);
}

static inline float	min(float a, float b, float c)
{
	if (a < b)
		return (a < c ? a : c);
	if (b < a)
		return (b < c ? b : c);
	return (c < b ? c : b);
}

struct s_hsv		*rgb2hsvl(uint32_t rgba, struct s_hsv *restrict dst)
{

	const double	r = ((double)((rgba & 0xFF000000U) >> 24U)) / 255.0f;
	const double	g = ((double)((rgba & 0x00FF0000U) >> 16U)) / 255.0f;
	const double	b = ((double)((rgba & 0x0000FF00U) >> 8U)) / 255.0f;
	double			max_color;
	double			min_color;

	max_color = max(r, g, b);
	min_color = min(r, g, b);
	if (max_color == min_color)
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
//	printf("%0.8x    - hsv(%f, %f, %f)\n", rgba, dst->h, dst->s, dst->v);
	return (dst);
}

static inline uint32_t	get_rgba(double r, double g, double b)
{
	uint8_t		bytes[4];

	bytes[0] = (uint8_t)round(r * 255.);
	bytes[1] = (uint8_t)round(g * 255.);
	bytes[2] = (uint8_t)round(b * 255.);
	bytes[3] = 0;
//	printf("Returning rgba: %.8x\n", __builtin_bswap32(*((uint32_t *)bytes)));
//	return (__builtin_bswap32(*((uint32_t *)bytes)));
	return (*((uint32_t *)bytes));
}

uint32_t			hsv2rgb(const struct s_hsv *restrict hsv)
{
	const double		c = hsv->v * hsv->s;
	const double		sector = hsv->h / 60.0f;
	const double		x = c * (1 - fabs(fmod(sector, 2.0f) - 1));
	const double		m = hsv->v * (1 - hsv->s);

//	printf("- hsv(%f, %f, %f)\n", hsv->h, hsv->s, hsv->v);
	assert(hsv->h >= 0. && hsv->h <= 360.);
	assert(hsv->v >= 0. && hsv->v <= 1.);
	assert(hsv->s >= 0. && hsv->s <= 1.);
	assert(c + m <= 0xFF && c + m >= 0);
	assert(x + m <= 0xFF && x + m >= 0);
	assert(m <= 0xFF && m >= 0);
	assert(sector >= 0 && sector <= 6);
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
