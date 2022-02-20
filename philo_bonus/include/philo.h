/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:11:48 by alorain           #+#    #+#             */
/*   Updated: 2022/02/20 19:13:08 by augustinlorai    ###   ########.fr       */
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
	sem_t			*stop;
	sem_t			*eat;
	sem_t			*forks;
	pthread_t		eat_check;
}	t_info;

typedef struct s_philo
{
	t_info			*info;
	size_t			eated;
	size_t			idx;
	size_t			last_eat;
	sem_t			*check_finish;
	sem_t			*monitoring;
	pthread_t		check_dead;
	pthread_t		check_other_died;
}	t_philo;

/*-----main.c-----*/

int		ft_isdigit(int c);

/*-----utils.c-----*/

int		ft_atoi(const char *str);
size_t	get_time(void);
int		create_semaphore(sem_t **sem, const char *prefix, size_t val, size_t i);
size_t	ft_strlen(const char *str);
char	*create_name(const char *prefix, size_t idx);

/*-----process.c-----*/

int		launch_process(t_info *info);

/*-----semaphores.c-----*/

void	close_sem(t_info *info);
int		init_semaphores(t_info *info);
int		create_philo_sem(t_info *info);

/*-----routine.c-----*/

void	smart_sleep(size_t time);
void	routine(t_philo *philo);

/*-----thread.c-----*/

int		launch_philo(t_philo *philo);
int		launch_eat_thread(t_info *info);

#endif
