/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_tpool.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 17:46:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/18 17:48:33 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FRACTOL_TPOOL_H
# define FRACTOL_TPOOL_H

#include "fractol_data.h"
#include "fractol_common.h"


#ifndef THREAD_POOL_CAPACITY
# define THREAD_POOL_CAPACITY 4
#endif

typedef struct		s_task
{
	struct s_fractal	*fractal;
	struct s_rgba_map	*pixels;
	uint32_t			region_start;
	uint32_t			region_length;
	bool				is_finished;
}					t_task;

struct				s_calc_thread
{
	pthread_t		thread;
	t_task			tfractal;
};

struct				s_thread_pool
{
	pthread_mutex_t			pool_mutex;
	pthread_mutex_t			job_mutex;
	pthread_cond_t			job_cond;
	pthread_cond_t			job_end_cond;
	struct s_calc_thread	*threads;
	int						threads_number;
	int						job_number;
	bool					dies;
};

void						tpool_init(int thread_quantity);
int							tpool_add_task(t_task *task);
int							tpool_wait(void);
int							tpool_runnwait(void);

#endif
