/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 14:36:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/12 20:07:49 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"

/*
** TODO: add flags:
**  -h, --help         - print the following message and exit
**  -q, --quiet        - no GUI mode, just output to a file
**  -i, --input        - input file name (.png). If not specified, only one file
**                       with bare fractal set will be generated.
**                       Otherwise, there will be `-pattern.png' file with applied
**                       input bitmap.
**  -o, --output       - output file name. Always `.png' even if other format
**                       specified
**  -j [number]        - compute fractal on CPU in parallel with a [number] of thread.
**                       Overrides --avx.
**  -j=[number]
**  -n [string],       - use specified fractal.
**  --name [string]
**  --software-render (?)  - use software renderers in SDL
**  --avx              - use AVX and AVX2 instructions, if possible. Emits error
**                       message, if it isn't possible and runs in a classic way.
**                       Defaults to `true'.
**  --opencl           - use OpenCL, if possible. Emits error message,
**                       if it isn't possible and runs in a classic way.
**                       Overrides --avx and -j
**  --cuda              - use Nvidia CUDA, if available. Emits error message
**                        if it isn't possible. Overrides --opencl and -j
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
	if (argc == 1)
		return ((ft_dprintf(2, "usage: fractol [-j] fractal_name\n") & 0) | 1);
	dispatch(argv + 1);
	return (0);
}