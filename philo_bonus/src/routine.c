/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 12:07:47 by alorain           #+#    #+#             */
/*   Updated: 2022/02/18 16:22:08 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo, const char *str)
{
	t_info	*info;

	info = philo->info;
	sem_wait(info->forks);
	sem_wait(info->print);
	if (!philo->eated)
		printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_post(info->print);
	sem_wait(info->forks);
	sem_wait(info->print);
	if (!philo->eated)
		printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_post(info->print);
}

void	eat(t_philo *philo, const char *str)
{
	t_info	*info;

	info = philo->info;
	sem_wait(philo->monitoring);
	philo->last_eat = get_time();
	sem_post(philo->monitoring);
	sem_wait(info->print);
	if (!philo->eated)
		printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_post(info->print);
	sem_post(info->eat);
	usleep(info->time_to_eat);
	sem_post(info->forks);
	sem_post(info->forks);
}

void	ft_sleep(t_philo *philo, const char *str)
{
	t_info	*info;

	info = philo->info;
	sem_wait(info->print);
	if (!philo->eated)
		printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_post(info->print);
	usleep(info->time_to_sleep);
}

void	think(t_philo *philo, const char *str)
{
	t_info	*info;

	info = philo->info;
	sem_wait(info->print);
	if (!philo->eated)
		printf(FORMAT, get_time() - info->start_time, philo->idx, str);
	sem_post(info->print);
}

void	routine(t_philo *philo)
{
	if (philo->idx % 2)
		usleep(philo->info->time_to_eat);
	while (1)
	{
		take_forks(philo, FORK);
		eat(philo, EAT);
		ft_sleep(philo, SLEEP);
		think(philo, THINK);
	}
	exit(0);
}
