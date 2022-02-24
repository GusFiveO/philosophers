/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:13:02 by alorain           #+#    #+#             */
/*   Updated: 2022/02/24 14:20:54 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	if (!check_int_overflow(argc, argv))
	{	
		printf("Error in args\n");
		return (0);
	}
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (!ft_isdigit(argv[i][j]))
			{	
				printf("Error in args\n");
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	parse_args(t_info *info, int argc, char **argv)
{
	info->nb_philo = ft_atol(argv[1]);
	info->time_to_die = ft_atol(argv[2]);
	info->time_to_eat = ft_atol(argv[3]) * 1000;
	info->time_to_sleep = ft_atol(argv[4]) * 1000;
	if (argc == 6)
		info->nb_t_philo_m_eat = ft_atol(argv[5]);
	else
		info->nb_t_philo_m_eat = -1;
	if (info->nb_t_philo_m_eat == 0 || info->nb_philo == 0)
	{
		free(info);
		return (0);
	}
	info->forks = malloc(sizeof(pthread_mutex_t) * info->nb_philo);
	if (!info->forks)
		return (0);
	info->philo = malloc(sizeof(t_philo) * info->nb_philo);
	if (!info->philo)
	{
		free(info->forks);
		return (0);
	}
	return (1);
}

int	fill_struct(t_info *info, int argc, char **argv)
{
	size_t	i;

	i = 0;
	if (!parse_args(info, argc, argv))
		return (0);
	info->stop = 0;
	info->start_time = 0;
	info->nb_time_all_philo_eat = 0;
	while (i < info->nb_philo)
	{
		info->philo[i].last_eat = 0;
		info->philo[i].left_fork = i;
		if (i == info->nb_philo - 1)
			info->philo[i].right_fork = 0;
		else
			info->philo[i].right_fork = i + 1;
		info->philo[i].dead = 0;
		info->philo[i].eated = 0;
		info->philo[i].info = info;
		i++;
	}
	return (1);
}

int	init_mutex(t_info *info)
{
	if (pthread_mutex_init(&info->m_stop, NULL))
		return (1);
	if (pthread_mutex_init(&info->eat, NULL))
	{
		pthread_mutex_destroy(&info->m_stop);
		return (1);
	}
	if (pthread_mutex_init(&info->print, NULL))
	{
		pthread_mutex_destroy(&info->m_stop);
		pthread_mutex_destroy(&info->eat);
		return (1);
	}
	if (init_philo_mutex(info))
	{
		pthread_mutex_destroy(&info->m_stop);
		pthread_mutex_destroy(&info->eat);
		pthread_mutex_destroy(&info->print);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_info	*info;

	info = malloc(sizeof(t_info) * 1);
	if (argc >= 5 && argc <= 6 && info)
	{
		if (check_args(argc, argv))
		{	
			if (!fill_struct(info, argc, argv))
				return (-1);
			launch_thread(info);
			free(info->forks);
			free(info->philo);
			free(info);
			return (1);
		}
		return (-1);
	}
	printf("Error");
	return (-1);
}
