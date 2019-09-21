/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 14:36:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/20 18:42:30 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_tpool.h"
#include "fractol_common.h"
#include <getopt.h>

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
**  -t [number]        - compute fractal on CPU in parallel with a [number] of threads.
**                       Overrides --avx.
**  -t=[number]
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

__unused const static struct option	g_opts[] = {
	{"help", no_argument, NULL, 'h'},
	{"quiet", no_argument, NULL, 'q'},
	{"input", required_argument, NULL, 'i'},
	{"output", required_argument, NULL, 'o'},
	{"threads", required_argument, NULL, 't'},
	{"avx", no_argument, NULL, OPTION_AVX},
	{"opencl", no_argument, NULL, OPTION_OPENCL},
	{"cuda", no_argument, NULL, OPTION_CUDA},
	{"cx", required_argument, NULL, 257},
	{"cy", required_argument, NULL, 258},
	{"scale", required_argument, NULL, 259},
	{"width", required_argument, NULL, 260},
	{"height", required_argument, NULL, 261},
	{"disable-control", no_argument, NULL, OPTION_NO_USER_INPUT},
	{"sdl", no_argument, NULL, OPTION_SDL},
	{"mlx", no_argument, NULL, OPTION_MLX},
	{NULL, no_argument, NULL, 0},
};

int						parse_flags(int argc, const char **argv, struct s_options *opts)
{
	int		i;
	__unused int		j;

	i = 0;
	while (i < argc && argv[i][0] == '-')
	{
		if (ft_strcmp(argv[i], "-j") == 0)
		{
			opts->opts |= OPTION_THREADED;
			opts->threads = ft_atoi(argv[++i]);
			opts->threads = opts->threads > 64 ? 64 : opts->threads;
		}
		i++;
	}
	return (i);
}

int							main(int argc, const char **argv)
{
	struct s_options	options;

	ft_bzero(&options, sizeof(struct s_options));
	if (argc == 1)
		return ((ft_dprintf(2, "usage: fractol [-t] fractal_name\n") & 0) | 1);
	argv += parse_flags(argc - 1, argv + 1, &options) + 1;
	if (options.opts & OPTION_THREADED)
		tpool_init(options.threads); // TODO: replace with -j option argument
	dispatch(argv, &options);
	return (0);
}