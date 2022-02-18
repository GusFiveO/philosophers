/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 14:06:41 by alorain           #+#    #+#             */
/*   Updated: 2022/02/18 16:26:20 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*check_death(void *arg)
{
	t_philo	*philo;
	t_info	*info;

	philo = (t_philo *)arg;
	info = philo->info;
	while (1)
	{
		sem_wait(philo->monitoring);
		if (get_time() - philo->last_eat > info->time_to_die)
		{
			sem_wait(info->print);
			printf(FORMAT, get_time() - info->start_time, philo->idx, DEAD);
			sem_post(info->stop);
			break ;
		}
		sem_post(philo->monitoring);
		usleep(10);
	}
	return (arg);
}

void	*eat_check(void *arg)
{
	t_info	*info;
	size_t	eated;

	info = (t_info *)arg;
	eated = 0;
	while (eated != info->nb_philo * info->nb_t_philo_m_eat)
	{
		sem_wait(info->check_finish);
		if (info->finish)
			return (NULL);
		sem_post(info->check_finish);
		sem_wait(info->eat);
		eated++;
	}
	sem_wait(info->print);
	sem_post(info->stop);
	return (NULL);
}

int	launch_eat_thread(t_info *info)
{
	int	ret;

	ret = 0;
	ret = pthread_create(&info->eat_check, NULL, eat_check, info);
	if (ret)
		return (0);
	return (1);
}

int	launch_philo(t_philo *philo)
{
	int	ret;

	ret = 0;
	philo->last_eat = get_time();
	ret = pthread_create(&philo->id, NULL, check_death, philo);
	if (ret)
		return (0);
	ret = pthread_detach(philo->id);
	if (ret)
		return (0);
	routine(philo);
	return (1);
}
