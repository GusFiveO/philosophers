/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:11:48 by alorain           #+#    #+#             */
/*   Updated: 2022/02/24 14:06:22 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <signal.h>
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

# define INT_MAX 2147483647
# define INT_MIN -2147483648

typedef int				t_group;

typedef struct s_philo	t_philo;

enum e_group
{
	master = 1,
	philo	
};

typedef struct s_info
{
	int				*pid_tab;
	int				group;
	t_philo			*philo;
	size_t			nb_philo;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	int				nb_t_philo_m_eat;
	size_t			start_time;
	size_t			finish;
	sem_t			*check_stop;
	sem_t			*stop;
	sem_t			*eat;
	sem_t			*forks;
	sem_t			*print;
	pthread_t		eat_check;
}	t_info;

typedef struct s_philo
{
	t_info			*info;
	size_t			idx;
	size_t			last_eat;
	sem_t			*check_finish;
	sem_t			*monitoring;
	pthread_t		check_dead;
	pthread_t		check_other_died;
}	t_philo;

/*-----utils.c-----*/

int		ft_isdigit(int c);
int		ft_strcmp(const char *s1, const char *s2);
long	ft_atol(const char *str);
size_t	get_time(void);
size_t	ft_strlen(const char *str);

/*-----process.c-----*/

int		launch_philo(t_info *info);
int		launch_process(t_info *info);

/*-----semaphores.c-----*/

void	close_sem(t_info *info);
int		init_semaphores(t_info *info);
int		create_philo_sem(t_info *info);

/*-----routine.c-----*/

void	print(const char *str, t_philo *philo);
void	routine(t_philo *philo);

/*-----thread.c-----*/

int		launch_thread(t_philo *philo);
void	*check_dead(void *arg);
void	*check_other_died(void *arg);

#endif
