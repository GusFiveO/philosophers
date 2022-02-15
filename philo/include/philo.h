/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:11:48 by alorain           #+#    #+#             */
/*   Updated: 2022/02/15 15:21:24 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

# define FORK "\033[0;33m has taken a fork\n\033[0m"
# define EAT "\033[0;32m is eating\n\033[0m"
# define SLEEP "\033[0;34m is sleeping\n\033[0m"
# define THINK "\033[0;35m is thinking\n\033[0m"
# define DEAD "\033[0;31m is dead\n\033[0m"
# define FORMAT " %-4ld ms philo [%ld] %s\n"

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
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	eat;
	pthread_mutex_t	m_stop;
}	t_info;

typedef struct s_philo
{
	t_info			*info;
	size_t			eated;
	size_t			idx;
	size_t			last_eat;
	size_t			dead;
	size_t			left_fork;
	size_t			right_fork;
	size_t			time;
	pthread_mutex_t	mutex;
}	t_philo;

/*-----main.c-----*/

int		init_mutex(t_info *info);

/*-----write_utils.c-----*/

void	ft_putchar(char c);
void	ft_putnbr(size_t n);
void	ft_putstr(const char *str);
void	put_action(size_t timestamp, const char *action, size_t idx);

/*-----utils.c-----*/

int		ft_atoi(const char *str);
int		ft_isdigit(int c);
size_t	get_time(void);
void	smart_sleep(size_t time);

/*-----thread.c-----*/

int		launch_thread(t_info *info);

/*-----routine.c-----*/

void	routine(t_philo *philo);

/*-----mutex.c-----*/

int		init_philo_mutex(t_info *info);
void	lock_forks(t_info *info, size_t idx);
void	mutex_eat(t_philo *philo);
void	unlock_forks(t_info *info, size_t idx);
void	print(t_philo *philo, const char *str);

#endif
