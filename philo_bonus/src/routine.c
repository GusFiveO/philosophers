/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augustinlorain <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:41:18 by augustinlorai     #+#    #+#             */
/*   Updated: 2022/02/24 11:49:29 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	time_overflow(t_philo *philo, size_t time)
{
	sem_wait(philo->check_finish);
	philo->info->finish = 1;
	sem_post(philo->check_finish);
	sem_post(philo->info->stop);
	printf(FORMAT, time - philo->info->start_time, philo->idx, DEAD);
	usleep(10000);
	sem_post(philo->info->print);
}

void	print(const char *str, t_philo *philo)
{
	size_t	time;

	sem_wait(philo->info->print);
	sem_wait(philo->check_finish);
	if (philo->info->finish)
	{	
		sem_post(philo->info->print);
		sem_post(philo->check_finish);
		return ;
	}
	sem_post(philo->check_finish);
	sem_wait(philo->monitoring);
	time = get_time();
	if (time - philo->last_eat + 2 >= philo->info->time_to_die)
		time_overflow(philo, time);
	else
	{
		printf(FORMAT, time - philo->info->start_time, philo->idx, str);
		sem_post(philo->info->print);
	}
	sem_post(philo->monitoring);
}

void	increase_eat(t_philo *philo)
{
	sem_post(philo->info->eat);
	sem_wait(philo->check_finish);
	if (!philo->info->finish)
	{
		sem_post(philo->check_finish);
		sem_wait(philo->monitoring);
		philo->last_eat = get_time();
		sem_post(philo->monitoring);
	}
	sem_post(philo->check_finish);
}

void	eat(const char *str, t_philo *philo)
{
	print(str, philo);
	increase_eat(philo);
	usleep(philo->info->time_to_eat);
	sem_post(philo->info->forks);
	sem_post(philo->info->forks);
}

void	routine(t_philo *philo)
{
	if (!(philo->idx % 2))
		usleep(philo->info->time_to_eat);
	sem_wait(philo->check_finish);
	while (!philo->info->finish)
	{
		sem_post(philo->check_finish);
		sem_wait(philo->info->forks);
		print(FORK, philo);
		sem_wait(philo->info->forks);
		print(FORK, philo);
		eat(EAT, philo);
		print(SLEEP, philo);
		usleep(philo->info->time_to_sleep);
		print(THINK, philo);
		sem_wait(philo->monitoring);
		if (philo->info->time_to_die - (get_time() - philo->last_eat)
			< (philo->info->time_to_eat + philo->info->time_to_sleep) / 2)
			usleep((philo->info->time_to_eat + philo->info->time_to_sleep) / 2);
		sem_post(philo->monitoring);
		sem_wait(philo->check_finish);
	}
	sem_post(philo->check_finish);
}
