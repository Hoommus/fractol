/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 14:36:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/01 16:54:31 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"

/*
** TODO: add flags:
**  -h, --help         - print the following message
**  -q, --quiet        - no GUI mode
**  -i, --input        - input file name (.png). If not specified, only one file
**                       with bare fractal set will be generated.
**                       Otherwise, there will me `-pattern.png' file with applied
**                       input bitmap.
**  -o, --output       - output file name. Always `.png' even if other format
**                       specified
**  -n [string],       - use specified fractal.
**  --name [string]
**  --software-render (?)  - use software renderers in SDL
**  --avx=[true|false] - use AVX and AVX2 instructions, if possible. Emits error
**                       message, if it isn't possible and runs in a classic way.
**                       Defaults to `true'.
**  --opencl=[true|false] - use OpenCL, if possible. Emits error
**                          message, if it isn't possible and runs in a classic way.
**                          Defaults to `false'.
**  --cuda              - use Nvidia CUDA, if available. Emits error message
**                        if it isn't possible
**  --gpu=[device name] - use specified device while calculating fractal.
**                        Silently sets --opencl to `true'
**  --gradient [color1, location1, color2, location2, ...] - specify gradient
**  --cx=[float]       - use such real constant. Defaults to best one for specific
**                       fractal.
**  --cy=[float]       - use such imaginary constant. Defaults to best one for specific
**                       fractal.
**  --scale=[float]    - set image scale for both axes. Defaults to 1.0
**  --width=[int]      - set window or output image width in pixels. For window defaults to 1600
**  --height=[int]     - set window or output image height in pixels. For window defaults to 1400
**  --disable-control(?) - start with GUI ignoring user keyboard and mouse input
**                         at runtime
**
**
*/

int		main(int argc, const char **argv)
{
	argc = 0;
	//fukken_mlx = mlx_init();
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		exit(ft_dprintf(2, "hello from fukken SDL: %s\n", SDL_GetError()));
	signal(SIGINT, &quit);
	TTF_Init();
	dispatch(argv + 1, NULL);
	return (0);
}