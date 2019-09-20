/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/13 13:24:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/09/20 15:18:37 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol_common.h"
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include "fractol_tpool.h"

static struct s_thread_pool	*g_tpool;

void						*tpool_routine(void *arg)
{
	t_task		*task;
	uint32_t	point;

	task = arg;
	while (true)
	{
		pthread_mutex_lock(&g_tpool->job_mutex);
		if (g_tpool->dies ||
			pthread_cond_wait(&g_tpool->job_cond, &g_tpool->job_mutex) != 0)
		{
			pthread_mutex_unlock(&g_tpool->job_mutex);
			break ;
		}
		if (task->fractal == NULL || task->pixels == NULL)
		{
			pthread_mutex_unlock(&g_tpool->job_mutex);
			continue ;
		}
		point = task->region_start;
		pthread_mutex_unlock(&g_tpool->job_mutex);
		while (point < task->region_start + task->region_length)
		{
			if (task->fractal->input.is_avx)
			{
				task->fractal->calculator_avx(task->fractal, task->pixels,
											  point % task->pixels->width,
											  point / task->pixels->width);
				point += 4;
			}
			else
			{
				task->fractal->calculator(task->fractal, task->pixels,
										  point % task->pixels->width,
										  point / task->pixels->width);
				point++;
			}
		}
		task->is_finished = true;
		pthread_cond_signal(&g_tpool->job_end_cond);
	}
	return (NULL);
}

void						tpool_init(int thread_quantity)
{
	int				i;
	pthread_attr_t	attr;

	thread_quantity = thread_quantity <= 0 ? THREAD_POOL_CAPACITY : thread_quantity;
	g_tpool = ft_memalloc(sizeof(struct s_thread_pool));
	g_tpool->threads_number = thread_quantity;
	pthread_mutex_init(&g_tpool->pool_mutex, PTHREAD_MUTEX_DEFAULT);
	pthread_mutex_lock(&g_tpool->pool_mutex);
	pthread_mutex_init(&g_tpool->job_mutex, PTHREAD_MUTEX_DEFAULT);
	pthread_cond_init(&g_tpool->job_cond, NULL);
	g_tpool->threads = ft_memalloc(sizeof(struct s_calc_thread) * thread_quantity);
	i = 0;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	while (i < thread_quantity)
	{
		g_tpool->threads[i].tfractal.is_finished = true;
		pthread_create(&g_tpool->threads[i].thread, &attr, tpool_routine,
			&g_tpool->threads[i].tfractal);
		i++;
	}
	pthread_attr_destroy(&attr);
	pthread_mutex_unlock(&g_tpool->pool_mutex);
}

int							tpool_add_task(t_task *task)
{
	int		i;
	int		status;

	i = -1;
	status = -1;
	pthread_mutex_lock(&g_tpool->pool_mutex);
	while (++i < g_tpool->threads_number)
		if (g_tpool->threads[i].tfractal.is_finished)
		{
//			pthread_mutex_lock(&g_tpool->job_mutex);
			g_tpool->threads[i].tfractal = *task;
//			pthread_mutex_unlock(&g_tpool->job_mutex);
			g_tpool->job_number++;
			pthread_cond_signal_thread_np(&g_tpool->job_cond, g_tpool->threads[i].thread);
			status = 0;
			break ;
		}
	pthread_mutex_unlock(&g_tpool->pool_mutex);
	return (status);
}

int							tpool_wait(void)
{
	int					i;

	i = 0;
	while (i < g_tpool->threads_number)
	{
		pthread_mutex_lock(&g_tpool->pool_mutex);
		if (g_tpool->threads[i].tfractal.is_finished ||
			pthread_cond_wait(&g_tpool->job_end_cond, &g_tpool->pool_mutex) == 0)
			i++;
		pthread_mutex_unlock(&g_tpool->pool_mutex);
	}
	return (i);
}

int							tpool_runnwait(void)
{
	pthread_cond_broadcast(&g_tpool->job_cond);
	return (tpool_wait());
}
