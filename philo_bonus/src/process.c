/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:52:49 by alorain           #+#    #+#             */
/*   Updated: 2022/02/18 16:25:37 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philo(t_info *info)
{
	size_t	i;

	i = 0;
	while (i < info->nb_philo)
	{
		info->pid_tab[i] = fork();
		if (info->pid_tab[i] == 0)
		{
			info->group = philo;
			info->philo[i].idx = i + 1;
			launch_philo(&info->philo[i]);
			break ;
		}
		i++;
	}
	return (1);
}

void	end_program(t_info *info)
{
	size_t	i;

	i = 0;
	while (i < info->nb_philo)
		kill(info->pid_tab[i++], SIGTERM);
	sem_post(info->eat);
	if (info->nb_t_philo_m_eat != 1)
		pthread_join(info->eat_check, NULL);
	i = 0;
	while (i < info->nb_philo)
		waitpid(info->pid_tab[i++], NULL, 0);
	free(info->pid_tab);
	i = 0;
	sem_close(info->print);
	sem_close(info->eat);
	sem_close(info->stop);
	sem_close(info->forks);
	sem_close(info->check_finish);
	while (i < info->nb_philo)
	{
		sem_close(info->philo[i].monitoring);
		i++;
	}
	usleep(info->time_to_die);
}

int	launch_process(t_info *info)
{
	info->pid_tab = malloc(sizeof(int) * info->nb_philo);
	if (!init_semaphores(info))
		return (0);
	info->start_time = get_time();
	create_philo(info);
	if (info->nb_t_philo_m_eat != -1)
		launch_eat_thread(info);
	sem_wait(info->stop);
	sem_wait(info->check_finish);
	info->finish = 1;
	sem_post(info->check_finish);
	end_program(info);
	return (1);
}
