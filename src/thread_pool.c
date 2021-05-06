/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 13:24:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/29 15:28:53 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include <pthread.h>
#include <errno.h>
#include "fractol_tpool.h"
#include <stdatomic.h>

static struct s_thread_pool *g_tpool;

void *tpool_routine(void *arg) {
	t_fract_calc func;
	t_task *task;
	uint32_t point;

	task = arg;
	while (true) {
		pthread_mutex_lock(&g_tpool->job_mutex);
		if (g_tpool->dies ||
			pthread_cond_wait(&g_tpool->job_cond, &g_tpool->job_mutex) != 0)
			return ((void *) (long) (pthread_mutex_unlock(&g_tpool->job_mutex) & 0));
		if (task->fractal == NULL) {
			pthread_mutex_unlock(&g_tpool->job_mutex);
			continue;
		}
		point = task->region_start;
		func = task->fractal->input.is_avx ? task->fractal->calc_avx : task->fractal->calc;
		pthread_mutex_unlock(&g_tpool->job_mutex);
		while (point < task->region_start + task->region_length) {
			func(task->fractal, task->pixels, point % task->pixels->width,
				 point / task->pixels->width);
			point += task->fractal->input.is_avx ? 4 : 1;
		}
		task->fractal = NULL;
		atomic_fetch_and(&g_tpool->must_finish, ~task->thread_bit);
	}
}

void tpool_init(int size) {
	pthread_mutexattr_t mutex_attr;
	pthread_attr_t thread_attr;
	int i;

	g_tpool = calloc(1, sizeof(struct s_thread_pool));
	g_tpool->threads_number = size;
	pthread_mutexattr_init(&mutex_attr);
	pthread_mutex_init(&g_tpool->pool_mutex, &mutex_attr);
	pthread_mutex_init(&g_tpool->job_mutex, &mutex_attr);
	pthread_cond_init(&g_tpool->job_cond, NULL);
	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	g_tpool->threads = calloc(1, sizeof(struct s_calc_thread) * size);
	i = -1;
	while (++i < size) {
		g_tpool->threads[i].tfractal.thread_number = i;
		g_tpool->threads[i].tfractal.thread_bit = 1ULL << i;
		pthread_create(&g_tpool->threads[i].pthread, &thread_attr, tpool_routine,
					   &g_tpool->threads[i].tfractal);
	}
}

#ifdef __APPLE__

int							tpool_add_task(t_task *task, bool start_right_away)
{
	int		i;
	int		status;

	i = -1;
	status = -1;
	pthread_mutex_lock(&g_tpool->pool_mutex);
	while (++i < g_tpool->threads_number)
	{
		if (!(g_tpool->must_finish & (1ULL << i)))
		{
			g_tpool->threads[i].tfractal.fractal = task->fractal;
			g_tpool->threads[i].tfractal.pixels = task->pixels;
			g_tpool->threads[i].tfractal.region_length = task->region_length;
			g_tpool->threads[i].tfractal.region_start = task->region_start;
			atomic_fetch_or(&g_tpool->must_finish, g_tpool->threads[i].tfractal.thread_bit);
			if (start_right_away)
				pthread_cond_signal_thread_np(&g_tpool->job_cond,
					g_tpool->threads[i].pthread);
			status = 0;
			break;
		}
	}
	pthread_mutex_unlock(&g_tpool->pool_mutex);
	return (status);
}

#else

int tpool_add_task(t_task *task, bool start_right_away) {
	int i;
	int status;

	i = -1;
	status = -1;
	pthread_mutex_lock(&g_tpool->pool_mutex);
	while (++i < g_tpool->threads_number) {
		if (!(g_tpool->must_finish & (1ULL << i))) {
			g_tpool->threads[i].tfractal.fractal = task->fractal;
			g_tpool->threads[i].tfractal.pixels = task->pixels;
			g_tpool->threads[i].tfractal.region_length = task->region_length;
			g_tpool->threads[i].tfractal.region_start = task->region_start;
			atomic_fetch_or(&g_tpool->must_finish, g_tpool->threads[i].tfractal.thread_bit);
			if (start_right_away)
				pthread_cond_broadcast(&g_tpool->job_cond);
			status = 0;
			break;
		}
	}
	pthread_mutex_unlock(&g_tpool->pool_mutex);
	return (status);
}

#endif


int tpool_cleanup(void) {
	if (!g_tpool)
		return (1);
	pthread_mutex_lock(&g_tpool->pool_mutex);
	pthread_mutex_lock(&g_tpool->job_mutex);
	g_tpool->dies = true;
	pthread_mutex_unlock(&g_tpool->job_mutex);
	pthread_mutex_unlock(&g_tpool->pool_mutex);
	pthread_cond_broadcast(&g_tpool->job_cond);
	pthread_cond_destroy(&g_tpool->job_cond);
	pthread_mutex_destroy(&g_tpool->pool_mutex);
	pthread_mutex_destroy(&g_tpool->job_mutex);
	free(g_tpool);
	return (0);
}

int tpool_runnwait(void) {
	pthread_cond_broadcast(&g_tpool->job_cond);
	while (g_tpool->must_finish);
	return (0);
}
