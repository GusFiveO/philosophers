/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:13:02 by alorain           #+#    #+#             */
/*   Updated: 2022/02/18 15:12:58 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
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
	info->nb_philo = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]) * 1000;
	info->time_to_sleep = ft_atoi(argv[4]) * 1000;
	if (argc == 6)
		info->nb_t_philo_m_eat = ft_atoi(argv[5]);
	else
		info->nb_t_philo_m_eat = -1;
	if (info->nb_t_philo_m_eat == 0 || info->nb_philo == 0)
	{
		free(info);
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
	info->group = master;
	info->start_time = 0;
	info->finish = 0;
	info->eated = 0;
	info->philo = malloc(sizeof(t_philo) * info->nb_philo);
	if (!info->philo)
		return (0);
	while (i < info->nb_philo)
	{
		info->philo[i].last_eat = 0;
		info->philo[i].eat = 0;
		info->philo[i].eated = 0;
		info->philo[i].info = info;
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_info	*info;

	info = malloc(sizeof(t_info) * 1);
	if (!info)
		return (-1);
	if (argc >= 5 && argc <= 6)
	{
		if (check_args(argc, argv))
		{	
			if (!fill_struct(info, argc, argv))
				return (-1);
			launch_process(info);
			free(info->philo);
			free(info);
			return (1);
		}
		free(info);
		return (-1);
	}
	printf("Error");
	return (-1);
}
