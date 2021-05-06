/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_tpool.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 17:46:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/28 21:25:46 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FRACTOL_TPOOL_H
# define FRACTOL_TPOOL_H

# include "fractol_data.h"
# include "fractol_common.h"
# include <stdatomic.h>

# define TPOOL_MAX_THREADS 64

# ifndef THREAD_POOL_CAPACITY
#  define THREAD_POOL_CAPACITY 4
# endif

typedef struct s_task {
	struct s_fractal *fractal;
	struct s_rgba_map *pixels;
	uint32_t region_start;
	uint32_t region_length;
	uint64_t thread_number;
	uint64_t thread_bit;
	uint64_t thread_bit_inverse;
} t_task;

struct s_calc_thread {
	pthread_t pthread;
	t_task tfractal;
};

struct s_thread_pool {
	pthread_mutex_t pool_mutex;
	pthread_mutex_t job_mutex;
	pthread_cond_t job_cond;
	struct s_calc_thread *threads;
	int32_t threads_number;
	atomic_ullong must_finish;
	atomic_bool dies;
};

void tpool_init(int size);

int tpool_add_task(t_task *task, bool start_right_away);

int tpool_cleanup(void);

int tpool_runnwait(void);

#endif
