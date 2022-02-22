/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 14:06:41 by alorain           #+#    #+#             */
/*   Updated: 2022/02/22 19:25:35 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*check_dead(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->monitoring);
		sem_wait(philo->check_finish);
		if (get_time() - philo->last_eat > philo->info->time_to_die ||
			philo->info->finish)
		{
			sem_post(philo->check_finish);
			sem_post(philo->monitoring);
			print(DEAD, philo);
			sem_wait(philo->info->print);
			sem_wait(philo->check_finish);
			philo->info->finish = 1;
			sem_post(philo->check_finish);
			sem_post(philo->info->stop);
			for (size_t i = 0; i <= philo->info->nb_philo; i++)
				sem_post(philo->info->stop);
			usleep(1000);
			sem_post(philo->info->print);
			break;
		}
		sem_post(philo->check_finish);
		sem_post(philo->monitoring);
		usleep(15);
	}
	return (NULL);
}

void	*check_other_died(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	sem_wait(philo->info->stop);
	sem_wait(philo->check_finish);
	philo->info->finish = 1;
	sem_post(philo->check_finish);
	sem_post(philo->info->stop);
	return (NULL);
}

int	launch_thread(t_philo *philo)
{
	if (pthread_create(&philo->check_dead, NULL, check_dead, philo))
		return (0);
	if (pthread_create(&philo->check_other_died, NULL, check_other_died, philo))
		return (0);
	return (1);
}
