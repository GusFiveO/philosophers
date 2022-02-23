/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processBis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augustinlorain <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:14:57 by augustinlorai     #+#    #+#             */
/*   Updated: 2022/02/23 12:00:11 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*eat_check(void	*arg)
{
	size_t	eat;
	t_info	*info;

	eat = 0;
	info = (t_info *)arg;
	while (eat != info->nb_philo * info->nb_t_philo_m_eat)
	{
		sem_wait(info->eat);
	//	sem_wait(info->print);
		eat++;
		sem_wait(info->check_stop);
		if (info->finish)
		{
			sem_post(info->check_stop);
			break;
		}
		sem_post(info->check_stop);
		//sem_post(info->print);
	}
	sem_post(info->stop);
	//usleep(2000);
	//sem_post(info->print);
	return (NULL);
}

void	end_program(t_info *info)
{
	size_t	i;

	i = 0;
	if (info->group == master)
		free(info->pid_tab);
	sem_close(info->eat);
	sem_close(info->stop);
	sem_close(info->forks);
	sem_close(info->check_stop);
	sem_close(info->print);
	while (i < info->nb_philo)
	{
		sem_close(info->philo[i].check_finish);
		sem_close(info->philo[i].monitoring);
		i++;
	}
}

int	launch_philo(t_info *info)
{
	size_t	i;

	i = 0;
	info->start_time = get_time();
	while (i < info->nb_philo)
	{
		info->philo[i].idx = i + 1;
		info->pid_tab[i] = fork();
		if (info->pid_tab[i] < 0)
			return (0);
		if (info->pid_tab[i] == 0)
		{
			info->group = philo;
			info->philo[i].last_eat = get_time();
			if (!launch_thread(&info->philo[i]))
				return (0);
			routine(&info->philo[i]);
			free(info->pid_tab);
			pthread_join(info->philo[i].check_dead, NULL);
			pthread_join(info->philo[i].check_other_died, NULL);
			return (1);
		}
		//usleep(1000);
		i++;
	}
	return (1);
}

int	launch_process(t_info *info)
{
	size_t	i;

	i = -1;
	init_semaphores(info);
	launch_philo(info);
	if (info->group == master && info->nb_t_philo_m_eat != -1)
		if (pthread_create(&info->eat_check, NULL, eat_check, info))
			return (0);
	while (++i < info->nb_philo && info->group == master)
		waitpid(info->pid_tab[i], NULL, 0);
	sem_wait(info->check_stop);
	sem_post(info->eat);
	info->finish = 1;
	sem_post(info->check_stop);
	if (info->group == master && info->nb_t_philo_m_eat != -1)
		pthread_join(info->eat_check, NULL);
	end_program(info);
	return (1);
}
