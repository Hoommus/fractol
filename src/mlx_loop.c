/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_loop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 17:11:15 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/08 21:27:57 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fractals.h>
#include "fractol_common.h"

struct			s_mlx_crutch
{
	struct s_fractal	*fractal;
	struct s_rgba_map	*pixels;
	void				*mlx_ptr;
	void				*mlx_window;
	void				*mlx_image;
};

int				iterations_hook(int scancode, void *param)
{
	const struct s_mlx_crutch	*crutch = param;
	int							dummy[3];

	ft_printf("iterations hook with %d scancode\n", scancode);
	if (scancode == 27)
		crutch->fractal->max_iterations--;
	if (scancode == 24)
		crutch->fractal->max_iterations++;
	calculate_fractal_avx(crutch->fractal, crutch->pixels,
		mlx_get_data_addr(crutch->mlx_image, dummy, dummy + 1, dummy + 2));
	mlx_put_image_to_window(crutch->mlx_ptr, crutch->mlx_window, crutch->mlx_image, 0, 0);
	return (1);
}

int				put_image(void *param)
{
	const struct s_mlx_crutch	*crutch = param;
	int							dummy[3];

	calculate_fractal_avx(crutch->fractal, crutch->pixels,
						  mlx_get_data_addr(crutch->mlx_image, dummy, dummy + 1, dummy + 2));
	mlx_put_image_to_window(crutch->mlx_ptr, crutch->mlx_window, crutch->mlx_image, 0, 0);
	return (0);
}

int				quit_mlx(void *param)
{
	const struct s_mlx_crutch	*crutch = param;

	mlx_destroy_window(crutch->mlx_ptr, crutch->mlx_window);
	free(crutch->pixels->map);
	free(crutch->pixels->map_metadata);
	free(crutch->pixels);
	exit(0);
}

int				quit_mlx_button(int scancode, void *param)
{
	if (scancode == 53 || scancode == 12)
		quit_mlx(param);
	return (1);
}

void			hook_keys(void *mlx_window, struct s_mlx_crutch *crutch)
{
	mlx_key_hook(mlx_window, &quit_mlx, crutch);
}

noreturn void			mlx_game_loop(void *mlx_ptr,
	void *mlx_window,
	struct s_fractal *fractal,
	struct s_rgba_map *pixels)
{
	struct s_mlx_crutch	crutch;

	crutch = (struct s_mlx_crutch){fractal, pixels, mlx_ptr, mlx_window, 0};
	crutch.mlx_image = mlx_new_image(mlx_ptr, pixels->width, pixels->height);
	mlx_key_hook(mlx_window, &iterations_hook, (void *)&crutch);
	mlx_hook(mlx_window, 2, 0, &iterations_hook, (void *)&crutch);
	mlx_hook(mlx_window, 2, 0, &quit_mlx_button, (void *)&crutch);
	mlx_hook(mlx_window, 17, 0, &quit_mlx, (void *)&crutch);
	mlx_loop(mlx_ptr);
	exit(0);
}
