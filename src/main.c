/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 14:36:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/24 18:38:32 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include "fractol_common.h"

int		main(int argc, const char **argv)
{
	argc = 0;
	//fukken_mlx = mlx_init();
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		exit(ft_dprintf(2, "hello from fukken SDL: %s\n", SDL_GetError()));
	signal(SIGINT, &quit);
	dispatch(argv + 1, NULL);
	return (0);
}