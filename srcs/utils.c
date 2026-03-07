/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 20:01:46 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/07 20:56:00 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	get_time(long time)
{
	struct timeval start;
	struct timeval end;
	long	time_elapsed;

	gettimeofday(&start, NULL);
	while (1)
	{
		usleep(500);
		gettimeofday(&end, NULL);
		time_elapsed = ((end.tv_sec - start.tv_sec) * 1000) + ((end.tv_usec - start.tv_usec) / 1000);
		if (time_elapsed >= time)
		{
			printf("%ld", time_elapsed);
			return (1);
		}
	}
	return (0);
}
