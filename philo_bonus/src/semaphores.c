/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 16:55:25 by alorain           #+#    #+#             */
/*   Updated: 2022/02/14 16:48:38 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philo_sem(t_info *info)
{
	size_t	i;

	i = 0;
	while (i < info->nb_philo)
	{
		if (!create_semaphore(&info->philo[i].monitoring, "monitor", 1, i))
		{
			while (i--)
			{
				sem_close(info->philo[i].monitoring);
				sem_close(info->philo[i].fork);
				return (0);
			}
		}
		if (!create_semaphore(&info->philo[i].fork, "fork", 1, i))
		{
			while (i--)
			{
				sem_close(info->philo[i].monitoring);
				sem_close(info->philo[i].fork);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	init_semaphores(t_info *info)
{
	if (!create_semaphore(&info->stop, "stop", 0, 0))
		return (0);
	if (!create_semaphore(&info->print, "print", 1, 0))
	{
		sem_close(info->eat);
		return (0);
	}
	if (!create_semaphore(&info->eat, "eat", 0, 0))
	{
		sem_close(info->print);
		sem_close(info->eat);
		return (0);
	}
	if (!create_philo_sem(info))
	{
		sem_close(info->eat);
		sem_close(info->stop);
		return (0);
	}
	return (1);	
}
