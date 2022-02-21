/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routineBis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augustinlorain <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:41:18 by augustinlorai     #+#    #+#             */
/*   Updated: 2022/02/21 15:16:46 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	print(const char *str, t_philo *philo)
{
	sem_wait(philo->check_finish);
	if (philo->info->finish)
	{	
		sem_post(philo->check_finish);
		return ;
	}
	sem_post(philo->check_finish);
	printf(FORMAT, get_time() - philo->info->start_time, philo->idx, str);
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

void	take_forks(const char *str, t_philo *philo)
{
	sem_wait(philo->info->forks);
	print(str, philo);
	sem_wait(philo->info->forks);
	print(str, philo);
}

void	eat(const char *str, t_philo *philo)
{
	print(str, philo);
	increase_eat(philo);	
	usleep(philo->info->time_to_eat);
	sem_post(philo->info->forks);
	sem_post(philo->info->forks);
}

void	ft_sleep(const char *str, t_philo *philo)
{
	print(str, philo);
	usleep(philo->info->time_to_sleep);
}

void	routine(t_philo *philo)
{
	if (!(philo->idx % 2))
		usleep(philo->info->time_to_eat);
	sem_wait(philo->check_finish);
	while (!philo->info->finish)
	{
		sem_post(philo->check_finish);
		take_forks(FORK, philo);
		eat(EAT, philo);
		ft_sleep(SLEEP, philo);
		print(THINK, philo);
	}
	sem_post(philo->check_finish);
}
