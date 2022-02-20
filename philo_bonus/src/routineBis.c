/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routineBis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augustinlorain <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 15:41:18 by augustinlorai     #+#    #+#             */
/*   Updated: 2022/02/20 19:33:03 by augustinlorai    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	print(const char *str, t_philo *philo)
{
	sem_wait(philo->check_finish);
	if (info->finish)
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
	if (philo->idx % 2)
		usleep(philo->info->time_to_eat);
	sem_wait(philo->check_finish);
	while (!philo->info->finish)
	{
		sem_post(philo->check_finish);
		take_forks(FORKS, philo);
		eat(EAT, philo);
		ft_sleep(SLEEP, philo);
		print(THINK, philo);
	}
	sem_post(philo->check_finish);
}

void	*check_dead(void *arg)
{
	t_philo	*philo;
	size_t	i;

	i = 0;
	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->monitoring);
		if (get_time() - philo->last_eat > philo->info->time_to_die)
		{
			sem_post(philo->monitoring);
			sem_wait(philo->check_finish);
			philo->info->finish = 1;
			sem_post(philo->check_finish);
			while (i++ < philo->info->nb_philo)
				sem_post(philo->info->stop);
			break;
		}
		sem_post(philo->monitoring);
		usleep(10);
	}
	return (NULL);
}

void	*check_other_died(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	sem_wait(info->stop);
	sem_wait(philo->check_finish);
	philo->info->finish = 1;
	sem_post(philo->check_finish);
	return (NULL);
}
