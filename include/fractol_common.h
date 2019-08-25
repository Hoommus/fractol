/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_common.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 22:09:50 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/21 22:27:31 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_COMMON_H
# define FRACTOL_COMMON_H

# include <mlx.h>
# include <stddef.h>
# include <unistd.h>
# include "libft.h"
# include "ft_printf.h"
# include <SDL.h>

int					dispatch(const char **argv, void *display);

#endif
