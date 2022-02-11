/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:52:49 by alorain           #+#    #+#             */
/*   Updated: 2022/02/11 17:05:18 by alorain          ###   ########.fr       */
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
				info->philo[i].last_eat = get_time();
				launch_thread(&info->philo[i]);
				routine(&info->philo[i]);
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
				info->philo[i].last_eat = get_time();
				launch_thread(&info->philo[i]);
				routine(&info->philo[i]);
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
	usleep(200);
	create_even(info);
	if (!launch_eat_thread(info))
		return (1);
	sem_wait(info->dead);
//	while (i++ < info->nb_philo)
//		waitpid(-1, NULL, 0);
	while (i < info->nb_philo)
		kill(info->pid_tab[i++], SIGSTOP);
	free(info->pid_tab);
	i = 0;
	while (i < info->nb_philo)
	{
		sem_close(info->philo[i].fork);
		i++;
	}
	return (1);
}
