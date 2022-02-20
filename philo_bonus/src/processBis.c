/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processBis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: augustinlorain <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 16:14:57 by augustinlorai     #+#    #+#             */
/*   Updated: 2022/02/20 19:31:16 by augustinlorai    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lauch_thread(t_philo *philo)
{
	if (pthread_create(&philo->check_dead, NULL, check_dead, philo))
		return (0);
	if (pthread_create(&philo->check_other_died, NULL, check_other_died, philo))
		return (0);
}

int	launch_philo(t_info *info)
{
	size_t	i;

	i = 0;
	info->start_time = get_time();
	while (i < info->nb_philo)
	{
		info->pid_tab[i] = fork();
		if (info->pid_tab[i] < 0)
			return (0);
		if (info->pid_tab[i] == 0)
		{
			if (!launch_thread(info->philo[i]))
				return (0);
			routine(info->philo[i]);
			return (1);
		}
		i++;
	}
	return (1);
}

void	*eat_check(void	*arg)
{
	size_t	eat;

	eat = 0;
	while (eat != info->nb_philo * info->nb_t_philo_m_eat)
	{
		sem_wait(info->eat);
		eat++;
		sem_wait(info->stop);
		if (info->finish)
		{
			sem_post(info->stop);
			break;
		}
		sem_post(info->stop);
	}
	return (NULL);
}

void	end_program(t_info *info)
{
	size_t	i;

	i = 0;
	free(info->pid_tab);
	sem_close(info->eat);
	sem_close(info->stop);
	sem_close(info->forks);
	while (i < info->nb_philo)
	{
		sem_close(info->check_finish);
		sem_close(info->monitoring);
	}
}

void	process(t_info *info)
{
	size_t	i;

	i = 0;
	lanch_philo(info);
	if (info->nb_t_philo_m_eat != -1)
		if (pthread_create(&info->eat_check, NULL, eat_check, info))
			return (0);
	while (i < info->nb_philo)
		waitpid(info->pid_tab[i++], NULL, 0);
	sem_wait(info->stop)
	info->finish = 1;
	sem_post(info->stop);
	if (info->nb_t_philo_m_eat != -1)
		pthread_join(info->eat_check, NULL);
	end_program(info);
}
