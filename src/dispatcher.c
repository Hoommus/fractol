/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 21:52:16 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/08/24 18:00:21 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include "fractals.h"

static const struct s_command	g_dispatchable[] =
{
	{
		.name = "mandelbrot",
		.temp_late = {
			1,
			10,
			1.0,
			1.0,

			0.33,
			0.5,

			&mandel_pixel,
			NULL
		}
	},
	{
		.name = "julia"
	},
	{
		.name = NULL
	}
};

static int						g_flags;

void							calculate_fractal(struct s_fractal *fractal, struct s_rgba_map *pixels)
{
	int32_t		i;
	int32_t		j;

	i = -1;
	while (++i < pixels->width)
	{
		ft_dprintf(2, "calculating x = %d\n", i);
		j = -1;
		while (++j < pixels->height)
		{
			fractal->calculator(fractal, pixels, i, j);
		}
	}
}

int								forknrun(const struct s_command *cmd, const char __unused **argv, void *display)
{
	struct s_fractal	fractal;
	struct s_rgba_map	*pixels;
	void				*window;
	void				*image;

	int32_t				color_depth;
	int32_t				size_line;
	int32_t				endian;

	uint8_t			*data;

	pixels = ft_memalloc(sizeof(struct s_rgba_map));
	pixels->height = 1000;
	pixels->width = 1000;
	pixels->map = ft_memalloc(sizeof(uint32_t) * pixels->width * pixels->width);
	fractal = cmd->temp_late;
	window = mlx_new_window(display, pixels->width, pixels->height, (char *)cmd->name);
	mlx_clear_window(display, window);
	image = mlx_new_image(display, pixels->width, pixels->height);
	calculate_fractal(&fractal, pixels);
	data = (uint8_t *)mlx_get_data_addr(image, &color_depth, &size_line, &endian);
	int		i = 0;
	while (i < pixels->height * pixels->width - 1)
	{
		data[i] = 0xFF;
		//ft_memset(data += size_line, 0xFFFFFF, size_line);
		//ft_memcpy(data += size_line, pixels->map + i, size_line);
		i++;
	}

	mlx_put_image_to_window(display, window, image, 0, 0);
	mlx_loop(display);
	return (0);
}



int								dispatch(const char **argv, void *display)
{
	int		i;
	int		status;

	status = -1;
	i = -1;
	while (g_dispatchable[++i].name)
	{
		if (ft_strcmp(argv[0], g_dispatchable[i].name) == 0)
			status = forknrun(g_dispatchable + i, argv + 1, display);
	}
	return (status);
}
