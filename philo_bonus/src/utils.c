/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 12:21:30 by alorain           #+#    #+#             */
/*   Updated: 2022/02/22 18:41:13 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_atoi(const char *str)
{
	size_t	i;
	int		ret;
	int		signe;

	i = 0;
	ret = 0;
	signe = 1;
	while (str[i] == '\t' || str[i] == ' ' || str[i] == '\n'
		|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f')
		i++;
	if (str[i] == '-')
		signe = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (ft_isdigit(str[i]))
	{
		ret = ret * 10 + str[i] - '0';
		i++;
	}
	return (ret * signe);
}

char	*create_name(const char *prefix, size_t idx)
{
	size_t	len;
	size_t	i;
	char	*ret;

	len = ft_strlen(prefix);
	ret = malloc(sizeof(char) * len + 3);
	i = 0;
	ret[i] = '/';
	while (prefix[i])
	{
		ret[i + 1] = prefix[i];
		i++;
	}
	ret[i + 1] = idx + '0';
	ret[i + 2] = '\0';
	return (ret);
}

int	create_semaphore(sem_t **sem, const char *prefix, size_t value, size_t idx)
{
	char	*name;

	name = create_name(prefix, idx);
	if (!name)
		return (0);
	*sem = sem_open(name, O_CREAT | O_EXCL, 0644, value);
	if (sem_unlink(name) == -1)
	{
		free(name);
		return (0);
	}
	free(name);
	return (1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
