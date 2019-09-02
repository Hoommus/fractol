/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_common.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 22:09:50 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/30 19:02:59 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_COMMON_H
# define FRACTOL_COMMON_H

# include <mlx.h>
# include <stddef.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdnoreturn.h>
# include <stdarg.h>
# include "libft.h"
# include "ft_printf.h"
# include "fractol_data.h"

# include <SDL.h>
# include <SDL_ttf.h>

#define UI_FEEDBACK_REDRAW       0x0001U
#define UI_FEEDBACK_AVX          0x0002U
#define UI_FEEDBACK_NEUTRAL      0x0100U
#define UI_FEEDBACK_MOUSE_DOWN   0x0010U
#define UI_FEEDBACK_MOUSE_UP     0x0010U

int					dispatch(const char **argv, void *display);
void				game_loop(SDL_Window *window, struct s_fractal *fractal, struct s_rgba_map *pixels);
uint32_t			poll_events(SDL_Window *window, struct s_fractal *fractal, struct s_rgba_map *pixels);

noreturn void		quit(int status);

#endif
