/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 16:55:25 by alorain           #+#    #+#             */
/*   Updated: 2022/02/18 16:30:57 by alorain          ###   ########.fr       */
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
			while (i--)
				sem_close(info->philo[i].monitoring);
		i++;
	}
	return (1);
}

int	init_semaphores2(t_info *info)
{
	if (!create_semaphore(&info->eat, "eat", 0, 0))
	{
		sem_close(info->print);
		sem_close(info->stop);
		sem_close(info->forks);
		sem_close(info->check_finish);
		return (0);
	}
	if (!create_philo_sem(info))
	{
		sem_close(info->eat);
		sem_close(info->stop);
		sem_close(info->forks);
		sem_close(info->check_finish);
		return (0);
	}
	return (1);
}

int	init_semaphores(t_info *info)
{
	if (!create_semaphore(&info->stop, "stop", 0, 0))
		return (0);
	if (!create_semaphore(&info->check_finish, "finish", 1, 0))
	{
		sem_close(info->stop);
		return (0);
	}
	if (!create_semaphore(&info->forks, "forks", info->nb_philo, 0))
	{
		sem_close(info->stop);
		sem_close(info->check_finish);
		return (0);
	}
	if (!create_semaphore(&info->print, "print", 1, 0))
	{
		sem_close(info->stop);
		sem_close(info->forks);
		sem_close(info->check_finish);
		return (0);
	}
	if (!init_semaphores2(info))
		return (0);
	return (1);
}
