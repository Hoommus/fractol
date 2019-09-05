/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_to_hsv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/02 16:55:22 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/05 12:59:55 by vtarasiu         ###   ########.fr       */
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

struct s_hsvl		*rgb2hsvl(uint32_t rgba, struct s_hsvl *restrict dst)
{
	const float		r = ((float)((rgba & 0xFF000000U) >> 24U)) / 256.0f;
	const float		g = ((float)((rgba & 0x00FF0000U) >> 16U)) / 256.0f;
	const float		b = ((float)((rgba & 0x0000FF00U) >> 8U)) / 256.0f;
	float			max_color;
	float			min_color;

	max_color = max(r, g, b);
	min_color = min(r, g, b);
	dst->h = 0;
	if (max_color == r)
		dst->h = 60. * ((g - b) / (max_color - min_color));
	else if (max_color == g)
		dst->h = 60. * (2 + (b - r) / (max_color - min_color));
	else if (max_color == b)
		dst->h = 60. * (4 + (r - g) / (max_color - min_color));
	if (dst->h < 0.)
		dst->h += 360.;
	if (max_color == 0)
		dst->s = 0;
	else
		dst->s = (max_color - min_color) / max_color;
	dst->v = max_color;
	dst->l = (max_color + min_color) / 2.;
	printf("%0.8x    - hsv(%f, %f, %f)\n", rgba, dst->h, dst->s, dst->v);
	return (dst);
}

static inline uint32_t	get_rgba(float r, float g, float b)
{
	uint8_t		bytes[4];

	bytes[0] = (uint8_t)roundf(r * 255);
	bytes[1] = (uint8_t)roundf(g * 255);
	bytes[2] = (uint8_t)roundf(b * 255);
	bytes[3] = 0;
	return (*((uint32_t *)bytes) << 8U);
}

uint32_t			hsvl2rgb(const struct s_hsvl *restrict hsvl)
{
	const float		c = hsvl->v * hsvl->s;
	const float		sector = hsvl->h / 60.0f;
	const float		x = c * (1 - fabsf(fmodf(sector, 2.0f) - 1));
	const float		m = hsvl->v * (1 - hsvl->s);

	printf("- hsv(%f, %f, %f)\n", hsvl->h, hsvl->s, hsvl->v);
	assert(hsvl->h >= 0. && hsvl->h <= 360.);
	assert(hsvl->v >= 0. && hsvl->v <= 1.);
	assert(hsvl->s >= 0. && hsvl->s <= 1.);
	assert(c + m <= 0xFF && c + m >= 0);
	assert(x + m <= 0xFF && x + m >= 0);
	assert(m <= 0xFF && m >= 0);
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
