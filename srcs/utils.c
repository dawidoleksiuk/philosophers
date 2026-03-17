/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doleksiu <doleksiu@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 20:01:46 by doleksiu          #+#    #+#             */
/*   Updated: 2026/03/17 20:37:08 by doleksiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	ft_sleep(long time)
{
	struct timeval	start;
	struct timeval	end;
	long			time_elapsed;

	gettimeofday(&start, NULL);
	usleep(time * 1000 * 0.7);
	while (1)
	{
		gettimeofday(&end, NULL);
		time_elapsed = (end.tv_sec - start.tv_sec) * 1000;
		time_elapsed += (end.tv_usec - start.tv_usec) / 1000;
		if (time_elapsed >= time)
			break ;
		usleep(1000);
	}
}

int	str_to_int(char *str, long *arg)
{
	int	i;

	i = 0;
	*arg = 0;
	while (str[i])
	{
		if (*arg > (UINT_MAX - (str[i] - '0')) / 10)
			return (1);
		*arg *= 10;
		*arg += (str[i] - '0');
		i++;
	}
	if (*arg > UINT_MAX / 1000)
		return (1);
	return (0);
}

int	data_validation(int argc, char *argv[])
{
	int	i;
	int	j;

	i = 1;
	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die[ms] "
			"time_to_eat[ms] time_to_sleep[ms] "
			"[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
				return (printf("All arguments must be positive numbers.\n"), 1);
			j++;
		}
		i++;
	}
	return (0);
}

int	print(t_data *data, int philo_id, char *str)
{
	struct timeval	current_time;
	long			time;
	int				dead;

	pthread_mutex_lock(&data->mutex_print);
	if (!data->someone_died)
	{
		gettimeofday(&current_time, NULL);
		time = (current_time.tv_sec - data->start.tv_sec) * 1000;
		time += (current_time.tv_usec - data->start.tv_usec) / 1000;
		printf("%ld %d %s\n", time, philo_id, str);
		pthread_mutex_unlock(&data->mutex_print);
		return (0);
	}
	pthread_mutex_unlock(&data->mutex_print);
	return (1);
}
