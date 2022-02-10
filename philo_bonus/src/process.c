/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:52:49 by alorain           #+#    #+#             */
/*   Updated: 2022/02/10 15:13:02 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_semaphore(sem_t **sem, const char *prefix, size_t value, size_t idx)
{
	char	*name;

	name = create_name(prefix, idx);
	if (!name)
		return (0);
	*sem = sem_open(name, O_CREAT | O_EXCL, 0644, value);
	if (sem_unlink(name) == -1)
	{
		free(name);
		return (0);
	}
	free(name);
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
		if (!create_semaphore(&info->philo[i].fork, "fork", 1, i))
		{	
			while (i--)
				sem_close(info->philo[i].fork);
			sem_close(info->stop);
			sem_close(info->eat);
			return (0);
		}
		usleep(100);
		i++;
	}
	return (1);
}

int	create_even(t_info *info, int *pid_tab)
{
	size_t	i;

	i = 0;
	if (info->group == odd)
		return (1);
	while (i < info->nb_philo)
	{
		if (i % 2)
		{
			pid_tab[i] = fork();
			if (pid_tab[i] == 0)
			{
				info->group = even;
				info->philo[i].idx = i + 1;
				routine(&info->philo[i]);
				break ;
			}
			usleep(100);
		}
		i++;
	}
	return (1);
}

int	create_odd(t_info *info, int *pid_tab)
{
	size_t	i;

	i = 0;
	while (i < info->nb_philo)
	{
		if (!(i % 2))
		{
			pid_tab[i] = fork();
			if (pid_tab[i] == 0)
			{
				info->group = odd;
				info->philo[i].idx = i + 1;
				routine(&info->philo[i]);
				break ;
			}
			usleep(100);
		}
		i++;
	}
	return (1);
}

int	check_stop(t_info *info, int *pid_tab)
{
	size_t	i;
	size_t	eated;

	i = 0;
	eated = 0;
	while (eated != info->nb_philo * info->nb_t_philo_m_eat)
	{
		sem_wait(info->eat);
		eated++;
	}
	while (i < info->nb_philo)
		kill(pid_tab[i++], SIGSTOP);
	free(pid_tab);
	i = 0;
	while (i < info->nb_philo)
	{
		sem_close(info->philo[i].fork);
		i++;
	}
	return (1);
}

int	launch_process(t_info *info)
{
	size_t			i;
	int				*pid_tab;

	i = 0;
	pid_tab = malloc(sizeof(int) * info->nb_philo);
	if (!init_semaphores(info))
		return (0);
	info->start_time = get_time();
	create_odd(info, pid_tab);
	create_even(info, pid_tab);
	if (check_stop(info, pid_tab))
		return (1);
	while (i++ < info->nb_philo)
		waitpid(-1, NULL, 0);
	free(pid_tab);
	i = 0;
	while (i < info->nb_philo)
	{
		sem_close(info->philo[i].fork);
		i++;
	}
	return (1);
}
