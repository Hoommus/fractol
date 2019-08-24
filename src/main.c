/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 14:36:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/24 17:58:26 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"


void	quit(int sig)
{
	exit(sig = 0);
}

int		main(int argc, const char **argv)
{
	void	*fukken_mlx;


	fukken_mlx = mlx_init();
	signal(SIGINT, &quit);
	dispatch(argv + 1, fukken_mlx);
	return 0;
}