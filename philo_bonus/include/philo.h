/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:11:48 by alorain           #+#    #+#             */
/*   Updated: 2022/02/11 16:37:46 by alorain          ###   ########.fr       */
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
# define EAT "\033[0;32m is eating\033[0m"
# define SLEEP "\033[0;34m is sleeping\033[0m"
# define THINK "\033[0;35m is thinking\033[0m"
# define DEAD "\033[0;31m is dead\033[0m"
# define FORMAT " %-4ld ms philo [%ld] %s\n"

typedef int				t_group;

typedef struct s_philo	t_philo;

enum e_group
{
	odd = 0,
	even,
	master
};

typedef struct s_info
{
	int				*pid_tab;
	int				group;
	int				finish;
	t_philo			*philo;
	size_t			nb_philo;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				nb_t_philo_m_eat;
	size_t			start_time;
	int				first_loop;
	sem_t			*dead;
	sem_t			*eat;
	pthread_t		eat_check;
}	t_info;

typedef struct s_philo
{
	t_info			*info;
	size_t			eated;
	size_t			idx;
	size_t			last_eat;
	size_t			dead;
	sem_t			*p_eat;
	sem_t			*fork;
	sem_t			*set_time;
	pthread_t		id;
}	t_philo;

/*-----main.c-----*/

int		ft_isdigit(int c);

/*-----utils.c-----*/

int		ft_atoi(const char *str);
size_t	get_time(void);
int		create_semaphore(sem_t **sem, const char *prefix, size_t val, size_t i);
size_t	ft_strlen(const char *str);
char	*create_name(const char *prefix, size_t idx);

/*-----thread.c-----*/

int		launch_process(t_info *info);

/*-----semaphores.c-----*/

void	close_sem(t_info *info);
int		init_semaphores(t_info *info);
int		create_philo_sem(t_info *info, size_t i);

/*-----routine.c-----*/

void	routine(t_philo *philo);

/*-----thread.c-----*/

int	launch_thread(t_philo *philo);
int	launch_eat_thread(t_info *info);

#endif
