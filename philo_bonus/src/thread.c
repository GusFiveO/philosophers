/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 14:06:41 by alorain           #+#    #+#             */
/*   Updated: 2022/02/11 17:10:44 by alorain          ###   ########.fr       */
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
		sem_wait(philo->set_time);
		if (get_time() - philo->last_eat > info->time_to_die)
		{
			sem_post(info->dead);
			sem_post(philo->set_time);
			printf(FORMAT, get_time() - info->start_time, philo->idx, DEAD);
			return (arg);
		}
		sem_post(philo->set_time);
	}
	return (arg);
}

void	*eat_check(void *arg)
{
	t_info	*info;
	size_t	i;
	size_t	eated;

	info = (t_info *)arg;
	i = 0;
	eated = 0;
	while (eated != info->nb_philo * info->nb_t_philo_m_eat)
	{
		sem_wait(info->eat);
		eated++;
	}
	sem_post(info->dead);
	while (i < info->nb_philo)
		kill(info->pid_tab[i++], SIGSTOP);
	free(info->pid_tab);
	i = 0;
	while (i < info->nb_philo)
	{
		sem_close(info->philo[i].fork);
		i++;
	}
	return (arg);
}

int	launch_thread(t_philo *philo)
{
	int		ret;

	ret = 0;
	ret = pthread_create(&philo->id, NULL, check_death, philo);
	if (ret)
		return (0);
	ret = pthread_detach(philo->id);
	if (ret)
			return(0);
	return (1);
}

int	launch_eat_thread(t_info *info)
{
	int		ret;

	ret = 0;
	if (info->nb_t_philo_m_eat == -1)
		return (1);
	ret = pthread_create(&info->eat_check, NULL, eat_check, info);
	if (ret)
		return (0);
	ret = pthread_detach(info->eat_check);
	if (ret)
			return(0);
	return (1);
}
