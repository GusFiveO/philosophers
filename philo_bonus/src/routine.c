/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 12:07:47 by alorain           #+#    #+#             */
/*   Updated: 2022/02/10 17:08:02 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_wait(philo->fork);
	printf(FORMAT, get_time() - info->start_time, philo->idx, str);
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
	printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_post(philo->p_eat);	
	sem_post(info->eat);
	usleep(info->time_to_eat);
	sem_post(neighbor->fork);
	sem_post(philo->fork);
}

void	ft_sleep(t_philo *philo, const char *str)
{
	t_info	*info;

	info = philo->info;
	printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	usleep(info->time_to_sleep);
}

void	think(t_philo *philo, const char *str)
{
	t_info	*info;

	info = philo->info;
	printf(FORMAT, get_time() - info->start_time, philo->idx, str);
}

void	routine(t_philo *philo)
{
	sem_post(philo->p_eat);	
	while (1)
	{
		take_forks(philo, FORK);
		eat(philo, EAT);
		ft_sleep(philo, SLEEP);
		think(philo, THINK);
	}
}
