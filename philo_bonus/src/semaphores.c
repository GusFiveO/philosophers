/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 16:55:25 by alorain           #+#    #+#             */
/*   Updated: 2022/02/11 16:53:09 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	close_sem(t_info *info)
{
	size_t	i;

	i = 0;
	sem_close(info->eat);
	sem_close(info->dead);
	while (i < info->nb_philo)
	{
		sem_close(info->philo[i].fork);
		sem_close(info->philo[i].p_eat);
	}
}

int	create_philo_sem(t_info *info, size_t i)
{
	int	j;

	j = i;
	if (!create_semaphore(&info->philo[i].fork, "fork", 1, i))
	{	
		while (j--)
			sem_close(info->philo[j].fork);
		sem_close(info->eat);
		return (0);
	}
	if (!create_semaphore(&info->philo[i].p_eat, "p_eat", 0, i))
	{	
		while (i--)
		{
			sem_close(info->philo[j].fork);
			sem_close(info->philo[i].p_eat);
			j--;
		}
		sem_close(info->eat);
		return (0);
	}
	if (!create_semaphore(&info->philo[i].set_time, "set_time", 1, i))
	{	
		while (i--)
		{
			sem_close(info->philo[j].fork);
			sem_close(info->philo[j].p_eat);
			sem_close(info->philo[i].set_time);
			j--;
		}
		sem_close(info->eat);
		return (0);
	}
	return (1);
}

int	init_semaphores(t_info *info)
{
	size_t	i;

	i = 0;

	if (!create_semaphore(&info->dead, "dead", 0, 0))
		return (0);
	if (!create_semaphore(&info->eat, "eat", 0, 0))
	{
		sem_close(info->dead);
		return (0);
	}
	while (i < info->nb_philo)
	{
		if (!create_philo_sem(info, i))
			return (0);
		usleep(100);
		i++;
	}
	return (1);
}
