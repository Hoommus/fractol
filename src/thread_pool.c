/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 13:24:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/23 19:20:20 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include <pthread.h>
#include <errno.h>
#include "fractol_tpool.h"
#include <stdatomic.h>

static struct s_thread_pool	*g_tpool;

void						*tpool_routine(void *arg)
{
	t_fract_calc	func;
	t_task			*task;
	uint32_t		point;

	task = arg;
	while (true)
	{
		if (g_tpool->dies ||
			pthread_cond_wait(&g_tpool->job_cond, &g_tpool->job_mutex) != 0)
		{
			pthread_mutex_unlock(&g_tpool->job_mutex);
			break ;
		}
		if (task->is_finished)
			continue ;
		point = task->region_start;
		func = task->fractal->input.is_avx ? task->fractal->calculator_avx : task->fractal->calculator;
		pthread_mutex_unlock(&g_tpool->job_mutex);
		while (point < task->region_start + task->region_length)
		{
			func(task->fractal, task->pixels,
				 point % task->pixels->width,
				 point / task->pixels->width);
			point += task->fractal->input.is_avx ? 4 : 1;
		}
		pthread_cond_signal(&g_tpool->job_end_cond);
		task->is_finished = true;
	}
	return (NULL);
}

void						tpool_init(int size)
{
	int					i;
	struct sched_param	param;
	pthread_attr_t		attr;
	pthread_t			this;

	ft_bzero(&param, sizeof(struct sched_param));
	this = pthread_self();
	param.sched_priority = sched_get_priority_min(SCHED_RR);
	pthread_setschedparam(this, SCHED_RR, &param);
	g_tpool = ft_memalloc(sizeof(struct s_thread_pool));
	g_tpool->threads_number = size;
	pthread_mutex_init(&g_tpool->pool_mutex, PTHREAD_MUTEX_DEFAULT);
	pthread_mutex_init(&g_tpool->job_mutex, PTHREAD_MUTEX_DEFAULT);
	pthread_cond_init(&g_tpool->job_cond, NULL);
	pthread_cond_init(&g_tpool->job_cond, NULL);
	pthread_attr_init(&attr);
	param.sched_priority = sched_get_priority_max(SCHED_RR);
	pthread_attr_setschedparam(&attr, &param);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	g_tpool->threads = ft_memalloc(sizeof(struct s_calc_thread) * size);
	i = -1;
	while (++i < size)
	{
		g_tpool->threads[i].tfractal.is_finished = true;
		pthread_create(&g_tpool->threads[i].pthread, &attr, tpool_routine,
						&g_tpool->threads[i].tfractal);
	}
	pthread_attr_destroy(&attr);
}

int							tpool_add_task(t_task *_Nonnull task)
{
	int		i;
	int		status;

	i = -1;
	status = -1;
	pthread_mutex_lock(&g_tpool->pool_mutex);
	while (++i < g_tpool->threads_number)
		if (g_tpool->threads[i].tfractal.is_finished)
		{
			g_tpool->threads[i].tfractal = *task;
			g_tpool->threads[i].tfractal.thread_number = i;
			g_tpool->threads[i].tfractal.is_finished = false;
#ifdef __APPLE__
			pthread_cond_signal_thread_np(&g_tpool->job_cond, g_tpool->threads[i].pthread);
#else
			pthread_cond_broadcast(&g_tpool->job_cond);
#endif
			status = 0;
			break ;
		}
	pthread_mutex_unlock(&g_tpool->pool_mutex);
	return (status);
}

int							tpool_wait(void)
{
	int					i;

	pthread_mutex_lock(&g_tpool->pool_mutex);
	i = 0;
	while (i < g_tpool->threads_number)
	{
		if (g_tpool->threads[i].tfractal.is_finished)
			i++;
		else if (pthread_cond_wait(&g_tpool->job_end_cond, &g_tpool->pool_mutex) == 0)
		{
			pthread_mutex_unlock(&g_tpool->pool_mutex);
			i++;
		}
	}
	pthread_mutex_unlock(&g_tpool->pool_mutex);
	return (i);
}

int							tpool_cleanup(void)
{
	if (!g_tpool)
		return (1);
	pthread_mutex_lock(&g_tpool->pool_mutex);
	pthread_mutex_lock(&g_tpool->job_mutex);
	g_tpool->dies = true;
	pthread_mutex_unlock(&g_tpool->job_mutex);
	pthread_mutex_unlock(&g_tpool->pool_mutex);
	pthread_cond_broadcast(&g_tpool->job_cond);
	pthread_cond_destroy(&g_tpool->job_cond);
	pthread_cond_destroy(&g_tpool->job_end_cond);
	pthread_mutex_destroy(&g_tpool->pool_mutex);
	pthread_mutex_destroy(&g_tpool->job_mutex);
	free(g_tpool);
	return (0);
}

int							tpool_runnwait(void)
{
	pthread_cond_broadcast(&g_tpool->job_cond);
	return (tpool_wait());
}
