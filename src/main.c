/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/21 14:36:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/28 19:03:07 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_tpool.h"
#include "fractol_common.h"
#include <getopt.h>
#include <errno.h>

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
**  --float            - use single precision floats instead of double
**
**
*/

#define FRACTOL_USAGE_STR "usage: fractol [-h] [<options>...] fractal_name\n"

const static struct option g_opts[] = {
		{"help",            no_argument,       NULL, 'h'},
		{"quiet",           no_argument,       NULL, 'q'},
		{"input",           required_argument, NULL, 'i'},
		{"output",          required_argument, NULL, 'o'},
		{"threads",         required_argument, NULL, 't'},
		{"avx",             no_argument,       NULL, OPTION_AVX},
		{"opencl",          no_argument,       NULL, OPTION_OPENCL},
		{"cuda",            no_argument,       NULL, OPTION_CUDA},
		{"cx",              required_argument, NULL, 257},
		{"cy",              required_argument, NULL, 258},
		{"scale",           required_argument, NULL, 259},
		{"width",           required_argument, NULL, 260},
		{"height",          required_argument, NULL, 261},
		{"disable-control", no_argument,       NULL, OPTION_NO_USER_INPUT},
		{0},
};

static int help(void) {
	dprintf(2, FRACTOL_USAGE_STR
			  "    -h, --help  - print the following message and exit\n"
			  "    -t [number] - compute fractal on CPU in parallel with a [number] of threads.\n"
			  "    -t=[number]\n"
			  "    --classic   - use 'classic' fractal calculation function\n"
			  "    --avx       - use AVX and AVX2 instructions, if possible. Emits error\n"
			  "                  message, if it isn't possible and runs in a classic way.\n"
			  "                  Defaults to `true'.\n"
			  "    --verbose   - print to controlling terminal additional info: render speeds,\n"
			  "                  warnings etc. Might have a minor performance impact\n");
	return (0);
}

static void cleanup(void) {
	SDL_Quit();
	TTF_Quit();
	tpool_cleanup();
	(void) g_opts;
}

int parse_flags(int argc, char **argv, struct s_options *opts) {
	int longindex;
	int arg = 0;

	while (true) {
		arg = getopt_long(argc, argv, "t:hV", g_opts, &longindex);
		switch (arg) {
		case 't':
			opts->opts |= OPTION_THREADED;
			if (optarg[0] == '=')
				optarg++;
			opts->threads = strtol(optarg, NULL, 0) < 64 ? : 64 ;
			break;
		case 'h':
			return help();
		case -1:
			/* end of options */
			return optind;
		case '?':
			fprintf(stderr, "%s: %s\n", strerror(EINVAL), optarg);
			help();
			return -EINVAL;
		default:
			fprintf(stderr, "What the hell?\n");
			break;
		}
	}
	return optind;
}

int main(int argc, char **argv) {
	struct s_options options = {0};
	int ret;

	if (argc == 1)
		return help();
	ret = parse_flags(argc, argv, &options);
	if (ret < 0)
		return -ret;
	argv += ret;
	if (options.opts & OPTION_THREADED)
		tpool_init(options.threads);
	atexit(&cleanup);
	dispatch((const char **)argv, &options);
	return (0);
}
