/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 16:55:25 by alorain           #+#    #+#             */
/*   Updated: 2022/02/10 17:27:15 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philo_sem(t_info *info, size_t i)
{
	int	j;

	j = i;
	if (!create_semaphore(&info->philo[i].fork, "fork", 1, i))
	{	
		while (j--)
		{
			sem_close(info->philo[j].fork);
			sem_close(info->philo[j].p_eat);
		}
		sem_close(info->stop);
		sem_close(info->eat);
		return (0);
	}
	if (!create_semaphore(&info->philo[i].p_eat, "p_eat", 0, i))
	{	
		while (i--)
		{
			sem_close(info->philo[j].fork);
			sem_close(info->philo[i].p_eat);
		}
		sem_close(info->stop);
		sem_close(info->eat);
		return (0);
	}
	return (1);
}

int	init_semaphores(t_info *info)
{
	size_t	i;

	i = 0;
	if (!create_semaphore(&info->stop, "stop", 0, 0))
		return (0);
	if (!create_semaphore(&info->eat, "eat", 0, 0))
	{
		sem_close(info->stop);
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
