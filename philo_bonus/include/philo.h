/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:11:48 by alorain           #+#    #+#             */
/*   Updated: 2022/02/09 18:58:44 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <semaphore.h>

# define FORK "\033[0;33m has taken a fork\033[0m"
# define EAT "\033[0;32m is eating \033[0m"
# define SLEEP "\033[0;34m is sleeping \033[0m"
# define THINK "\033[0;35m is thinking \033[0m"
# define DEAD "\033[0;31m is dead\033[0m"
# define FORMAT " %-4ld ms philo [%ld] %s\n"

typedef struct s_semaphores
{
	sem_t	*fork;
}	t_semaphores;
typedef struct s_philo	t_philo;

typedef struct s_info
{
	t_philo			*philo;
	size_t			stop;
	size_t			nb_philo;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				nb_t_philo_m_eat;
	size_t			start_time;
	size_t			nb_time_all_philo_eat;
}	t_info;

typedef struct s_philo
{
	t_info			*info;
	size_t			eated;
	size_t			idx;
	size_t			last_eat;
	size_t			dead;
	sem_t			*fork;
	//t_semaphores	*fork;
}	t_philo;


/*-----utils.c-----*/

int		ft_atoi(const char *str);
int		ft_isdigit(int c);
size_t	get_time(void);

/*-----thread.c-----*/

int		launch_process(t_info *info);

/*-----semaphores.c-----*/

int		init_semaphores(t_info *info);

#endif