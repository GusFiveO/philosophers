/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 12:17:36 by alorain           #+#    #+#             */
/*   Updated: 2022/02/08 16:14:23 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	mutex_stop(t_info *info)
{
	pthread_mutex_lock(&info->m_stop);
	if (info->nb_time_all_philo_eat == info->nb_philo)
		info->stop = 1;
	pthread_mutex_unlock(&info->m_stop);
}

void	mutex_eat(t_philo *philo)
{
	t_info	*info;

	info = philo->info;
	pthread_mutex_lock(&info->eat);
	if ((int)philo->eated == info->nb_t_philo_m_eat)
		info->nb_time_all_philo_eat++;
	mutex_stop(info);
	pthread_mutex_unlock(&info->eat);
}

void	lock_forks(t_info *info, size_t idx)
{
	t_philo	*philo;

	philo = &info->philo[idx];
	if (idx % 2 && !(info->nb_philo % 2))
	{
		pthread_mutex_lock(&info->forks[philo->left_fork]);
		print(philo, FORK);
		pthread_mutex_lock(&info->forks[philo->right_fork]);
		print(philo, FORK);
	}
	else if (!(idx % 2) && info->nb_philo % 2)
	{
		pthread_mutex_lock(&info->forks[philo->left_fork]);
		print(philo, FORK);
		pthread_mutex_lock(&info->forks[philo->right_fork]);
		print(philo, FORK);
	}
	else
	{
		pthread_mutex_lock(&info->forks[philo->right_fork]);
		print(philo, FORK);
		pthread_mutex_lock(&info->forks[philo->left_fork]);
		print(philo, FORK);
	}
}

void	unlock_forks(t_info *info, size_t idx)
{
	t_philo	*philo;

	philo = &info->philo[idx];
	if (idx % 2 && !(info->nb_philo % 2))
	{
		pthread_mutex_unlock(&info->forks[philo->left_fork]);
		pthread_mutex_unlock(&info->forks[philo->right_fork]);
	}
	else if (!(idx % 2) && info->nb_philo % 2)
	{
		pthread_mutex_unlock(&info->forks[philo->left_fork]);
		pthread_mutex_unlock(&info->forks[philo->right_fork]);
	}
	else
	{
		pthread_mutex_unlock(&info->forks[philo->right_fork]);
		pthread_mutex_unlock(&info->forks[philo->left_fork]);
	}
}

void	print(t_philo *philo, const char *str)
{
	size_t	i;
	t_info	*info;

	i = philo->idx;
	info = philo->info;
	pthread_mutex_lock(&info->print);
	pthread_mutex_lock(&info->m_stop);
	if (!info->stop)
	{
		if (info->start_time == 0)
			info->start_time = get_time();
		printf(" %-4ld ms philo [%ld] %s\n", get_time() - info->start_time, i, str);
	}
	pthread_mutex_unlock(&info->m_stop);
	pthread_mutex_unlock(&info->print);
}

int	init_mutex(t_info *info)
{
	size_t	i;
	int		ret;

	i = 0;
	ret = pthread_mutex_init(&info->m_stop, NULL);
	if (ret)
		return (ret);
	ret = pthread_mutex_init(&info->eat, NULL);
	if (ret)
		return (ret);
	ret = pthread_mutex_init(&info->print, NULL);
	if (ret)
		return (ret);
	while (i < info->nb_philo)
	{
		ret = pthread_mutex_init(&info->forks[i], NULL);
		if (ret)
			return (ret);
		ret = pthread_mutex_init(&info->philo[i].mutex, NULL);
		if (ret)
			return (ret);
		i++;
	}
	return (0);
}
