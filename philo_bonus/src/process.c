/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:52:49 by alorain           #+#    #+#             */
/*   Updated: 2022/02/09 19:04:35 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	routine(t_philo *philo)
{
	t_info	*info;
	t_philo	*neighbor;

	info = philo->info;
	if (philo->idx == info->nb_philo)
		neighbor = &info->philo[0];
	else
		neighbor = &info->philo[philo->idx + 1];
	(void)neighbor;
	sem_wait(philo->fork);
	//sem_wait(neighbor->fork);
  	printf("lol %d %ld\n", getpid(), philo->idx);
	//sem_post(philo->fork);
	sem_post(neighbor->fork);
}

int	init_semaphores(t_info *info)
{
	size_t	i;

	i = 0;
	while (i < info->nb_philo)
	{
		info->philo[i].fork = sem_open("fork", O_CREAT, S_IWOTH, 1);
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
	while (i < info->nb_philo)
	{
		pid_tab[i] = fork();
		if (pid_tab[i] == 0)
		{
			info->philo[i].idx = i + 1; 
			routine(&info->philo[i]);
			break ;
		}
		i++;
	}
	for (size_t j = 0; j < info->nb_philo; j++)
		waitpid(pid_tab[i], NULL, 0);
	return (1);
}
