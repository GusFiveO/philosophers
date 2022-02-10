/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/10 14:06:41 by alorain           #+#    #+#             */
/*   Updated: 2022/02/10 18:23:54 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*death_checker(void *arg)
{
	t_info	*info;
	t_philo	*philo;
	int		finish;
	size_t	start_eat;
	int		pid;

	philo = (t_philo *)arg;
	finish = 0;
	info = philo->info;
	pid = 0;
	size_t	i = 0;
	while (1)
	{
		sem_wait(philo->p_eat);
		start_eat = get_time();
		pid = fork();
		while (!finish)
		{
			if (pid == 0)
			{
				if (get_time() - start_eat >= info->time_to_die)
				{
					printf(FORMAT, get_time() - info->start_time, philo->idx, DEAD);
					while (i < info->nb_philo)
						kill(info->pid_tab[i++], SIGSTOP);
					free(info->pid_tab);
					return (NULL);
				}
			}
			else
			{
				sem_wait(philo->p_eat);
				kill(pid , SIGSTOP);
				finish = 1;
			}
			//usleep(100);
		}
	}
	return (NULL);
}

int	launch_thread(t_info *info)
{
	size_t		i;
	pthread_t	*id_tab;
	int			ret;

	i = 0;
	ret = 0;
	id_tab = malloc(sizeof(pthread_t) * info->nb_philo);
	if (!id_tab)
		return (0);
	while (i < info->nb_philo)
	{
		ret = pthread_create(&id_tab[i], NULL, death_checker, &info->philo[i]);
		if (ret)
			return (0);
		i++;
	}
//	while (i < info->nb_philo)
//		pthread_join(id_tab[i++], NULL);
//	free(id_tab);
	return (1);

}
