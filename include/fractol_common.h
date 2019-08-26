/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_common.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 22:09:50 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/26 19:04:31 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_COMMON_H
# define FRACTOL_COMMON_H

# include <mlx.h>
# include <stddef.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>
# include "libft.h"
# include "ft_printf.h"
# include "fractol_data.h"

# include <SDL.h>

#define UI_FEEDBACK_REDRAW  0x0001U
#define UI_FEEDBACK_AVX     0x0010U
#define UI_FEEDBACK_NEUTRAL 0x0100U

int					dispatch(const char **argv, void *display);
void				game_loop(SDL_Window *window, struct s_fractal *fractal, struct s_rgba_map *pixels);
uint32_t			poll_events(SDL_Window *window, struct s_fractal *fractal, struct s_rgba_map *pixels);


#endif
