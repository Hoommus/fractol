/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_loop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 17:11:15 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/11 17:54:31 by vtarasiu         ###   ########.fr       */
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

int				put_image(void *param)
{
	const struct s_mlx_crutch	*crutch = param;
	int							dummy[3];

	calculate_fractal_avx(crutch->fractal, crutch->pixels,
						  mlx_get_data_addr(crutch->mlx_image, dummy, dummy + 1, dummy + 2));
	mlx_put_image_to_window(crutch->mlx_ptr, crutch->mlx_window, crutch->mlx_image, 0, 0);
	return (0);
}

int				iterations_hook(int scancode, void *param)
{
	const struct s_mlx_crutch	*crutch = param;

	if (scancode == 27 || scancode == 24)
	{
		crutch->fractal->max_iterations += scancode == 27 ? -1 : 1;
		grad_cache_colors(crutch->fractal->gradient_map);
	}
	if (scancode == 15)
		crutch->fractal->gradient_map->is_reverse = !crutch->fractal->gradient_map->is_reverse;
	put_image(param);
	return (1);
}

int				mouse_hook(int x, int y, void *param)
{
	const struct s_mlx_crutch	*crutch = param;

	crutch->fractal->input.mouse_x = x;
	crutch->fractal->input.mouse_y = y;
	crutch->fractal->input.mouse_cx = x;
	crutch->fractal->input.mouse_cy = y;
	put_image(param);
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

int				arrows_hook(int scancode, void *param)
{
	const struct s_mlx_crutch	*crutch = param;

	if (scancode == 123)
		crutch->fractal->input.shift_x -= 24;
	if (scancode == 124)
		crutch->fractal->input.shift_x += 24;
	if (scancode == 126)
		crutch->fractal->input.shift_y -= 24;
	if (scancode == 125)
		crutch->fractal->input.shift_y += 24;
	put_image(param);
	return (0);
}

int				key_hooks(int scancode, void *param)
{
	int							status;

//	ft_printf("%s %d scancode\n", __func__, scancode);
	if (scancode == 53 || scancode == 12)
		quit_mlx(param);
	status = arrows_hook(scancode, param);
	status |= iterations_hook(scancode, param);
	return (status);
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
	put_image((void *)&crutch);
	mlx_do_key_autorepeaton(mlx_ptr);
	mlx_hook(mlx_window, 2, 0, &key_hooks, (void *)&crutch);
	mlx_hook(mlx_window, 6, 0, &mouse_hook, (void *)&crutch);
	mlx_hook(mlx_window, 17, 0, &quit_mlx, (void *)&crutch);
	mlx_loop(mlx_ptr);
	exit(0);
}
