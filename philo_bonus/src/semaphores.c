/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 16:55:25 by alorain           #+#    #+#             */
/*   Updated: 2022/02/23 14:59:05 by alorain          ###   ########.fr       */
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
				sem_close(info->philo[i].check_finish);
			}
			return (0);
		}
		if (!create_semaphore(&info->philo[i].check_finish, "finish", 1, i))
		{
			while (i--)
			{
				sem_close(info->philo[i].monitoring);
				sem_close(info->philo[i].check_finish);
			}
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_semaphores2(t_info *info)
{
	create_semaphore(&info->print, "print", 1, 0);
	if (!create_semaphore(&info->eat, "eat", 1, 0))
	{
		sem_close(info->stop);
		sem_close(info->check_stop);
		sem_close(info->forks);
		return (0);
	}
	sem_wait(info->eat);
	if (!create_philo_sem(info))
	{
		sem_close(info->check_stop);
		sem_close(info->eat);
		sem_close(info->stop);
		sem_close(info->forks);
		return (0);
	}
	return (1);
}

int	init_semaphores(t_info *info)
{
	if (!create_semaphore(&info->stop, "stop", 0, 0))
		return (0);
	sem_post(info->stop);
	sem_wait(info->stop);
	if (!create_semaphore(&info->check_stop, "check_stop", 1, 0))
	{
		sem_close(info->stop);
		return (0);
	}
	if (!create_semaphore(&info->forks, "forks", info->nb_philo, 0))
	{
		sem_close(info->stop);
		sem_close(info->check_stop);
		return (0);
	}
	if (!init_semaphores2(info))
		return (0);
	return (1);
}
