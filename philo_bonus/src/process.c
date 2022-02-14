/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:52:49 by alorain           #+#    #+#             */
/*   Updated: 2022/02/14 16:46:33 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_even(t_info *info)
{
	size_t	i;

	i = 0;
	if (info->group == odd)
		return (1);
	while (i < info->nb_philo)
	{
		if (i % 2)
		{
			info->pid_tab[i] = fork();
			if (info->pid_tab[i] == 0)
			{
				info->group = even;
				info->philo[i].idx = i + 1;
				launch_philo(&info->philo[i]);
				break ;
			}
		}
		i++;
	}
	return (1);
}

int	create_odd(t_info *info)
{
	size_t	i;

	i = 0;
	while (i < info->nb_philo)
	{
		if (!(i % 2))
		{
			info->pid_tab[i] = fork();
			if (info->pid_tab[i] == 0)
			{
				info->group = odd;
				info->philo[i].idx = i + 1;
				launch_philo(&info->philo[i]);
				break ;
			}
		}
		i++;
	}
	return (1);
}


int	launch_process(t_info *info)
{
	size_t	i;

	i = 0;
	info->pid_tab = malloc(sizeof(int) * info->nb_philo);
	if (!init_semaphores(info))
		return (0);
	info->start_time = get_time();
	create_odd(info);
	smart_sleep(info->time_to_eat - 1000);
	create_even(info);
	launch_eat_thread(info);

	sem_wait(info->stop);
	while (i < info->nb_philo)
		kill(info->pid_tab[i++], SIGSTOP);
	free(info->pid_tab);
	i = 0;
	sem_close(info->print);
	sem_close(info->eat);
	sem_close(info->stop);
	while (i < info->nb_philo)
	{
		sem_close(info->philo[i].fork);
		sem_close(info->philo[i].monitoring);
		i++;
	}
	return (1);
}
