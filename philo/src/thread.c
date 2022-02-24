/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:52:49 by alorain           #+#    #+#             */
/*   Updated: 2022/02/24 12:19:36 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*fn_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (!(philo->idx % 2))
		usleep(philo->info->time_to_eat);
	if (philo->info->nb_philo == 1)
	{
		philo->info->start_time = get_time();
		print(philo, FORK);
		usleep(philo->info->time_to_die * 1000);
		print(philo, DEAD);
		pthread_mutex_lock(&philo->info->m_stop);
		philo->info->stop = 1;
		pthread_mutex_unlock(&philo->info->m_stop);
		return (arg);
	}
	pthread_mutex_lock(&philo->info->m_stop);
	while (!philo->info->stop)
	{	
		pthread_mutex_unlock(&philo->info->m_stop);
		routine(philo);
		pthread_mutex_lock(&philo->info->m_stop);
	}
	pthread_mutex_unlock(&philo->info->m_stop);
	return (arg);
}

void	check_dead(t_info *info)
{
	size_t	i;

	i = 0;
	pthread_mutex_lock(&info->m_stop);
	while (!info->stop)
	{
		pthread_mutex_lock(&info->philo[i].mutex);
		if ((info->philo[i].last_eat
				&& get_time() - info->philo[i].last_eat > info->time_to_die))
		{
			put_action(get_time() - info->start_time, DEAD, i + 1);
			info->stop = 1;
			pthread_mutex_unlock(&info->m_stop);
			pthread_mutex_unlock(&info->philo[i].mutex);
			return ;
		}
		pthread_mutex_unlock(&info->philo[i].mutex);
		i++;
		i = i % info->nb_philo;
		pthread_mutex_unlock(&info->m_stop);
		usleep(10);
		pthread_mutex_lock(&info->m_stop);
	}
	pthread_mutex_unlock(&info->m_stop);
}

int	create_thread(pthread_t *id_tab, t_info *info)
{
	size_t	i;
	int		ret;

	i = 0;
	ret = 0;
	while (i < info->nb_philo)
	{
		info->philo[i].idx = i + 1;
		ret = pthread_create(&id_tab[i], NULL, fn_philo, &info->philo[i]);
		if (ret)
			return (ret);
		i++;
	}
	return (0);
}

int	launch_thread(t_info *info)
{
	size_t			i;
	int				ret;
	pthread_t		*id_tab;

	i = 0;
	ret = 0;
	ret = init_mutex(info);
	if (ret)
		return (0);
	id_tab = malloc(sizeof(pthread_t) * info->nb_philo);
	if (!id_tab)
		return (0);
	ret = create_thread(id_tab, info);
	if (ret)
		return (0);
	check_dead(info);
	i = 0;
	while (i < info->nb_philo)
		pthread_join(id_tab[i++], NULL);
	free(id_tab);
	return (1);
}
