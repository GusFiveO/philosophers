/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alorain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 11:59:42 by alorain           #+#    #+#             */
/*   Updated: 2022/02/24 14:18:01 by alorain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	put_action(size_t timestamp, const char *action, size_t idx)
{
	ft_putnbr(timestamp);
	ft_putstr(" ms philo ");
	ft_putnbr(idx);
	ft_putstr(action);
}

void	ft_putstr(const char *str)
{
	size_t	len;

	if (!str)
		return ;
	len = ft_strlen(str);
	write(1, str, len);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(size_t n)
{
	if (n >= 10)
	{
		ft_putnbr(n / 10);
		ft_putnbr(n % 10);
	}
	else
		ft_putchar(n + '0');
}
