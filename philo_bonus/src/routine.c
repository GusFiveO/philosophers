/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 12:07:47 by alorain           #+#    #+#             */
/*   Updated: 2022/02/14 16:32:50 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	smart_sleep(size_t time)
{
	size_t	start_time;

	start_time = get_time();
	while (get_time() - start_time < time / 1000)
		usleep(10);
}

void	take_forks(t_philo *philo, const char *str)
{
	t_info	*info;
	t_philo	*neighbor;

	info = philo->info;
	if (philo->idx == info->nb_philo)
		neighbor = &info->philo[0];
	else
		neighbor = &info->philo[philo->idx];
	sem_wait(neighbor->fork);
	
	sem_wait(info->print);
	printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_post(info->print);

	sem_wait(philo->fork);

	sem_wait(info->print);
	printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_post(info->print);

}

void	eat(t_philo *philo, const char *str)
{
	t_info	*info;
	t_philo	*neighbor;

	info = philo->info;
	if (philo->idx == info->nb_philo)
		neighbor = &info->philo[0];
	else
		neighbor = &info->philo[philo->idx];

	sem_wait(philo->monitoring);
	philo->last_eat = get_time();
	sem_post(philo->monitoring);

	sem_wait(info->print);
	printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_post(info->print);

	sem_post(info->eat);

	smart_sleep(info->time_to_eat);
	sem_post(neighbor->fork);
	sem_post(philo->fork);
}

void	ft_sleep(t_philo *philo, const char *str)
{
	t_info	*info;

	info = philo->info;

	sem_wait(info->print);
	printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_post(info->print);

	smart_sleep(info->time_to_sleep);
}

void	think(t_philo *philo, const char *str)
{
	t_info	*info;

	info = philo->info;

	sem_wait(info->print);
	printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_post(info->print);

}

void	routine(t_philo *philo)
{
	while (1)
	{
		take_forks(philo, FORK);
		eat(philo, EAT);
		ft_sleep(philo, SLEEP);
		think(philo, THINK);
	}
}
