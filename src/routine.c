/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 15:59:09 by alorain           #+#    #+#             */
/*   Updated: 2022/02/08 16:21:54 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	t_info	*info;
	size_t	i;

	i = philo->idx;
	info = philo->info;
	lock_forks(info, i - 1);
}

void	eat(t_philo *philo, const char *str)
{
	t_info	*info;

	info = philo->info;
	pthread_mutex_lock(&philo->mutex);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->mutex);
	print(philo, str);
	philo->eated++;
	mutex_eat(philo);
	usleep(info->time_to_eat);
}

void	ft_sleep(t_philo *philo, const char *str)
{
	t_info	*info;
	size_t	i;

	info = philo->info;
	i = philo->idx;
	print(philo, str);
	unlock_forks(info, i - 1);
	usleep(info->time_to_sleep);
}

void	think(t_philo *philo, const char *str)
{
	t_info	*info;

	info = philo->info;
	print(philo, str);
	while (get_time() - philo->last_eat < info->time_to_die * 3 / 4)
		usleep(100);
	(void)info;
}

void	routine(t_philo *philo)
{
	take_forks(philo);
	eat(philo, EAT);
	ft_sleep(philo, SLEEP);
	think(philo, THINK);
}
